/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lua5.3/lua.h"
#include "lua5.3/lualib.h"
#include "lua5.3/lauxlib.h"

#include "util.h"
#include "tim.h"
#include "tim-lua.h"

static int
tim_lua_get_path(lua_State *L)
{
	const char *fname = luaL_checkstring(L, 1);

	lua_pushstring(L, get_path(fname));

	return 1;
}

char *
tim_lua_run(char *script, char *args)
{
	char *ret = NULL;
	lua_State *L;

	/* init script */
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, script))
		die("tim_lua: couldn't load file: %s\n", lua_tostring(L, -1));

	/* add variables */
	lua_pushstring(L, args);
	lua_setglobal(L, "args");

	/* add functions */
	lua_pushcfunction(L, tim_lua_get_path);
	lua_setglobal(L, "get_path");

	/* run script */
	if (lua_pcall(L, 0, LUA_MULTRET, 0))
		die("tim_lua: failed to run script: %s", lua_tostring(L, -1));

	/* get the returned value */
	const char *tmp = lua_tostring(L, -1);
	if (tmp)
		ret = strdup(tmp);
	lua_pop(L, 1); /* take the returned value out of the stack */

	/* cleanup */
	lua_close(L);

	return ret;
}
