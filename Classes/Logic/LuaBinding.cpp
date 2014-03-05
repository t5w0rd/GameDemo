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
#include "MultiRefObject.h"


// common
void luaL_insertloader(lua_State *L, lua_CFunction loader)
{
    if (!loader) return;
    
    // stack content after the invoking of the function
    // get loader table
    lua_getglobal(L, "package");                                  /* L: package */
    lua_getfield(L, -1, "searchers");                               /* L: package, loaders */
    
    // insert loader into index 2
    lua_pushcfunction(L, loader);                                   /* L: package, loaders, func */
    int n = lua_rawlen(L, -2) + 1;
    for (int i = n; i > 2; --i)
    {
        lua_rawgeti(L, -2, i - 1);                                /* L: package, loaders, func, function */
        // we call lua_rawgeti, so the loader table now is at -3
        lua_rawseti(L, -3, i);                                    /* L: package, loaders, func */
    }
    lua_rawseti(L, -2, 2);                                        /* L: package, loaders */
    
    // set loaders into package
    lua_setfield(L, -2, "loaders");                               /* L: package */
    
    lua_pop(L, 1);
}

void luaL_addpath(lua_State *L, const char* pPath)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    const char* cur_path =  lua_tostring(L, -1);
    lua_pushfstring(L, "%s;%s/?.lua", cur_path, pPath);
    lua_setfield(L, -3, "path");
    lua_pop(L, 2);
}

int luaL_getcopy(lua_State* L, int idx)
{
    lua_pushnil(L);
    int copy = lua_gettop(L);
    lua_copy(L, idx, copy);
    
    return copy;
}

int luaL_setregistry( lua_State* L, int idx )
{
    int key = luaL_ref(L, LUA_REGISTRYINDEX);
    luaL_getcopy(L, idx);
    lua_rawseti(L, LUA_REGISTRYINDEX, key);

    return key;
}

int luaL_getregistery( lua_State* L, int key )
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, key);

    return lua_gettop(L);
}

int obj_sctor(lua_State* L)
{
    int self = 1;
    int n = lua_gettop(L) + 1;
    assert(n > self);
    
    lua_getfield(L, self, "_cur");
    lua_getfield(L, -1, "super");
    lua_setfield(L, self, "_cur");
    lua_pop(L, 1);

    lua_getfield(L, self, "_cur");
    lua_getfield(L, -1, "ctor");
    luaL_getcopy(L, self);
    for (int argv = 2; argv < n; ++argv)
    {
        luaL_getcopy(L, argv);
    }
    lua_call(L, n - self, 0);
    lua_pop(L, 1);
    
    return 0;
}

int obj_ctor(lua_State* L)
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
    
    luaL_getcopy(L, class_type);
    lua_setmetatable(L, obj);
    
    luaL_getcopy(L, obj);
    lua_setfield(L, obj, "_cur");

    lua_getfield(L, obj, "ctor");
    luaL_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        luaL_getcopy(L, argv);
    }
    lua_call(L, n - class_type, 0);

    lua_pushnil(L);
    lua_setfield(L, obj, "_cur");
    
    return 1;
}

int g_class(lua_State* L)
{
    int super = lua_gettop(L);
    
    lua_newtable(L);
    int t = lua_gettop(L);
    
    if (super)
    {
        luaL_getcopy(L, super);
        lua_setfield(L, t, "super");
        luaL_getcopy(L, super);
        lua_setmetatable(L, t);
    }
    
    luaL_getcopy(L, t);
    lua_setfield(L, t, "__index");
    
    lua_getfield(L, t, "sctor");
    if (lua_isnil(L, -1))
    {
        lua_pushcfunction(L, obj_sctor);
        lua_setfield(L, t, "sctor");
    }
    lua_pop(L, 1);

    lua_getfield(L, t, "ctor");
    if (lua_isnil(L, -1))
    {
        lua_pushcfunction(L, obj_ctor);
        lua_setfield(L, t, "ctor");
    }
    lua_pop(L, 1);
    
    lua_pushcfunction(L, class_type_new);
    lua_setfield(L, t, "new");
    
    return 1;
}

int luaRegCommFunc(lua_State* L)
{
    lua_register(L, "class", g_class);
    return 0;
}

// game

CUnit* luaL_tounitptr(lua_State* L, int idx)
{
    lua_getfield(L, idx, "_p");
    CUnit* ret = (CUnit*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);
    
    return ret;
}

CAbility* luaL_toabilityptr(lua_State* L, int idx)
{
    lua_getfield(L, idx, "_p");
    CAbility* ret = (CAbility*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}

luaL_Reg mrobj_funcs[] = {
    {"getId", mrobj_getId},
    {NULL, NULL}
};

int mrobj_getId( lua_State* L )
{
    CMultiRefObject* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getId());

    return 1;
}

luaL_Reg unit_funcs[] = {
    {"ctor", unit_ctor},
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
    
    CUnit* u = luaL_tounitptr(L);
    u->setMaxHp(n);
    
    return 0;
}

int unit_setHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);
    
    CUnit* u = luaL_tounitptr(L);
    u->setHp(n);
    
    return 0;
}

int unit_setForceByIndex(lua_State* L)
{
    int n = lua_tonumber(L, 2);
    
    CUnit* u = luaL_tounitptr(L);
    u->setForceByIndex(n);
    
    return 0;
}

int unit_setBaseArmor(lua_State* L)
{
    int armorType = lua_tointeger(L, 2);
    float armorValue = lua_tonumber(L, 3);
    
    CUnit* u = luaL_tounitptr(L);
    u->setBaseArmor(CArmorValue((CArmorValue::ARMOR_TYPE)armorType, armorValue));
    
    return 0;
}

int unit_setAlly( lua_State* L )
{
    unsigned int ally = lua_tounsigned(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setAlly(ally);

    return 0;
}

int unit_addActiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CActiveAbility* a = DCAST(luaL_toabilityptr(L, 2), CActiveAbility*);
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
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CPassiveAbility* a = DCAST(luaL_toabilityptr(L, 2), CPassiveAbility*);
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
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CBuffAbility* a = DCAST(luaL_toabilityptr(L, 2), CBuffAbility*);
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
    
    CUnit* u = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setBaseMoveSpeed(n);
    
    return 0;
}

int unit2d_setPosition(lua_State* L)
{
    CPoint p(lua_tonumber(L, 2), lua_tonumber(L, 3));
    
    CUnit* u = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setPosition(p);
    
    return 0;
}

luaL_Reg ability_funcs[] = {
    {"ctor", ability_ctor},
    {"onUnitAddAbility", ability_onUnitAddAbility},
    {"onUnitDelAbility", ability_onUnitDelAbility},
    {"onUnitAbilityReady", ability_onUnitAbilityReady},
    {"onUnitRevive", ability_onUnitRevive},
    {"onUnitDying", ability_onUnitDying},
    {"onUnitDead", ability_onUnitDead},
    {"onUnitChangeHp", ability_onUnitChangeHp},
    {"onUnitTick", ability_onUnitTick},
    {"onUnitInterval", ability_onUnitInterval},
    {"onUnitAttackTarget", ability_onUnitAttackTarget},
    {"onUnitAttacked", ability_onUnitAttacked},
    {"onUnitDamaged", ability_onUnitDamaged},
    {"onUnitDamagedDone", ability_onUnitDamagedDone},
    {"onUnitDamageTargetDone", ability_onUnitDamageTargetDone},
    {"onUnitProjectileEffect", ability_onUnitProjectileEffect},
    {"setTriggerFlags", ability_setTriggerFlags},
    {NULL, NULL}
};

int ability_ctor(lua_State* L)
{
    return 0;
}

int ability_onUnitAddAbility(lua_State* L)
{
    return 0;
}

int ability_onUnitDelAbility(lua_State* L)
{
    return 0;
}

int ability_onUnitAbilityReady(lua_State* L)
{
    return 0;
}

int ability_onUnitRevive(lua_State* L)
{
    return 0;
}

int ability_onUnitDying(lua_State* L)
{
    return 0;
}

int ability_onUnitDead(lua_State* L)
{
    return 0;
}

int ability_onUnitChangeHp(lua_State* L)
{
    return 0;
}

int ability_onUnitTick(lua_State* L)
{
    return 0;
}

int ability_onUnitInterval(lua_State* L)
{
    return 0;
}

int ability_onUnitAttackTarget(lua_State* L)
{
    int ad = 2;
    
    luaL_getcopy(L, ad);
    
    return 1;
}

int ability_onUnitAttacked(lua_State* L)
{
    int ad = 2;
    
    luaL_getcopy(L, ad);
    
    return 0;
}

int ability_onUnitDamaged(lua_State* L)
{
    return 0;
}

int ability_onUnitDamagedDone(lua_State* L)
{
    return 0;
}

int ability_onUnitDamageTargetDone(lua_State* L)
{
    return 0;
}

int ability_onUnitProjectileEffect(lua_State* L)
{
    return 0;
}

int ability_setTriggerFlags(lua_State* L)
{
    unsigned int trigger = lua_tounsigned(L, 2);

    CAbility* a = luaL_toabilityptr(L, 1);
    a->setTriggerFlags(trigger);

    return 0;
}

int ActiveAbility_ctor(lua_State* L)
{
    // TODO: get init params
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float cd = lua_tonumber(L, 4);
    int castType = lua_tointeger(L, 5);
    unsigned int effect = lua_tounsigned(L, 6);
    ;;;;;;;;;;; // and castRange
    // TODO: bind class to _p
    CActiveAbility* _p = new CActiveAbility(root, name, cd, (CCommandTarget::TARGET_TYPE)castType, effect);
    _p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int ActiveAbility_checkConditions(lua_State* L)
{
    lua_pushboolean(L, true);

    return 1;
}

int ActiveAbility_onUnitCastAbility(lua_State* L)
{
    return 0;
}

int PassiveAbility_ctor(lua_State* L)
{
    // TODO: get init params
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float cd = lua_tonumber(L, 4);

    // TODO: bind class to _p
    CPassiveAbility* _p = new CPassiveAbility(root, name, cd);
    _p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int BuffAbility_ctor( lua_State* L )
{
    // TODO: get init params
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float dur = lua_tonumber(L, 4);
    bool stackable = lua_toboolean(L, 5) != 0;

    // TODO: bind class to _p
    CBuffAbility* _p = new CBuffAbility(root, name, dur, stackable);
    _p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int AttackAct_ctor( lua_State* L )
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float cd = lua_tonumber(L, 4);
    int at = lua_tointeger(L, 5);
    float av = lua_tonumber(L, 6);
    float ar = lua_tonumber(L, 7);

    float minRange = lua_tonumber(L, 8);
    float maxRange = lua_tonumber(L, 9);
    bool hor = (lua_toboolean(L, 10) != 0);

    int anis = lua_gettop(L) + 1;
    assert(anis > 10);


    CAttackAct* _p = new CAttackAct(root, name, cd, CAttackValue((CAttackValue::ATTACK_TYPE)at, av), ar);
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    _p->setCastMinRange(minRange);
    _p->setCastRange(maxRange);
    _p->setCastHorizontal(hor);
    for (int i = 11; i < anis; ++i)
    {
        _p->addCastAnimation(lua_tointeger(L, i));
    }

    return 0;
}

luaL_Reg attackData_funcs[] = {
    {"ctor", attackData_ctor},
    {"setAttackType", attackData_setAttackType},
    {"getAttackType", attackData_getAttackType},
    {"setAttackValue", attackData_setAttackValue},
    {"getAttackValue", attackData_getAttackValue},
    {"addAttackBuff", attackData_addAttackBuff},
    {NULL, NULL}
};

int attackData_ctor( lua_State* L )
{
    // TODO: get init params

    // TODO: bind class to _p
    CAttackData* _p = new CAttackData();
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int attackData_setAttackType( lua_State* L )
{
    int type = lua_tointeger(L, 2);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setType(type);

    return 0;
}

int attackData_getAttackType( lua_State* L )
{
    CAttackData* _p = NULL;
    int type = luaL_toobjptr(L, 1, _p)->getAttackValue().getType();

    lua_pushnumber(L, type);

    return 1;
}

int attackData_setAttackValue( lua_State* L )
{
    float value = lua_tonumber(L, 2);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setValue(value);

    return 0;
}

int attackData_getAttackValue( lua_State* L )
{
    CAttackData* _p = NULL;
    float value = luaL_toobjptr(L, 1, _p)->getAttackValue().getValue();

    lua_pushnumber(L, value);

    return 1;
}

int attackData_addAttackBuff( lua_State* L )
{
    int buff = lua_tointeger(L, 2);
    int level = lua_tointeger(L, 3);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->addAttackBuff(CAttackBuff(buff, level));

    return 0;
}

int g_addUnit( lua_State* L )
{
    CUnit* u = luaL_tounitptr(L);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    w->addUnit(u);
    lua_pop(L, 1);

    return 0;
}

int g_addTemplateAbility( lua_State* L )
{
    CAbility* a = luaL_toabilityptr(L, 1);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    int id = w->addTemplateAbility(a);
    lua_pop(L, 1);

    lua_pushinteger(L, id);

    return 1;
}

int g_setControlUnit( lua_State* L )
{
    CUnit* u = luaL_tounitptr(L);

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
    luaL_setfuncs(L, mrobj_funcs, 0);
    lua_setglobal(L, "MRObj");

    lua_getglobal(L, "class");
    lua_getglobal(L, "MRObj");
    lua_call(L, 1, 1);  // ret a class
    luaL_setfuncs(L, unit_funcs, 0);
    lua_setglobal(L, "Unit");

    lua_getglobal(L, "class");
    lua_getglobal(L, "MRObj");
    lua_call(L, 1, 1);  // ret a class
    int ability = lua_gettop(L);
    luaL_setfuncs(L, ability_funcs, 0);
    lua_pushunsigned(L, CUnit::kOnReviveTrigger);
    lua_setfield(L, ability, "kOnReviveTrigger");
    lua_pushunsigned(L, CUnit::kOnDyingTrigger);
    lua_setfield(L, ability, "kOnDyingTrigger");
    lua_pushunsigned(L, CUnit::kOnDeadTrigger);
    lua_setfield(L, ability, "kOnDeadTrigger");
    lua_pushunsigned(L, CUnit::kOnChangeHpTrigger);
    lua_setfield(L, ability, "kOnChangeHpTrigger");
    lua_pushunsigned(L, CUnit::kOnTickTrigger);
    lua_setfield(L, ability, "kOnTickTrigger");
    lua_pushunsigned(L, CUnit::kOnAttackTargetTrigger);
    lua_setfield(L, ability, "kOnAttackTargetTrigger");
    lua_pushunsigned(L, CUnit::kOnAttackedTrigger);
    lua_setfield(L, ability, "kOnAttackedTrigger");
    lua_pushunsigned(L, CUnit::kOnDamagedSurfaceTrigger);
    lua_setfield(L, ability, "kOnDamagedSurfaceTrigger");
    lua_pushunsigned(L, CUnit::kOnDamagedInnerTrigger);
    lua_setfield(L, ability, "kOnDamagedInnerTrigger");
    lua_pushunsigned(L, CUnit::kOnDamagedDoneTrigger);
    lua_setfield(L, ability, "kOnDamagedDoneTrigger");
    lua_pushunsigned(L, CUnit::kOnDamageTargetDoneTrigger);
    lua_setfield(L, ability, "kOnDamageTargetDoneTrigger");
    lua_pushunsigned(L, CUnit::kOnProjectileEffectTrigger);
    lua_setfield(L, ability, "kOnProjectileEffectTrigger");
    lua_setglobal(L, "Ability");
    
    M_LUA_BIND_CLASS_EX(L, ActiveAbility, Ability);
    M_LUA_BIND_CLASS_EX(L, PassiveAbility, Ability);
    M_LUA_BIND_CLASS_EX(L, BuffAbility, Ability);
    M_LUA_BIND_CLASS_EX(L, AttackAct, ActiveAbility);

    lua_getglobal(L, "class");
    lua_call(L, 0, 1);
    int attackData = lua_gettop(L);
    luaL_setfuncs(L, attackData_funcs, 0);
    lua_setglobal(L, "AttackData");
    
    return 0;
}

