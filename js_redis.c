#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mujs.h"
#include "hiredis.h"

#include "js_redis.h"

#define TAG "Redis"

static void Redis_finalize(js_State *J, void *data)
{
	printf("redis.finalize()\n");
}

static void new_Redis(js_State *J)
{
	redisContext *c;
	const char *hostname = js_tostring(J, 1);
	int port = js_touint16(J, 2);

	struct timeval timeout = {1, 500000}; // 1.5 seconds

	c = redisConnectWithTimeout(hostname, port, timeout);
	if (c == NULL || c->err)
	{
		if (c)
		{
			js_error(J, "Connection error: %s\n", c->errstr);
			redisFree(c);
		}
		else
		{
			js_error(J, "Connection error: can't allocate redis context\n");
		}
		// exit(1);
		printf("todo: exit(1)");
	}

	js_currentfunction(J);
	js_getproperty(J, -1, "prototype");
	js_newuserdata(J, TAG, c, Redis_finalize);
	printf("redis.newRedis()\n");
}

static void Redis_prototype_cmd(js_State *J)
{
	redisContext *c = js_touserdata(J, 0, TAG);
	const char *s = js_tostring(J, 1);
	redisReply *reply = redisCommand(c, s);
	js_pushstring(J, reply->str);
	freeReplyObject(reply);
	printf("redis.cmd()\n");
}

static void Redis_prototype_close(js_State *J)
{
	redisContext *c = js_touserdata(J, 0, TAG);
	redisFree(c);
	js_pushundefined(J);
	printf("redis.close()\n");
}

void init_Redis(js_State *J)
{
	js_newcfunction(J, Redis_prototype_cmd, "Redis.prototype.cmd", 1);
	js_defproperty(J, -2, "cmd", JS_DONTENUM);

	js_newcfunction(J, Redis_prototype_close, "Redis.prototype.close", 0);
	js_defproperty(J, -2, "close", JS_DONTENUM);

	js_newcconstructor(J, new_Redis, new_Redis, "Redis", 1);
	js_defglobal(J, "Redis", JS_DONTENUM);
	printf("init redis()\n");
}

