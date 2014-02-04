#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "lua.hpp"
#include "lauxlib.h"
#include "misc.h"

static int l_dir(lua_State *L){
    DIR *dir;
    struct dirent *entry;
    int i;
    const char *path = luaL_checkstring(L,1);

    dir = opendir(path);
    if(dir==NULL){
	lua_pushnil(L);
	lua_pushstring(L,strerror(errno));
	return 2;
    }

    lua_newtable(L);
    i = 1;
    while( (entry=readdir(dir))!=NULL ) {
	lua_pushnumber(L,i++);
	lua_pushstring(L, entry->d_name);
	lua_settable(L,-3);
    }
    closedir(dir);
    return 1;
}
static int dir_iter(lua_State *L) {
    DIR *d = *(DIR **)lua_touserdata(L, lua_upvalueindex(1));
    struct dirent *entry;
    if(d!=NULL && (entry=readdir(d))!=NULL){
	lua_pushstring(L, entry->d_name);
	return 1;
    }else{
	return 0;
    }
}
static int l_idir(lua_State *L){
    const char* path = luaL_checkstring(L,1);

    DIR **d = (DIR **)lua_newuserdata(L,sizeof(DIR**));

    luaL_setmetatable(L, "luadir");

    *d = opendir(path);
    if(*d == NULL) {
	WARNING("can't open %s. msg:%m", path);
    }
    lua_pushcclosure(L, dir_iter, 1);
    return 1;
}
static int dir_gc(lua_State*L){
    DIR *d = *(DIR**)lua_touserdata(L,1);
    if(d) {
	closedir(d);
    }
    //NOTICE("destructor");
    return 0;
}
static const struct luaL_Reg mylib [] = {
    {"dir", l_dir},
    {"open", l_idir},
    {NULL, NULL}
};

extern "C" 
int luaopen_luadir(lua_State *L) {
    luaL_newmetatable(L, "luadir");

    lua_pushcfunction(L, dir_gc);
    lua_setfield(L, -2, "__gc");

    luaL_newlib(L, mylib);
    return 1;
}
