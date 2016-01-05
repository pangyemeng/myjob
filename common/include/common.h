#ifndef __COMMON_H__
#define __COMMON_H__


typedef struct _configure_file
{
	char ip[32];
	char db_type[8]; /* mysql\oracle */
	char db_name[32]; /* database service name */
	char db_user[32]; /* database user */
	char db_pwd[32]; /* database user */
	int nonsql_port; /* redis 6379 */
	int sql_port;	/* mysql: 3306 */
	int thread_max; /* thread pool max */
	int queue_max;  /* queue max */
	char project_name[128]; /* project name */
	char keywords[256];    /* search keyword*/
	char locate[128];    /* location, example: 040000 is shengzhen*/
}configure_file;

extern configure_file g_conf;

#endif
