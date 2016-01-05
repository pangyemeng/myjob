/**
 *	author:pangyemeng
 *	date:2015-05-16
 *	function: main
 **/

#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <signal.h>

#include "debug.h"
#include "log.h"
#include "threadpool.h"
#include "db.h"
#include "argument_parser.h"
#include "common.h"
#include "rule.h"
#include "job_parser.h"
#include "task_create.h"

configure_file g_conf;

#define THREAD_MAX_NUM 20
const char *configure_file_path = "./config/myjob.json";

void start_parser()
{
	PRINT_INFO("Start Produce Data ......\n");
	int i, err;
	pthread_t child[THREAD_MAX_NUM];
	for (i = 0; i < g_conf.thread_max; i++)
	{
		PRINT_DEBUG("Creating thread %d\n", i);
		err = pthread_create(&child[i], NULL, job_parser, (void *) i);
		if (err)
		{
			LOG_ERROR("Can't create thread %d\n", i);
			exit(0);
		}

	}
	for (i = 0; i < g_conf.thread_max; i++)
	{
		pthread_detach(child[i]);
	}
}

void print_configure_file()
{
	PRINT_INFO("-----------------configure file info-------------------------\n");
	PRINT_INFO("ip = %s\n", g_conf.ip);
	PRINT_INFO("db_type = %s\n", g_conf.db_type);
	PRINT_INFO("db_name = %s\n", g_conf.db_name);
	PRINT_INFO("db_user = %s\n", g_conf.db_user);
	PRINT_INFO("db_pwd = %s\n", g_conf.db_pwd);
	PRINT_INFO("nonsql_port = %d\n", g_conf.nonsql_port);
	PRINT_INFO("sql_port = %d\n", g_conf.sql_port);
	PRINT_INFO("queue_max = %d\n", g_conf.queue_max);
	PRINT_INFO("thread_max = %d\n", g_conf.thread_max);
	PRINT_INFO("project_name = %s\n", g_conf.project_name);
	PRINT_INFO("keywords = %s\n", g_conf.keywords);
	PRINT_INFO("locate = %s\n", g_conf.locate);
	PRINT_INFO("--------------------------------------------------------------\n");
}

void finish(int) {
	PRINT_INFO("Finish ! \n");
}


int init_linux()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, finish);
	signal(SIGHUP, finish);
	signal(SIGINT, finish);
	signal(SIGTERM, finish);
	return 1;
}

void init()
{
	pthread_t create_project_id;
	int ret;

	/* catch signal */
	init_linux();

	/* log system init */
	log_init(LL_TRACE, "log", "./");
	/* read configure */
	argument_parser(configure_file_path);
	print_configure_file();
	/* init database: libzdb and hredis */
	init_db();
	/* create project */
	ret = pthread_create(&create_project_id, NULL, project_create, NULL);
	if (ret)
	{
		LOG_ERROR("Can't create thread\n");
		PRINT_ERROR("Can't create thread\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{

	/* threadpool init*/
	init();

	/* main thread */
	start_parser();

	return 0;
}
