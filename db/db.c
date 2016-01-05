#include <zdb.h>

#include "db.h"
#include "sql_db.h"
#include "log.h"
#include "common.h"

void init_db()
{
	int ret = 0;
	ret = sql_db_init();
	if(ret)
	{
		LOG_TRACE("Database Init Succed\n");
	}else
	{
		LOG_ERROR("Database Init Fail\n");
	}
}
