#include "CommHeader.h"

#include "LuaBindingForCC.h"
#include "LuaBinding.h"
#include "DrawForCC.h"
#include "GameControl.h"
#include "AbilityForCC.h"


luaL_Reg unit4cc_funcs[] = {
    {"ctor", unit4cc_ctor},
    {NULL, NULL}
};

int unit4cc_ctor(lua_State* L)
{
    int sprite = 2;
    const char* name = lua_tostring(L, 3);

    CUnitDrawForCC* d = getSpritePtr(L, sprite);
    CUnit* u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(name);

    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addUnit(u);
    u->setAI(CDefaultAI());

    return 0;
}

CUnitDrawForCC* getSpritePtr( lua_State* L, int idx /*= 1*/ )
{
    lua_getfield(L, idx, "_p");
    CUnitDrawForCC* ret = (CUnitDrawForCC*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}

luaL_Reg sprite4cc_funcs[] = {
    {"ctor", sprite4cc_ctor},
    {"prepareFrame", sprite4cc_prepareFrame},
    {"prepareAnimation", sprite4cc_prepareAnimation},
    {"setGeometry", sprite4cc_setGeometry},
    {NULL, NULL}
};

int sprite4cc_ctor( lua_State* L )
{
    const char* name = lua_tostring(L, 2);

    CUnitDrawForCC* d = new CUnitDrawForCC(name);
    lua_pushlightuserdata(L, d);
    lua_setfield(L, 1, "_p");

    return 0;
}

int sprite4cc_prepareFrame( lua_State* L )
{
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);
    
    CUnitDrawForCC* d = getSpritePtr(L);
    d->prepareFrame((CUnitDraw2D::FRM_ID)id, name);

    return 0;
}

int sprite4cc_prepareAnimation( lua_State* L )
{
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);
    int notify = lua_tointeger(L, 4);

    CUnitDrawForCC* d = getSpritePtr(L);
    d->prepareAnimation((CUnitDraw2D::ANI_ID)id, name, notify);

    return 0;
}

int sprite4cc_setGeometry( lua_State* L )
{
    float fHalfOfWidth = lua_tonumber(L, 2);
    float fHalfOfHeight = lua_tonumber(L, 3);
    CCPoint anchorPoint(lua_tonumber(L, 4), lua_tonumber(L, 5));
    CPoint firePoint(lua_tonumber(L, 6), lua_tonumber(L, 7));

    CUnitDrawForCC* d = getSpritePtr(L);
    d->setGeometry(fHalfOfWidth, fHalfOfHeight, anchorPoint, firePoint);

    return 0;
}

int g_loadTexture( lua_State* L )
{
    const char* name = lua_tostring(L, 1);

    M_DEF_GC(gc);
    gc->loadTexture(name);

    return 0;
}

int g_loadAnimation( lua_State* L )
{
    const char* path = lua_tostring(L, 1);
    const char* name = lua_tostring(L, 2);
    float delay = lua_tonumber(L, 3);

    M_DEF_GC(gc);
    gc->loadAnimation(path, name, delay);

    return 0;
}

int luaRegWorldFuncsForCC( lua_State* L, CWorld* pWorld )
{
    // TODO: reg global vars

    // TODO: reg global funcs
    lua_pushcfunction(L, g_loadTexture);
    lua_setglobal(L, "loadTexture");
    lua_pushcfunction(L, g_loadAnimation);
    lua_setglobal(L, "loadAnimation");

    // TODO: reg global class members
    lua_getglobal(L, "Unit");
    luaL_setfuncs(L, unit4cc_funcs, 0);
    lua_pop(L, 1);

    lua_getglobal(L, "class");
    lua_call(L, 0, 1);  // ret a class
    int sprite4cc = lua_gettop(L);
    luaL_setfuncs(L, sprite4cc_funcs, 0);
    lua_pushinteger(L, CUnitDraw::kAniMove);
    lua_setfield(L, sprite4cc, "kAniMove");
    lua_pushinteger(L, CUnitDraw::kAniDie);
    lua_setfield(L, sprite4cc, "kAniDie");
    lua_pushinteger(L, CUnitDraw::kAniAct1);
    lua_setfield(L, sprite4cc, "kAniAct1");
    lua_pushinteger(L, CUnitDraw::kAniAct2);
    lua_setfield(L, sprite4cc, "kAniAct2");
    lua_pushinteger(L, CUnitDraw::kAniAct3);
    lua_setfield(L, sprite4cc, "kAniAct3");
    lua_pushinteger(L, CUnitDraw::kAniAct4);
    lua_setfield(L, sprite4cc, "kAniAct4");
    lua_pushinteger(L, CUnitDraw::kAniAct5);
    lua_setfield(L, sprite4cc, "kAniAct5");
    lua_pushinteger(L, CUnitDraw::kAniAct6);
    lua_setfield(L, sprite4cc, "kAniAct6");
    lua_pushinteger(L, CUnitDraw::kAniAct7);
    lua_setfield(L, sprite4cc, "kAniAct7");
    lua_pushinteger(L, CUnitDraw::kAniAct8);
    lua_setfield(L, sprite4cc, "kAniAct8");
    lua_pushinteger(L, CUnitDraw::kFrmDefault);
    lua_setfield(L, sprite4cc, "kFrmDefault");
    lua_setglobal(L, "Sprite");

    int t = 0;

    // bind a class
    M_LUA_BIND_CLASS_EX(L, StatusShowPas, PassiveAbility);

    return 0;
}

int StatusShowPas_ctor( lua_State* L )
{
    CStatusShowPas* _p = new CStatusShowPas();
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}
