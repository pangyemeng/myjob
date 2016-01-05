#include <zdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <time.h>

#include "sql_db.h"
#include "log.h"
#include "common.h"

#define URL_LEN 256
#define DB_TYPE 128

const char *project_db = "projectdb";
const char *task_db = "taskdb";

ConnectionPool_T g_myjobdb_pool = NULL;
ConnectionPool_T g_projectdb_pool = NULL;
ConnectionPool_T g_taskdb_pool = NULL;

URL_T g_projectdb_url = NULL;
URL_T g_myjobdb_url = NULL;
URL_T g_taskdb_url = NULL;

char g_db_type[DB_TYPE];

int sql_db_init()
{
	char db_url[URL_LEN] = { 0 };

	/* init myjobdb database pool */
	snprintf(db_url, URL_LEN, "%s://%s:%d/%s?user=%s&password=%s",
			g_conf.db_type, g_conf.ip, g_conf.sql_port, g_conf.db_name,
			g_conf.db_user, g_conf.db_pwd);

	g_myjobdb_url = URL_new(db_url);

	if (g_myjobdb_url == NULL)
	{
		LOG_ERROR("Create DB URL fail\n");
		return 0;
	}
	g_myjobdb_pool = ConnectionPool_new(g_myjobdb_url);
	if (g_myjobdb_url == NULL)
	{
		LOG_ERROR("Create database connection pool fail\n");
		return 0;
	}
	ConnectionPool_start(g_myjobdb_pool);

	/* init projectdb database pool */
	memset(db_url, '\0', URL_LEN);
	snprintf(db_url, URL_LEN, "%s://%s:%d/%s?user=%s&password=%s",
			g_conf.db_type, g_conf.ip, g_conf.sql_port, project_db,
			g_conf.db_user, g_conf.db_pwd);

	g_projectdb_url = URL_new(db_url);

	if (g_projectdb_url == NULL)
	{
		LOG_ERROR("Create DB URL fail\n");
		return 0;
	}
	g_projectdb_pool = ConnectionPool_new(g_projectdb_url);
	if (g_projectdb_pool == NULL)
	{
		LOG_ERROR("Create database connection pool fail\n");
		return 0;
	}
	ConnectionPool_start(g_projectdb_pool);

	/* init taskdb database pool */
	memset(db_url, '\0', URL_LEN);
	snprintf(db_url, URL_LEN, "%s://%s:%d/%s?user=%s&password=%s",
			g_conf.db_type, g_conf.ip, g_conf.sql_port, task_db,
			g_conf.db_user, g_conf.db_pwd);

	g_taskdb_url = URL_new(db_url);

	if (g_taskdb_url == NULL)
	{
		LOG_ERROR("Create DB URL fail\n");
		return 0;
	}
	g_taskdb_pool = ConnectionPool_new(g_taskdb_url);
	if (g_taskdb_pool == NULL)
	{
		LOG_ERROR("Create database connection pool fail\n");
		return 0;
	}
	ConnectionPool_start(g_taskdb_pool);

	return 1;
}

Connection_T sql_db_connect(int db)
{
	const char mysql[] = "mysql";
	const char oracle[] = "oracle";
	Connection_T conn = NULL;

	switch(db)
	{
	case MYJOB_DB:
		conn = ConnectionPool_getConnection(g_myjobdb_pool);
		break;
	case PROJECT_DB:
		conn = ConnectionPool_getConnection(g_projectdb_pool);
		break;
	case TASK_DB:
		conn = ConnectionPool_getConnection(g_taskdb_pool);
		break;
	default :
		LOG_ERROR("Database Type Error\n");
		break;
	}
	if (conn != NULL)
	{
		if (!strncmp(g_conf.db_type, mysql, strlen(mysql)))
		{
			Connection_execute(conn, "SET NAMES utf8");
		}
		else if (!strncmp(g_conf.db_type, oracle, strlen(oracle)))
		{
			//later add
		}
	}
	return conn;
}

void sql_db_free()
{
	ConnectionPool_free(&g_myjobdb_pool);
	g_myjobdb_pool = NULL;
	URL_free(&g_myjobdb_url);
	g_myjobdb_url = NULL;
	ConnectionPool_free(&g_projectdb_pool);
	g_projectdb_pool = NULL;
	URL_free(&g_taskdb_url);
	g_taskdb_url = NULL;
	ConnectionPool_free(&g_taskdb_pool);
	g_taskdb_pool = NULL;
	URL_free(&g_taskdb_url);
	g_taskdb_url = NULL;
}

void sql_db_format_time(char *cur_time)
{
	char tmp_time[100];
	struct tm *p = NULL;
	time_t time_val;
	time(&time_val);
	p = localtime(&time_val);
	strftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d %H:%M:%S", p);
	snprintf(cur_time, 100, "%s", tmp_time);
}

