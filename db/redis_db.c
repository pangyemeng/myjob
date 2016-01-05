#include <stdio.h>
#include <hiredis.h>

#include "db.h"
#include "redis_db.h"
#include "log.h"
#include "debug.h"

redisContext *redis_connect(char *ip, int port)
{
	redisContext *context = NULL;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	context = redisConnectWithTimeout(ip, port, timeout);
	if (context == NULL || context->err)
	{
		if (context)
		{
			LOG_ERROR("Connection error: %s\n",   context->errstr);
			PRINT_ERROR("Connection error: %s\n", context->errstr);
			redisFree(context);
			return NULL;
		}
		else
		{
			LOG_ERROR("Connection error: can't allocate redis context\n");
			PRINT_ERROR("Connection error: can't allocate redis context\n");
			return NULL;
		}
	}
	return context;
}

redisReply *pop(redisContext *context, const char *q_name)
{
	redisReply *reply = NULL;
	char get_info_cmd[256];
	const char *cmd = "LPOP %s";

	snprintf(get_info_cmd, 256, cmd, q_name);
	while((reply = (redisReply*) redisCommand(context, get_info_cmd)) != NULL)
	{
		if (reply->type != REDIS_REPLY_STRING)
		{
			//LOG_ERROR("Failed to execute command[%s]\n", get_info_cmd);
			//PRINT_ERROR("Failed to execute command[%s]\n", get_info_cmd);
			freeReplyObject(reply);
			continue;
		}
		break;
	}
	return reply;
}
