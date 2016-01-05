#include <zdb.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>

#include "task_create.h"
#include "debug.h"
#include "log.h"
#include "sql_db.h"
#include "common.h"

const char *python_script = "#!/usr/bin/env python\n# -*- encoding: utf-8 -*-\n# Created on 2015-06-13 11:04:52\n# Project: job_51\n\nfrom pyspider.libs.base_handler import *\n\n\nclass Handler(BaseHandler):\n    crawl_config = {\n    }\n\n    @every(minutes=24 * 60)\n    def on_start(self):\n        self.crawl(\'http://search.51job.com/list/%s%%252C00,%%2B,%%2B,%%2B,%%2B,%%2B,%s,2,%%2B.html?lang=c&stype=1&image_x=21&image_y=24&specialarea=00\', callback=self.index_page)\n\n    @config(age=2)\n    def index_page(self, response):\n        for each in response.doc(\'a[href^=\"http\"]\').items():\n            self.crawl(each.attr.href, callback=self.detail_page)\n\n    @config(priority=2)\n    def detail_page(self, response):\n        return {\n            \"url\": response.url,\n            \"title\": response.doc(\'title\').text(),\n            \"content\": response.text,\n        }\n";
const char *pageing_python_script = "#!/usr/bin/env python\n# -*- encoding: utf-8 -*-\n# Created on 2015-06-13 11:04:52\n# Project: job_51\n\nfrom pyspider.libs.base_handler import *\n\n\nclass Handler(BaseHandler):\n    crawl_config = {\n    }\n\n    @every(minutes=24 * 60)\n    def on_start(self):\n        self.crawl(\'%s\', callback=self.index_page)\n\n    @config(age=2)\n    def index_page(self, response):\n        for each in response.doc(\'a[href^=\"http\"]\').items():\n            self.crawl(each.attr.href, callback=self.detail_page)\n\n    @config(priority=2)\n    def detail_page(self, response):\n        return {\n            \"url\": response.url,\n            \"title\": response.doc(\'title\').text(),\n            \"content\": response.text,\n        }\n";
const char *paging_url = "http://search.51job.com/jobsearch/search_result.php?fromJs=1&jobarea=%s%%2C00&district=000000&funtype=0000&industrytype=00&issuedate=9&providesalary=99&keyword=%s&keywordtype=2&curr_page=%d&lang=c&stype=1&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=01&companysize=99&lonlat=0%%2C0&radius=-1&ord_field=0&list_type=0&fromType=14";

#define SCRIPT_MAX 4096
#define PAGE_SIZE 20
#define BLANK "%20"
#define BACKSLASH "%2F"

static int save_project_to_db(char *project_name, char *script);
static void save_paging_project_to_db(char *project_name, char *keywords, char *locate);

void* project_create(void *arg)
{
	char script_buffer[SCRIPT_MAX] = { 0 };
	char key_words[128] = { 0 };
	int i, foot = 0;
	int ret = 0;

	pthread_detach(pthread_self());
	prctl(PR_SET_NAME, (unsigned long) "project_create");
	if (g_conf.project_name[0] == '\0' || g_conf.keywords[0] == '\0' || g_conf.locate[0] == '\0')
	{
		PRINT_ERROR("keywords OR locate is NULL\n");
		return 0;
	}
	for (i = 0; i < strlen(g_conf.keywords); i++)
	{
		if (g_conf.keywords[i] == ' ')
		{
			memcpy(key_words + foot, BLANK, strlen(BLANK));
			foot = foot + strlen(BLANK);
			continue;
		}
		if (g_conf.keywords[i] == '/')
		{
			memcpy(key_words + foot, BACKSLASH, strlen(BACKSLASH));
			foot = foot + strlen(BACKSLASH);
			continue;
		}
		key_words[foot++] = g_conf.keywords[i];
	}
	snprintf(script_buffer, SCRIPT_MAX, python_script, g_conf.locate, key_words);

	ret = save_project_to_db(g_conf.project_name, script_buffer);
	if(!ret)
	{
		save_paging_project_to_db(g_conf.project_name, key_words, g_conf.locate);
	}

	pthread_exit(0);
}

static void save_paging_project_to_db(char *project_name, char *keywords, char *locate)
{
	char insert_paging_project[] = "INSERT INTO projectdb.projectdb(name, status, script, rate, burst) VALUES (?, ?, ?, ?, ?)";
	char url_buffer[1024] = { 0 };
	char script_buffer[SCRIPT_MAX] = { 0 };
	char p_name[128];
	char *name = "%s_%d";
	const char *status = "STOP";
	int i;
	ResultSet_T result = NULL;
	Connection_T conn = NULL;
	PreparedStatement_T p = NULL;
	database db = PROJECT_DB;

	conn = sql_db_connect(db);
	TRY
	{
		for (i = 2; i <= PAGE_SIZE; i++)
		{
			snprintf(url_buffer, 1024, paging_url, locate, keywords, i);
//			PRINT_ERROR("url_buffer = %s\n", url_buffer);
			snprintf(script_buffer, SCRIPT_MAX, pageing_python_script, url_buffer);
			snprintf(p_name, 128, name, project_name, i);
			/* insert delete update*/
			p = Connection_prepareStatement(conn, insert_paging_project);
			PreparedStatement_setString(p, 1, p_name);
			PreparedStatement_setString(p, 2, status);
			PreparedStatement_setString(p, 3, script_buffer);
			PreparedStatement_setDouble(p, 4, 1.0);
			PreparedStatement_setDouble(p, 5, 8.0);
			PreparedStatement_execute(p);

			/* init */
			memset(url_buffer, '\0', 1024);
			memset(script_buffer, '\0', SCRIPT_MAX);
			memset(p_name, '\0', 128);
		}
	}
	CATCH(SQLException)
	{
		LOG_ERROR("SQLException -- %s\n", Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;
	PRINT_INFO("Paging Create Finish\n");
}

static int save_project_to_db(char *project_name, char *script)
{
	const char insert_sql[] = "INSERT INTO projectdb.projectdb(name, status, script, rate, burst) VALUES (?, ?, ?, ?, ?)";
	const char select_sql[] = "SELECT name FROM projectdb.projectdb WHERE name = ?;";
	Connection_T conn = NULL;
	PreparedStatement_T p = NULL;
	ResultSet_T result = NULL;
	const char *status = "STOP";
	int ret = 0;
	database db = PROJECT_DB;

	conn = sql_db_connect(db);
	TRY
	{
		/*1、select project name*/
		p = Connection_prepareStatement(conn, select_sql);
		PreparedStatement_setString(p, 1, project_name);
		result = PreparedStatement_executeQuery(p);
		if (ResultSet_next(result))
		{
			ret = 1;
		}
		if (!ret)
		{
			/* insert delete update*/
			p = Connection_prepareStatement(conn, insert_sql);
			PreparedStatement_setString(p, 1, project_name);
			PreparedStatement_setString(p, 2, status);
			PreparedStatement_setString(p, 3, script);
			PreparedStatement_setDouble(p, 4, 1.0);
			PreparedStatement_setDouble(p, 5, 8.0);
			PreparedStatement_execute(p);
		}
	}
	CATCH(SQLException)
	{
		LOG_ERROR("SQLException -- %s\n", Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;
	if(!ret)
	{
		LOG_TRACE("%s Project Create\n", project_name);
		PRINT_INFO("%s Project Create\n", project_name);
	}
	else
	{
		LOG_TRACE("%s Project Exist\n", project_name);
		PRINT_INFO("%s Project Exist\n", project_name);
	}
	return ret;
}
