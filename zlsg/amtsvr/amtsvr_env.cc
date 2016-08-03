#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_env.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <stdlib.h>
#include <assert.h>

struct amtsvr_env {
	int lock;
	lua_State *L;
};

static struct amtsvr_env *E = NULL;

#define LOCK(q) while (__sync_lock_test_and_set(&(q)->lock,1)) {}
#define UNLOCK(q) __sync_lock_release(&(q)->lock);

const char * 
amtsvr_getenv(const char *key) {
	LOCK(E)

	lua_State *L = E->L;
	
	lua_getglobal(L, key);
	const char * result = lua_tostring(L, -1);
	lua_pop(L, 1);

	UNLOCK(E)

	return result;
}

void 
amtsvr_setenv(const char *key, const char *value) {
	LOCK(E)
	
	lua_State *L = E->L;
	lua_getglobal(L, key);
	assert(lua_isnil(L, -1));
	lua_pop(L,1);
	lua_pushstring(L,value);
	lua_setglobal(L,key);

	UNLOCK(E)
}

void amtsvr_env_init() {
	E = (amtsvr_env*)malloc(sizeof(*E));
	E->lock = 0;
	E->L = luaL_newstate();
}

void amtsvr_env_release() {
    if (E != NULL) {
        free(E);
        E = NULL;
    }
}