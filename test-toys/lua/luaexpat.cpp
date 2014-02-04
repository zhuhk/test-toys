#include <stdlib.h>
#include "expat.h"
#include "lua.hpp"
#include "misc.h"

typedef struct lxp_userdata{
    XML_Parser parser;
    lua_State *L;
} lxp_userdata;

static void f_StartElement(void *ud, const char *name, const char **atts);
static void f_CharData(void *ud, const char *s, int len);
static void f_EndElement(void *ud, const char *name);

static int lxp_make_parser(lua_State *L){
    lxp_userdata *xpu = (lxp_userdata *)lua_newuserdata(L,sizeof(lxp_userdata));
    
    //init the user data
    xpu->parser = NULL;

    luaL_setmetatable(L,"luaexpat");

    xpu->parser = XML_ParserCreate(NULL);
    if(xpu->parser==NULL){
	WARNING("XML_ParserCreate(NULL) failed");
	return 0;
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    lua_pushvalue(L, 1);
    lua_setuservalue(L, -2);

    XML_SetUserData(xpu->parser, xpu);
    XML_SetElementHandler(xpu->parser, f_StartElement, f_EndElement);
    XML_SetCharacterDataHandler(xpu->parser, f_CharData);
    return 1;
}

static int lxp_parse(lua_State *L){
    int status;
    size_t len;
    const char *s;
    lxp_userdata *xpu;

    xpu = (lxp_userdata*)luaL_checkudata(L, 1, "luaexpat");;
    luaL_argcheck(L, xpu->parser!=NULL, 1, "parser is closed");

    //xml data streaming
    s = luaL_optlstring(L, 2, NULL, &len);

    lua_settop(L,2);
    lua_getuservalue(L,1);
    xpu->L = L;

    status = XML_Parse(xpu->parser, s, (int)len, s==NULL);
    //NOTICE("status=%d, len=%lu, s=%s", status,len, s);
    if(XML_STATUS_ERROR == status){
	WARNING("status=%d, len=%lu, msg:%s s=%s", status,len, XML_ErrorString(XML_GetErrorCode(xpu->parser)),s);
    }

    lua_pushboolean(L, status);
    return 1;
}

static void f_CharData(void *ud, const char*s, int len){
    lxp_userdata *xpu = (lxp_userdata *)ud;
    lua_State *L = xpu->L;

    lua_getfield(L, 3, "CharacterData");
    if(lua_isnil(L, -1)){
	lua_pop(L,1);
	return;
    }
    lua_pushvalue(L, 1);
    lua_pushlstring(L, s, len);
    int ret = lua_pcall(L, 2, 0, 0);
    if(ret != LUA_OK){
	WARNING("lua_pcall(L,2,0,0)failed.ret=%d. msg:%s", ret, lua_tostring(L, -1));
    }
}

static void f_EndElement(void *ud, const char*name){
    lxp_userdata *xpu = (lxp_userdata *)ud;

    lua_State *L = xpu->L;

    lua_getfield(L, 3, "EndElement");
    if( lua_isnil(L, -1)){
	lua_pop(L, 1);
	return;
    }
    lua_pushvalue(L, 1);
    lua_pushstring(L, name);
    int ret = lua_pcall(L, 2, 0, 0);
    if(ret != LUA_OK){
	WARNING("lua_pcall(L,2,0,0)failed.ret=%d. msg:%s", ret, lua_tostring(L, -1));
    }
}

static void f_StartElement(void *ud, const char*name, const char **atts){
    lxp_userdata *xpu = (lxp_userdata *)ud;

    lua_State *L = xpu->L;

    lua_getfield(L, 3, "StartElement");
    if( lua_isnil(L, -1)){
	lua_pop(L, 1);
	return;
    }
    lua_pushvalue(L, 1);
    lua_pushstring(L, name);

    lua_newtable(L);
    for(; *atts; atts+=2){
	lua_pushstring(L, *(atts+1));
	lua_setfield(L, -2, *atts); //table[*atts] = *(atts+1)
    }

    int ret = lua_pcall(L, 3, 0, 0);
    if(ret != LUA_OK){
	WARNING("lua_pcall(L,3,0,0)failed.ret=%d. msg:%s", ret, lua_tostring(L,-1));
    }
}

static int lxp_close(lua_State *L){
    lxp_userdata *xpu = (lxp_userdata *)luaL_checkudata(L, 1, "luaexpat");
 
    if(xpu && xpu->parser){
	XML_ParserFree(xpu->parser);
    }
    xpu->parser = NULL;
    return 0;
}

static const struct luaL_Reg lxp_methods[] = {
    {"parse", lxp_parse},
    {"close", lxp_close},
    {"__gc", lxp_close},
    {NULL, NULL}
};

static const struct luaL_Reg lxp_funcs[] = {
    {"new", lxp_make_parser},
    {"parse1", lxp_parse},
    {"close1", lxp_close},
    {NULL, NULL}
};

extern "C" int luaopen_luaexpat(lua_State *L){
    luaL_newmetatable(L, "luaexpat");

    //metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    luaL_setfuncs(L, lxp_methods, 0);
    luaL_newlib(L, lxp_funcs);
    return 1;
}

