/* 
 * File:   LuaBinding.cpp
 * Author: thunderliu
 * 
 * Created on 2014年3月2日, 上午11:48
 */

#include "CommInc.h"
#include "LuaBinding.h"
#include "Unit.h"
#include "Draw.h"
#include "Ability.h"


// common
int lua_getcopy(lua_State* L, int idx)
{
    lua_pushnil(L);
    int copy = lua_gettop(L);
    lua_copy(L, idx, copy);
    
    return copy;
}

int obj_sctor(lua_State* L)
{
    int obj = 1;
    int n = lua_gettop(L) + 1;
    assert(n > obj);
    
    lua_getfield(L, obj, "super");
    int super = lua_gettop(L);
    
    lua_getfield(L, super, "ctor");
    lua_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_getcopy(L, argv);
    }
    lua_call(L, n - obj, 0);
    
    return 0;
}

int obj_ctor(lua_State* L)
{
    return 0;
}

int obj_init(lua_State* L)
{
    return 0;
}

int class_type_new(lua_State* L)
{
    int class_type = 1;
    int n = lua_gettop(L) + 1;
    assert(n >= class_type);
    
    lua_newtable(L);
    int obj = lua_gettop(L);
    
    lua_getcopy(L, class_type);
    lua_setmetatable(L, obj);
    
    lua_getfield(L, obj, "ctor");
    lua_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_getcopy(L, argv);
    }
    lua_call(L, n - class_type, 0);
    
    lua_getfield(L, obj, "init");
    lua_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_getcopy(L, argv);
    }
    lua_call(L, n - class_type, 0);
    
    return 1;
}

int g_class(lua_State* L)
{
    int super = lua_gettop(L);
    
    lua_newtable(L);
    int t = lua_gettop(L);
    
    if (super)
    {
        lua_getcopy(L, super);
        lua_setfield(L, t, "super");
        lua_getcopy(L, super);
        lua_setmetatable(L, t);
    }
    
    lua_getcopy(L, t);
    lua_setfield(L, t, "__index");
    
    lua_pushcfunction(L, obj_sctor);
    lua_setfield(L, t, "sctor");
    
    lua_pushcfunction(L, obj_ctor);
    lua_setfield(L, t, "ctor");
    
    lua_pushcfunction(L, obj_init);
    lua_setfield(L, t, "init");
    
    lua_pushcfunction(L, class_type_new);
    lua_setfield(L, t, "new");
    
    return 1;
}

int luaRegCommFunc(lua_State* L)
{
    lua_pushcfunction(L, g_class);
    lua_setglobal(L, "class");
    
    return 0;
}

// game

CUnit* getUnitPtr(lua_State* L, int idx)
{
    lua_getfield(L, idx, "_p");
    CUnit* ret = (CUnit*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);
    
    return ret;
}

CAbility* getAbilityPtr(lua_State* L, int idx)
{
    lua_getfield(L, idx, "_p");
    CAbility* ret = (CAbility*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}

luaL_Reg unit_funcs[] = {
    {"ctor", unit_ctor},
    {"init", unit_init},
    {"setMaxHp", unit_setMaxHp},
    {"setHp", unit_setHp},
    {"setForceByIndex", unit_setForceByIndex},
    {"setBaseArmor", unit_setBaseArmor},
    {"addActiveAbility", unit_addActiveAbility},
    {"addPassiveAbility", unit_addPassiveAbility},
    {"addBuffAbility", unit_addBuffAbility},
    {"setBaseMoveSpeed", uint2d_setBaseMoveSpeed},
    {"setPosition", unit2d_setPosition},
    {NULL, NULL}
};

int unit_ctor(lua_State* L)
{
    return 0;
}

int unit_init(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    
    CUnit* u = new CUnit(root);
    CUnitDraw* d = new CUnitDraw(root);
    u->setDraw(d);
    u->setName(name);
    
    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addUnit(u);
    
    return 0;
}

int unit_setMaxHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);
    
    CUnit* u = getUnitPtr(L);
    u->setMaxHp(n);
    
    return 0;
}

int unit_setHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);
    
    CUnit* u = getUnitPtr(L);
    u->setHp(n);
    
    return 0;
}

int unit_setForceByIndex(lua_State* L)
{
    int n = lua_tonumber(L, 2);
    
    CUnit* u = getUnitPtr(L);
    u->setForceByIndex(n);
    
    return 0;
}

int unit_setBaseArmor(lua_State* L)
{
    int armorType = lua_tointeger(L, 2);
    float armorValue = lua_tonumber(L, 3);
    
    CUnit* u = getUnitPtr(L);
    u->setBaseArmor(CArmorValue((CArmorValue::ARMOR_TYPE)armorType, armorValue));
    
    return 0;
}

int unit_setAlly( lua_State* L )
{
    unsigned int ally = lua_tounsigned(L, 2);

    CUnit* u = getUnitPtr(L);
    u->setAlly(ally);

    return 0;
}

int unit_addActiveAbility(lua_State* L)
{
    CUnit* u = getUnitPtr(L);
    if (lua_istable(L, 2))
    {
        CActiveAbility* a = (CActiveAbility*)getAbilityPtr(L, 2);
        u->addActiveAbility(a);
    }
    else
    {
        int id = lua_tointeger(L, 2);
        u->addActiveAbility(id);
    }
    
    return 0;
}

int unit_addPassiveAbility(lua_State* L)
{
    CUnit* u = getUnitPtr(L);
    if (lua_istable(L, 2))
    {
        CPassiveAbility* a = (CPassiveAbility*)getAbilityPtr(L, 2);
        u->addPassiveAbility(a);
    }
    else
    {
        int id = lua_tointeger(L, 2);
        u->addPassiveAbility(id);
    }
    
    return 0;
}

int unit_addBuffAbility(lua_State* L)
{
    CUnit* u = getUnitPtr(L);
    if (lua_istable(L, 2))
    {
        CBuffAbility* a = (CBuffAbility*)getAbilityPtr(L, 2);
        u->addBuffAbility(a);
    }
    else
    {
        int id = lua_tointeger(L, 2);
        int src = lua_tointeger(L, 3);
        u->addBuffAbility(id, src);
    }
    
    return 0;
}

int uint2d_setBaseMoveSpeed(lua_State* L)
{
    float n = lua_tonumber(L, 2);
    
    CUnit* u = getUnitPtr(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setBaseMoveSpeed(n);
    
    return 0;
}

int unit2d_setPosition(lua_State* L)
{
    CPoint p(lua_tonumber(L, 2), lua_tonumber(L, 3));
    
    CUnit* u = getUnitPtr(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setPosition(p);
    
    return 0;
}

int g_addUnit( lua_State* L )
{
    CUnit* u = getUnitPtr(L);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    w->addUnit(u);
    lua_pop(L, 1);

    return 0;
}

int g_setControlUnit( lua_State* L )
{
    CUnit* u = getUnitPtr(L);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    w->setControlUnit(u->getId());
    lua_pop(L, 1);

    return 0;
}

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars
    lua_pushlightuserdata(L, pWorld);
    lua_setglobal(L, "_world");

    // TODO: reg global funcs
    lua_pushcfunction(L, g_setControlUnit);
    lua_setglobal(L, "setControlUnit");
    
    // TODO: reg global classes
    lua_getglobal(L, "class");
    lua_call(L, 0, 1);  // ret a class
    luaL_setfuncs(L, unit_funcs, 0);
    lua_setglobal(L, "Unit");

    M_LUA_BIND_CLASS(L, AttackAct);
    
    return 0;
}

int AttackAct_init( lua_State* L )
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float cd = lua_tonumber(L, 4);
    int at = lua_tointeger(L, 5);
    float av = lua_tonumber(L, 6);
    float ar = lua_tonumber(L, 7);

    float minRange = lua_tonumber(L, 8);
    float maxRange = lua_tonumber(L, 9);
    bool hor = (bool)lua_toboolean(L, 10);
    
    int anis = lua_gettop(L) + 1;
    assert(anis > 10);
    

    CAttackAct* _p = new CAttackAct(root, name, cd, CAttackValue((CAttackValue::ATTACK_TYPE)at, av), ar);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    _p->setCastMinRange(-3.0f);
    _p->setCastRange(15.0f);
    _p->setCastHorizontal();
    for (int i = 11; i < anis; ++i)
    {
        _p->addCastAnimation(lua_tointeger(L, i));
    }

    return 0;
}

