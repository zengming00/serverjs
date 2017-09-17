#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "mujs.h"
#include "hiredis.h"

#include "js_redis.h"

#define TAG "Redis"

static void Redis_finalize(js_State *J, void *data)
{
	// printf("redis.finalize()\n");
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
			redisFree(c); // TODO 这行不会执行
		}
		else
		{
			js_error(J, "Connection error: can't allocate redis context\n");
		}
	}

	js_currentfunction(J);
	js_getproperty(J, -1, "prototype");
	js_newuserdata(J, TAG, c, Redis_finalize);
	// printf("redis.newRedis()\n");
}

static void Redis_prototype_cmd(js_State *J)
{
	redisContext *c = js_touserdata(J, 0, TAG); // idx[0] === this
	unsigned int i, top = js_gettop(J);
	const char *s = ; // idx[1] === arg1           idx[-1] 是栈顶(返回值) -2 之下
	redisReply *reply;
	int ap[top];
	
	for(i=2; i<top; i++){
		js_Value *v = js_tovalue(J, idx);
		switch (v->type) {
		case JS_TNUMBER:
			ap[i] = (int)v->u.number;
			break;
		case JS_TSHRSTR:
			ap[i] = (int)v->u.shrstr;
			break;
		case JS_TLITSTR:
			ap[i] = (int)v->u.litstr;
		case JS_TMEMSTR:
			ap[i] = (int)v->u.memstr->p;
			break;
		default:
			js_error(J, "unsupport type\n");
		}
	}
	
	reply= redisvCommand(c, js_tostring(J, 1), (va_list)ap);

	switch (reply->type)
	{
	case REDIS_REPLY_STATUS:
	case REDIS_REPLY_ERROR:
	case REDIS_REPLY_STRING:
		js_pushstring(J, reply->str);
		break;
	case REDIS_REPLY_INTEGER:
		js_pushnumber(J, reply->integer);
		break;
	case REDIS_REPLY_ARRAY:
		js_newarray(J);
		for (i = 0; i < reply->elements; i++)
		{
			js_pushstring(J, reply->element[i]->str);
			js_setindex(J, -2, i);
		}
		break;
	case REDIS_REPLY_NIL:
	default:
		js_pushundefined(J);
	}

	freeReplyObject(reply);
	// printf("redis.cmd()\n");
}

static void Redis_prototype_close(js_State *J)
{
	redisContext *c = js_touserdata(J, 0, TAG);
	redisFree(c);
	js_pushundefined(J);
	// printf("redis.close()\n");
}

void init_Redis(js_State *J)
{
	js_newcfunction(J, Redis_prototype_cmd, "Redis.prototype.cmd", 1);
	js_defproperty(J, -2, "cmd", JS_DONTENUM);

	js_newcfunction(J, Redis_prototype_close, "Redis.prototype.close", 0);
	js_defproperty(J, -2, "close", JS_DONTENUM);

	js_newcconstructor(J, new_Redis, new_Redis, "Redis", 1);
	js_defglobal(J, "Redis", JS_DONTENUM);
	// printf("init redis()\n");
}
