/*
	Database operator(DML and so on) API:
*/
#ifndef _SQL_DB_H_
#define _SQL_DB_H_

typedef enum
{
	MYJOB_DB = 1,
	PROJECT_DB,
	TASK_DB
}database;
//#define MYSQL 1
#ifdef __cplusplus
extern "C" {
#endif
	int sql_db_init();
	Connection_T sql_db_connect(int db);
	void sql_db_free();
	void sql_db_format_time(char *cur_time);
#ifdef __cplusplus
}
#endif
#endif /* _SPIMS_DB_H_ */
