#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#include "misc.h"
#include "lua.hpp"

void usage(int argc, char**argv){
    printf("Usage: %s <type> ...\n", argv[0]);
    printf("  stack #dump stack\n");
    printf("  conf <fname> #load fname,print\n");

    return;
}

int main(int argc, char**argv){
    char *test_type;
    if(argc<2){
	WARNING("argc<2");
	usage(argc, argv);
	return 1;
    }

    test_type = argv[1];

    TEST_PROC_TYPE(stack);
    TEST_PROC_TYPE(conf);

    return 0;
}

static void stack_dump(lua_State *L);
static int getcolorfield(lua_State* L, const char *key){
    int result=0;

    lua_pushstring(L,key);
    NOTICE("key:%s BEFORE: top=%d, type=%s",key,lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    lua_gettable(L,-2);
    NOTICE("key:%s AFTER: top=%d, type=%s",key,lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    if(!lua_isnumber(L,-1)){
	WARNING("key:%s is not number. type:%s",key, lua_typename(L, lua_type(L,-1)));
    }else{
	result = (int)lua_tonumber(L, -1);
    }
    lua_pop(L,1);
    return result;
}
static void setcolorfield(lua_State* L, const char *index, int value){
    lua_pushnumber(L, double(value));
    lua_setfield(L, -2, index);
}
int test_conf(int argc, char**argv){
    if(argc<3){
	WARNING("fname NotExist. argc=%d",argc);
	return 1;
    }
    lua_State *L = luaL_newstate(); //open lua, If fail, throw excetion
    luaL_openlibs(L); //open standard libs.If fail, throw exception

    lua_newtable(L);
    setcolorfield(L,"r", 255);
    setcolorfield(L,"g", 255);
    setcolorfield(L,"b", 255);
    lua_setglobal(L,"white");

    lua_newtable(L);
    setcolorfield(L,"r", 255);
    setcolorfield(L,"g", 0);
    setcolorfield(L,"b", 0);
    lua_setglobal(L,"red");

    int width=0,height=0;
    char *fname = argv[2];
    if(luaL_loadfile(L,fname) || lua_pcall(L,0,0,0)){
	WARNING("fail to run config. file:%s. msg:%s",fname, lua_tostring(L,-1));
	lua_close(L);
	return 1;
    }
    lua_getglobal(L,"width");
    lua_getglobal(L,"height");
    width = lua_tointeger(L,-2);
    height = lua_tointeger(L,-1);
    NOTICE("width=%d, height=%d", width,height);
    lua_pop(L,2);

    NOTICE("BEFORE: top=%d, type=%s",lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    lua_getglobal(L, "background");
    NOTICE("AFTER: top=%d, type=%s",lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    if(!lua_istable(L,-1)){
	WARNING("'background' is not a table. type=%s",lua_typename(L,lua_type(L,-1)));
    }else{
	int r=0, g=-1, b=0;
	r = getcolorfield(L, "r");
	g = getcolorfield(L, "g");
	b = getcolorfield(L, "b");
	NOTICE("bg:{r:%d, g:%d, b:%d}", r, g ,b);
    }
    //lua_pop(L,1);

    const char *f="f";
    int x=11, y=13, z=0;
    lua_getglobal(L, f); //func(defined in lua script) to be called
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);

    stack_dump(L);
    NOTICE("BEFORE: top=%d, type=%s",lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    int ret = lua_pcall(L, 2, 1, 0);
    stack_dump(L);
    NOTICE("after: top=%d, type=%s",lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    if(ret != LUA_OK){
	WARNING("lua_pcall(L,2,1,0)failed. func=%s(%d,%d) ret=%d", f, x, y, ret);
    }else{
	z = lua_tonumber(L, -1);
	NOTICE("%s(%d,%d)=%d", f, x, y, z);
	lua_pop(L,1);
	NOTICE("BEFORE: top=%d, type=%s",lua_gettop(L),lua_typename(L, lua_type(L,-1)));
    }

    lua_close(L);

    return 0;
}
static void stack_dump(lua_State *L){
    int i;
    int top = lua_gettop(L); 
    printf("stack: ");
    for(i=1; i<=top; i++){
	int t=lua_type(L,i);
	switch(t){
	    case LUA_TSTRING:
		printf("'%s'",lua_tostring(L,i));
		break;
	    case LUA_TBOOLEAN:
		printf(lua_toboolean(L,i)?"true":"false");
		break;
	    case LUA_TNUMBER:
		printf("%g", lua_tonumber(L,i));
		break;
	    default:
		printf("%s", lua_typename(L,t));
		break;
	}
	printf(" ");
    }
    printf("\n");
    return;
}
int test_stack(int, char**){
    lua_State *L = luaL_newstate(); //open lua, If fail, throw excetion
    luaL_openlibs(L); //open standard libs.If fail, throw exception

    lua_pushboolean(L,1);
    lua_pushnumber(L,10);
    lua_pushnil(L);
    lua_pushstring(L,"hello");
    lua_pushlstring(L,"hello",3);

    stack_dump(L);

    lua_close(L);
    return 0;
}

