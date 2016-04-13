#define _XOPEN_SOURCE 700

#include <errno.h> /* errno */
#include <fcntl.h> /* open, fcntl, O_RDWR, O_NOCTTY, O_CLOEXEC */
#include <stdlib.h> /* grantpt, unlockpt, ptsname */
#include <string.h> /* strerror */
#include <unistd.h> /* close */

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"


static int pty_open(lua_State *L) {
	int flags = O_RDWR|O_NOCTTY; /* hard-coded for now */
#if __linux
	int fd = open("/dev/ptmx", flags|O_CLOEXEC);
#else
	int fd = posix_openpt(flags);
#endif
	if (-1 == fd) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
#if !__linux && defined FD_CLOEXEC
	(void)fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif
	lua_pushinteger(L, fd);
	return 1;
}

static int pty_grant(lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	if (0 != (grantpt(fd))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int pty_unlock(lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	if (0 != (unlockpt(fd))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int pty_ptsname(lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *name;
	if (NULL == (name = ptsname(fd))) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		lua_pushinteger(L, errno);
		return 3;
	}
	lua_pushstring(L, name);
	return 1;
}

static int pty_close(lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	close(fd);
	lua_pushboolean(L, 1);
	return 1;
}

static const luaL_Reg lib[] = {
	{ "open", pty_open },
	{ "grant", pty_grant },
	{ "unlock", pty_unlock },
	{ "ptsname", pty_ptsname },
	{ "close", pty_close },
	{ NULL, NULL }
};

int luaopen_pty(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
