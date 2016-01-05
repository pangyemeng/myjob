#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <msgpack.h>

#include "redis_db.h"

void doTest()
{
	const char * get_job_result = "LPOP job";
	const char * get_job_result_len = "strlen job";
	redisReply *reply;
	int len = 0;
	char *buffer = NULL;
	int j;

	redisContext* c = redisConnect("127.0.0.1", 6379);
	if (c->err)
	{
		redisFree(c);
		printf("Connect to redisServer faile\n");
		return;
	}
	printf("Connect to redisServer Success\n");

#if 1
	reply = (redisReply*) redisCommand(c, get_job_result);
	printf("%d\n", reply->type);
	if (reply->type != REDIS_REPLY_STRING)
	{
		printf("Failed to execute command[%s]\n", get_job_result);
		freeReplyObject(reply);
		redisFree(c);
		return;
	}
	printf("job = %s\n", reply->str);
#ifdef	MSGPACK
	/* msgpack::sbuffer is a simple buffer implementation. */
	msgpack_sbuffer sbuf;
	msgpack_sbuffer_init(&sbuf);

	/* serialize values into the buffer using msgpack_sbuffer_write callback function. */
	msgpack_packer pk;
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

	msgpack_pack_str_body(&pk, reply->str, reply->len);

	/* deserialize the buffer into msgpack_object instance. */
	/* deserialized object is valid during the msgpack_zone instance alive. */
	msgpack_zone mempool;
	msgpack_zone_init(&mempool, reply->len + 1);

	msgpack_object deserialized;
	msgpack_unpack(sbuf.data, sbuf.size, NULL, &mempool, &deserialized);

	/* print the deserialized object. */
	msgpack_object_print(stdout, deserialized);
	puts("");
	buffer = (char *)malloc(sizeof(char) * (deserialized.via.str.size + 1));
	if(NULL == buffer)
	{
		printf("malloc error \n");
		return;
	}
	memcpy(buffer, deserialized.via.str.ptr + 3, deserialized.via.str.size);
	printf("job = %s\n", buffer);
	msgpack_zone_destroy(&mempool);
	msgpack_sbuffer_destroy(&sbuf);
#endif
	freeReplyObject(reply);
#endif
	redisFree(c);

}

int main()
{
	while (1)
	{
		sleep(2);
		doTest();
	}
	return 0;
}
