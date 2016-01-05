#ifndef __REDIS_DB_H__
#define __REDIS_DB_H__

redisContext *redis_connect(char *ip, int port);
redisReply *pop(redisContext *context, const char *q_name);

#endif
