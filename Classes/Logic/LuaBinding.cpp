/* 
 * File:   LuaBinding.cpp
 * Author: thunderliu
 * 
 * Created on 2014��3��2��, ����11:48
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
#include "CommHeader.h"
int luaL_setregistry(lua_State* L, int idx)
{
    lua_pushvalue(L, idx);
    int key = luaL_ref(L, LUA_REGISTRYINDEX);

    return key;
}

int luaL_getregistery(lua_State* L, int key)
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
    lua_pushvalue(L, self);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_pushvalue(L, argv);
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
    
    lua_pushvalue(L, class_type);
    lua_setmetatable(L, obj);
    
    lua_pushvalue(L, obj);
    lua_setfield(L, obj, "_cur");

    lua_getfield(L, obj, "ctor");
    lua_pushvalue(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_pushvalue(L, argv);
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
        lua_pushvalue(L, super);
        lua_setfield(L, t, "super");
        lua_pushvalue(L, super);
        lua_setmetatable(L, t);
    }
    
    lua_pushvalue(L, t);
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

int mrobj_getId(lua_State* L)
{
    CMultiRefObject* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getId());

    return 1;
}

luaL_Reg unit_funcs[] = {
    {"ctor", unit_ctor},
    {"setMaxHp", unit_setMaxHp},
    {"getMaxHp", unit_getMaxHp},
    {"setExMaxHp", unit_setExMaxHp},
    {"getExMaxHp", unit_getExMaxHp},
    {"getRealMaxHp", unit_getRealMaxHp},
    {"setHp", unit_setHp},
    {"getHp", unit_getHp},
    {"setForceByIndex", unit_setForceByIndex},
    {"setAlly", unit_setAlly},
    {"setBaseArmor", unit_setBaseArmor},
    {"getBaseArmor", unit_getBaseArmor},
    {"getRealArmorValue", unit_getRealArmorValue},
    {"setExArmorValue", unit_setExArmorValue},
    {"getExArmorValue", unit_getExArmorValue},
    {"suspend", unit_suspend},
    {"isSuspended", unit_isSuspended},
    {"resume", unit_resume},
    {"getBuffStackSize", unit_getBuffStackSize},
    {"attack", unit_attack},
    {"damaged", unit_damaged},
    {"attackM", unit_attackM},
    {"damagedM", unit_damagedM},
    {"attackL", unit_attackL},
    {"damagedL", unit_damagedL},
    {"addActiveAbility", unit_addActiveAbility},
    {"addPassiveAbility", unit_addPassiveAbility},
    {"addBuffAbility", unit_addBuffAbility},
    {"getAttackAbility", unit_getAttackAbility},
    {"setBaseMoveSpeed", unit2d_setBaseMoveSpeed},
    {"getRealMoveSpeed", unit2d_getRealMoveSpeed},
    {"setExMoveSpeed", unit2d_setExMoveSpeed},
    {"getExMoveSpeed", unit2d_getExMoveSpeed},
    {"setPosition", unit2d_setPosition},
    {"getPosition", unit2d_getPosition},
    {"getNearestEnemyInRange", unit2d_getNearestEnemyInRange},
    {"move", unit2d_move},
    {"moveAlongPath", unit2d_moveAlongPath},
    {"castSpell", unit2d_castSpell},
    {"stop", unit2d_stop},
    {"setHostilityRange", unit2d_setHostilityRange},
    {"getHostilityRange", unit2d_getHostilityRange},
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

int unit_getMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    
    lua_pushnumber(L, u->getMaxHp());

    return 1;
}

int unit_setExMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);
    
    u->setExMaxHp(CExtraCoeff(exA, exB));

    return 0;
}

int unit_getExMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    
    const CExtraCoeff& ex = u->getExMaxHp();
    
    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());
    
    return 2;
}

int unit_getRealMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getRealMaxHp());

    return 1;
}

int unit_setHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setHp(n);

    return 0;
}

int unit_getHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getHp());

    return 1;
}

int unit_setForceByIndex(lua_State* L)
{
    int n = lua_tonumber(L, 2);
    
    CUnit* u = luaL_tounitptr(L);
    u->setForceByIndex(n);
    
    return 0;
}

int unit_setAlly(lua_State* L)
{
    unsigned int ally = lua_tounsigned(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setAlly(ally);

    return 0;
}

int unit_setBaseArmor(lua_State* L)
{
    int armorType = lua_tointeger(L, 2);
    float armorValue = lua_tonumber(L, 3);
    
    CUnit* u = luaL_tounitptr(L);
    u->setBaseArmor(CArmorValue(armorType, armorValue));
    
    return 0;
}

int unit_getBaseArmor(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const CArmorValue& av = u->getBaseArmor();
    
    lua_pushinteger(L, av.getType());
    lua_pushnumber(L, av.getValue());
    
    return 2;
}

int unit_getRealArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getRealArmorValue());

    return 1;
}

int unit_setExArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    u->getExArmorValue().setMulriple(exA);
    u->getExArmorValue().setAddend(exB);

    return 0;
}

int unit_getExArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    const CExtraCoeff& ex = u->getExArmorValue();
    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int unit_suspend(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    u->suspend();

    return 0;
}

int unit_isSuspended(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    lua_pushboolean(L, u->isSuspended());

    return 1;
}

int unit_resume(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    u->resume();

    return 0;
}

int unit_getBuffStackSize(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const char* root = lua_tostring(L, 2);

    int count = 0;
    CUnit::MAP_BUFF_ABILITYS& mapBuffs = u->getBuffAbilitys();
    M_MAP_FOREACH(mapBuffs)
    {
        CBuffAbility* buff = M_MAP_EACH;
        if (strcmp(buff->getName(), root) == 0)
        {
            ++count;
        }
    }

    lua_pushinteger(L, count);

    return 1;
}

int unit_attack(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = NULL;
    luaL_toobjptr(L, 2, ad);
    CUnit* t = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    ad = u->attackAdv(ad, t, mask);

    luaL_pushobjptr(L, "AttackData", ad);

    return 1;
}

int unit_damaged(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = NULL;
    luaL_toobjptr(L, 2, ad);
    CUnit* s = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    u->damagedAdv(ad, s, mask);

    return 0;
}

int unit_attackM(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = NULL;
    luaL_toobjptr(L, 2, ad);
    CUnit* t = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    ad = u->attackMid(ad, t, mask);

    luaL_pushobjptr(L, "AttackData", ad);
    
    return 1;
}

int unit_damagedM(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = NULL;
    luaL_toobjptr(L, 2, ad);
    CUnit* s = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    u->damagedMid(ad, s, mask);
    
    return 0;
}

int unit_attackL(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = NULL;
    luaL_toobjptr(L, 2, ad);
    CUnit* t = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    ad = u->attackBot(ad, t, mask);

    luaL_pushobjptr(L, "AttackData", ad);
    
    return 1;
}

int unit_damagedL(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float dmg = lua_tonumber(L, 2);
    CUnit* s = luaL_tounitptr(L, 3);
    unsigned int mask = lua_gettop(L) < 4 ? 0 : lua_tounsigned(L, 4);

    u->damagedBot(dmg, s, mask);
    
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
        int src = 0;
        if (lua_istable(L, 3))
        {
            src = luaL_tounitptr(L, 3)->getId();
        }
        else
        {
            src = lua_tointeger(L, 3);
        }
        u->addBuffAbility(id, src);
    }
    
    return 0;
}

int unit_getAttackAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackAct* _p = NULL;
    u->getActiveAbility(u->getAttackAbilityId())->dcast(_p);
    if (_p == NULL)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "AttackAct", _p);
    }
    
    return 1;
}

int unit2d_setBaseMoveSpeed(lua_State* L)
{
    float n = lua_tonumber(L, 2);
    
    CUnit* u = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setBaseMoveSpeed(n);
    
    return 0;
}

int unit2d_getRealMoveSpeed(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    lua_pushnumber(L, d->getRealMoveSpeed());
    
    return 1;
}

int unit2d_setExMoveSpeed(lua_State* L)
{
    float a = lua_tonumber(L, 2);
    float b = lua_tonumber(L, 2);
    
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setExMoveSpeed(CExtraCoeff(a, b));
    
    return 0;
}

int unit2d_getExMoveSpeed(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    CExtraCoeff& ex = d->getExMoveSpeed();
    
    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());
    
    return 2;
}

int unit2d_setPosition(lua_State* L)
{
    CPoint p(lua_tonumber(L, 2), lua_tonumber(L, 3));
    
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setPosition(p);
    
    return 0;
}

int unit2d_getPosition(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    CPoint& p = d->getPosition();
    
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);
    
    return 2;
}

int unit2d_getNearestEnemyInRange( lua_State* L )
{
    CUnit* u = luaL_tounitptr(L);
    float r = lua_tonumber(L, 2);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = CUnitGroup::getNearestUnitInRange(w, d->getPosition(), r, (FUNC_UNIT_CONDITION)&CUnitGroup::isLivingEnemyOf, DCAST(u, CUnitForce*));
    lua_pop(L, 1);

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int unit2d_move(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    bool ob = lua_gettop(L) < 4 ? true : (lua_toboolean(L, 4) != 0);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdMove(CPoint(x, y), ob);

    return 0;
}

int unit2d_moveAlongPath(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnitPath* up = NULL;
    luaL_toobjptr(L, 2, up);
    bool ob = lua_gettop(L) < 3 ? true : (lua_toboolean(L, 3) != 0);
    float ba = lua_gettop(L) < 4 ? 5.0f : lua_tonumber(L, 4);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdMoveAlongPath(up, ob, ba);

    return 0;
}

int unit2d_castSpell(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = lua_tointeger(L, 2);
    bool ob = lua_gettop(L) < 3 ? true : (lua_toboolean(L, 3) != 0);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    int res = d->cmdCastSpell(id, ob);

    lua_pushinteger(L, res);

    return 1;
}

int unit2d_stop(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdStop();

    return 0;
}

int unit2d_setHostilityRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float h = lua_tonumber(L, 2);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setHostilityRange(h);
    
    return 0;
}

int unit2d_getHostilityRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    
    lua_pushnumber(L, d->getHostilityRange());
    
    return 1;
}

luaL_Reg UnitPath_funcs[] = {
    {"ctor", UnitPath_ctor},
    {"addPoint", UnitPath_addPoint},
    {"getFirstPoint", UnitPath_getFirstPoint},
    {NULL, NULL}
};

int UnitPath_ctor(lua_State* L)
{
    // TODO: get init params

    // TODO: bind class to _p
    CUnitPath* _p = new CUnitPath;
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");
    
    return 0;
}

int UnitPath_addPoint(lua_State* L)
{
    CUnitPath* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);

    _p->addPoint(CPoint(x, y));

    return 0;
}

int UnitPath_getFirstPoint(lua_State* L)
{
    CUnitPath* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    const CPoint* p = _p->getCurTargetPoint(0);
    if (p != NULL)
    {
        lua_pushnumber(L, p->x);
        lua_pushnumber(L, p->y);
    }
    else
    {
        lua_pushnil(L);
        lua_pushnil(L);
    }

    return 2;
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
    {"copy", ability_copy},
    {"setTriggerFlags", ability_setTriggerFlags},
    {"getOwner", ability_getOwner},
    {"setInterval", ability_setInterval},
    {"getInterval", ability_getInterval},
    {"setCoolDown", ability_setCoolDown},
    {"getCoolDown", ability_getCoolDown},
    {"isCoolingDown", ability_isCoolingDown},
    {"resetCD", ability_resetCD},
    {"coolDown", ability_coolDown},
    {"setLevel", ability_setLevel},
    {"getLevel", ability_getLevel},
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
    
    lua_pushvalue(L, ad);
    
    return 1;
}

int ability_onUnitAttacked(lua_State* L)
{
    int ad = 2;
    
    lua_pushvalue(L, ad);
    
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

int ability_onUnitAbilityProjectileEffect(lua_State* L)
{
    return 0;
}

int ability_copy(lua_State* L)
{
    CAbility* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    
    CAbility* _p_ = NULL;
    _p->copy()->dcast(_p_);
    
    luaL_getregistery(L, _p_->getScriptHandler());
    
    return 1;
}

int ability_setTriggerFlags(lua_State* L)
{
    unsigned int trigger = lua_tounsigned(L, 2);

    CAbility* _p = luaL_toabilityptr(L, 1);
    _p->setTriggerFlags(trigger);

    return 0;
}

int ability_getOwner(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    CUnit* u = _p->getOwner();
    
    luaL_pushobjptr(L, "Unit", u);
    
    return 1;
}

int ability_setInterval(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    float fInterval = lua_tonumber(L, 2);
    
    _p->setInterval(fInterval);
    
    return 0;
}

int ability_getInterval(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    _p->getInterval();

    return 1;
}

int ability_setCoolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    float cd = lua_tonumber(L, 2);

    _p->setCoolDown(cd);

    return 1;
}

int ability_getCoolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushnumber(L, _p->getCoolDown());

    return 1;
}

int ability_isCoolingDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushboolean(L, _p->isCoolingDown());

    return 1;
}

int ability_resetCD(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    _p->resetCD();

    return 0;
}

int ability_coolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    _p->coolDown();

    return 0;
}

int ability_setLevel(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    int lvl = lua_tointeger(L, 2);
    
    _p->setLevel(lvl);
    
    return 0;
}

int ability_getLevel(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    
    lua_pushinteger(L, _p->getLevel());
    
    return 1;
}

luaL_Reg ActiveAbility_funcs[] = {
    {"ctor", ActiveAbility_ctor},
    {"checkConditions", ActiveAbility_checkConditions},
    {"onUnitCastAbility", ActiveAbility_onUnitCastAbility},
    {NULL, NULL}
};

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

int BuffAbility_ctor(lua_State* L)
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

luaL_Reg AttackAct_funcs[] = {
    {"ctor", AttackAct_ctor},
    {"setBaseAttack", AttackAct_setBaseAttack},
    {"getBaseAttack", AttackAct_getBaseAttack},
    {"getRealAttackValue", AttackAct_getRealAttackValue},
    {"setExAttackValue", AttackAct_setExAttackValue},
    {"getExAttackValue", AttackAct_getExAttackValue},
    {"setBaseAttackInterval", AttackAct_setBaseAttackInterval},
    {"getBaseAttackInterval", AttackAct_getBaseAttackInterval},
    {"getRealAttackInterval", AttackAct_getRealAttackInterval},
    {"getRealAttackSpeed", AttackAct_getRealAttackSpeed},
    {"setExAttackSpeed", AttackAct_setExAttackSpeed},
    {"getExAttackSpeed", AttackAct_getExAttackSpeed},
    {NULL, NULL}
};

int AttackAct_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float cd = lua_tonumber(L, 3);
    int at = lua_tointeger(L, 4);
    float av = lua_tonumber(L, 5);
    float ar = lua_tonumber(L, 6);

    float minRange = lua_tonumber(L, 7);
    float maxRange = lua_tonumber(L, 8);
    bool hor = (lua_toboolean(L, 9) != 0);

    int anis = lua_gettop(L) + 1;
    assert(anis > 9);


    CAttackAct* _p = new CAttackAct("Attack", name, cd, CAttackValue(at, av), ar);
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

int AttackAct_setBaseAttack(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    int type = lua_tointeger(L, 2);
    float value = lua_tonumber(L, 3);

    _p->setBaseAttack(CAttackValue(type, value));

    return 0;
}

int AttackAct_getBaseAttack(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    const CAttackValue& av = _p->getBaseAttack();
    lua_pushinteger(L, av.getType());
    lua_pushnumber(L, av.getValue());

    return 2;
}

int AttackAct_getRealAttackValue(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    bool rnd = (lua_gettop(L) < 2) || (lua_toboolean(L, 2) != 0);

    lua_pushnumber(L, _p->getRealAttackValue(rnd));

    return 1;
}

int AttackAct_setExAttackValue(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    _p->setExAttackValue(CExtraCoeff(exA, exB));

    return 0;
}

int AttackAct_getExAttackValue(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    const CExtraCoeff& ex = _p->getExAttackValue();
    
    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int AttackAct_setBaseAttackInterval(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    float cd = lua_tonumber(L, 2);

    _p->setBaseAttackInterval(cd);

    return 0;
}

int AttackAct_getBaseAttackInterval(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getBaseAttackInterval());

    return 1;
}

int AttackAct_getRealAttackInterval(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getRealAttackInterval());

    return 1;
}

int AttackAct_getRealAttackSpeed(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getRealAttackSpeed());

    return 1;
}

int AttackAct_setExAttackSpeed(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    _p->setExAttackSpeed(CExtraCoeff(exA, exB));

    return 0;
}

int AttackAct_getExAttackSpeed(lua_State* L)
{
    CAttackAct* _p = NULL;
    luaL_toobjptr(L, 1, _p);

    const CExtraCoeff& ex = _p->getExAttackSpeed();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int AttackBuffMakerPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float chance = lua_tonumber(L, 3);
    int buff = lua_tointeger(L, 4);
    bool toself = lua_toboolean(L, 5) != 0;
    float exAttackA = lua_tonumber(L, 6);
    float exAttackB = lua_tonumber(L, 7);

    CAttackBuffMakerPas* _p = new CAttackBuffMakerPas("ABM", name, chance, buff, toself, CExtraCoeff(exAttackA, exAttackB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int AuraPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float interval = lua_tonumber(L, 3);
    int buff = lua_tointeger(L, 4);
    float range = lua_tonumber(L, 5);
    unsigned int effect = lua_tounsigned(L, 6);

    CAuraPas* _p = new CAuraPas("Aura", name, interval, buff, range, effect);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int VampirePas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float conversion = lua_tonumber(L, 3);

    CVampirePas* _p = new CVampirePas("Vampire", name, conversion);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int StunBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);
    bool stackable = lua_toboolean(L, 5) != 0;

    CStunBuff* _p = new CStunBuff(root, name, duration, stackable);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int DoubleAttackPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float chance = lua_tonumber(L, 3);
    float exAttackA = lua_tonumber(L, 4);
    float exAttackB = lua_tonumber(L, 5);

    CDoubleAttackPas* _p = new CDoubleAttackPas("DoubleAttack", name, chance, CExtraCoeff(exAttackA, exAttackB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int SpeedBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);
    bool stackable = lua_toboolean(L, 5) != 0;
    float moveA = lua_tonumber(L, 6);
    float moveB = lua_tonumber(L, 7);
    float attackA = lua_tonumber(L, 8);
    float attackB = lua_tonumber(L, 9);

    CSpeedBuff* _p = new CSpeedBuff(root, name, duration, stackable, CExtraCoeff(moveA, moveB), CExtraCoeff(attackA, attackB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int ChangeHpPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float interval = lua_tonumber(L, 3);
    float exChangeMaxHpA = lua_tonumber(L, 4);
    float exChangeMaxHpB = lua_tonumber(L, 5);
    float exMinMaxHpA = lua_tonumber(L, 6);
    float exMinMaxHpB = lua_tonumber(L, 7);

    CChangeHpPas* _p = new CChangeHpPas("ChangeHp", name, interval, CExtraCoeff(exChangeMaxHpA, exChangeMaxHpB), CExtraCoeff(exMinMaxHpA, exMinMaxHpB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int ChangeHpBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);
    bool stackable = lua_toboolean(L, 5) != 0;
    float interval = lua_tonumber(L, 6);
    float exChangeMaxHpA = lua_tonumber(L, 7);
    float exChangeMaxHpB = lua_tonumber(L, 8);
    float exMinMaxHpA = lua_tonumber(L, 9);
    float exMinMaxHpB = lua_tonumber(L, 10);

    CChangeHpBuff* _p = new CChangeHpBuff(root, name, duration, stackable, interval, CExtraCoeff(exChangeMaxHpA, exChangeMaxHpB), CExtraCoeff(exMinMaxHpA, exMinMaxHpB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int RebirthPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float cd = lua_tonumber(L, 3);
    float exMaxHpA = lua_tonumber(L, 4);
    float exMaxHpB = lua_tonumber(L, 5);

    CRebirthPas* _p = new CRebirthPas("Rebirth", name, cd, CExtraCoeff(exMaxHpA, exMaxHpB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int EvadePas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float chance = lua_tonumber(L, 3);
    int buff = lua_tointeger(L, 4);

    CEvadePas* _p = new CEvadePas("Evade", name, chance, buff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int EvadeBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);
    bool stackable = lua_toboolean(L, 5) != 0;
    float chance = lua_tonumber(L, 6);

    CEvadeBuff* _p = new CEvadeBuff(root, name, duration, stackable, chance);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int BuffMakerAct_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float cd = lua_tonumber(L, 3);
    int target = lua_tointeger(L, 4);
    unsigned int effect = lua_tounsigned(L, 5);
    float chance = lua_tonumber(L, 6);
    int buff = lua_tointeger(L, 7);

    CBuffMakerAct* _p = new CBuffMakerAct("BM", name, cd, (CCommandTarget::TARGET_TYPE)target, effect, chance, buff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int DamageBuffMakerBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    int damageType = lua_tointeger(L, 3);
    float damageValue = lua_tonumber(L, 4);
    float chance = lua_tonumber(L, 5);
    int buff = lua_tointeger(L, 6);
    bool toself = lua_tounsigned(L, 7) != 0;
    float exAvA = lua_tonumber(L, 8);
    float exAvB = lua_tonumber(L, 9);

    CDamageBuffMakerBuff* _p = new CDamageBuffMakerBuff(name, CAttackValue(damageType, damageValue), chance, buff, toself, CExtraCoeff(exAvA, exAvB));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

luaL_Reg AttackData_funcs[] = {
    {"ctor", AttackData_ctor},
    {"setAttack", AttackData_setAttack},
    {"getAttack", AttackData_getAttack},
    {"setAttackType", AttackData_setAttackType},
    {"getAttackType", AttackData_getAttackType},
    {"setAttackValue", AttackData_setAttackValue},
    {"getAttackValue", AttackData_getAttackValue},
    {"addAttackBuff", AttackData_addAttackBuff},
    {NULL, NULL}
};

int AttackData_ctor(lua_State* L)
{
    // TODO: get init params

    // TODO: bind class to _p
    CAttackData* _p = new CAttackData;
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int AttackData_setAttack(lua_State* L)
{
    int type = lua_tointeger(L, 2);
    float value = lua_tonumber(L, 3);
    
    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->setAttackValue(type, value);
    
    return 0;
}

int AttackData_getAttack(lua_State* L)
{
    CAttackData* _p = NULL;
    const CAttackValue& av = luaL_toobjptr(L, 1, _p)->getAttackValue();
    
    lua_pushinteger(L, av.getType());
    lua_pushinteger(L, av.getValue());
    
    return 2;
}

int AttackData_setAttackType(lua_State* L)
{
    int type = lua_tointeger(L, 2);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setType(type);

    return 0;
}

int AttackData_getAttackType(lua_State* L)
{
    CAttackData* _p = NULL;
    int type = luaL_toobjptr(L, 1, _p)->getAttackValue().getType();

    lua_pushnumber(L, type);

    return 1;
}

int AttackData_setAttackValue(lua_State* L)
{
    float value = lua_tonumber(L, 2);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setValue(value);

    return 0;
}

int AttackData_getAttackValue(lua_State* L)
{
    CAttackData* _p = NULL;
    float value = luaL_toobjptr(L, 1, _p)->getAttackValue().getValue();

    lua_pushnumber(L, value);

    return 1;
}

int AttackData_addAttackBuff(lua_State* L)
{
    int buff = lua_tointeger(L, 2);
    int level = lua_tointeger(L, 3);

    CAttackData* _p = NULL;
    luaL_toobjptr(L, 1, _p)->addAttackBuff(CAttackBuff(buff, level));

    return 0;
}

int g_onWorldInit( lua_State* L )
{
    return 0;
}

int g_onWorldTick( lua_State* L )
{
    return 0;
}

int g_addTemplateAbility(lua_State* L)
{
    CAbility* a = luaL_toabilityptr(L, 1);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    int id = w->addTemplateAbility(a);
    lua_pop(L, 1);

    lua_pushinteger(L, id);

    return 1;
}

int g_setControlUnit(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L, 1);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    w->setControlUnit(u->getId());
    lua_pop(L, 1);

    return 0;
}

int g_getControlUnit(lua_State* L)
{
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = w->getUnit(w->getControlUnit());
    lua_pop(L, 1);
    
    if (_p != NULL)
    {
        luaL_pushobjptr(L, "Unit", _p);
    }
    else
    {
        lua_pushnil(L);
    }
    
    return 1;
}

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars
    lua_pushlightuserdata(L, pWorld);
    lua_setglobal(L, "_world");

    // TODO: reg global funcs
    lua_register(L, "onWorldInit", g_onWorldInit);
    lua_register(L, "onWorldTick", g_onWorldTick);
    lua_register(L, "addTemplateAbility", g_addTemplateAbility);
    lua_register(L, "setControlUnit", g_setControlUnit);
    lua_register(L, "getControlUnit", g_getControlUnit);
    
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
    lua_call(L, 0, 1);
    int UnitPath = lua_gettop(L);
    luaL_setfuncs(L, UnitPath_funcs, 0);
    lua_setglobal(L, "UnitPath");

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
    
    lua_getglobal(L, "class");
    lua_getglobal(L, "Ability");
    lua_call(L, 1, 1);  // ret a class
    luaL_setfuncs(L, ActiveAbility_funcs, 0);
    lua_setglobal(L, "ActiveAbility");

    M_LUA_BIND_CLASS_EX(L, PassiveAbility, Ability);
    M_LUA_BIND_CLASS_EX(L, BuffAbility, Ability);
    
    lua_getglobal(L, "class");
    lua_getglobal(L, "ActiveAbility");
    lua_call(L, 1, 1);  // ret a class
    luaL_setfuncs(L, AttackAct_funcs, 0);
    lua_setglobal(L, "AttackAct");

    M_LUA_BIND_CLASS_EX(L, AttackBuffMakerPas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, AuraPas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, VampirePas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, StunBuff, BuffAbility);
    M_LUA_BIND_CLASS_EX(L, DoubleAttackPas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, SpeedBuff, BuffAbility);
    M_LUA_BIND_CLASS_EX(L, ChangeHpPas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, ChangeHpBuff, BuffAbility);
    M_LUA_BIND_CLASS_EX(L, RebirthPas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, EvadePas, PassiveAbility);
    M_LUA_BIND_CLASS_EX(L, EvadeBuff, BuffAbility);
    M_LUA_BIND_CLASS_EX(L, DamageBuffMakerBuff, BuffAbility);

    lua_getglobal(L, "class");
    lua_call(L, 0, 1);
    int AttackData = lua_gettop(L);
    luaL_setfuncs(L, AttackData_funcs, 0);
    lua_setglobal(L, "AttackData");
    
    return 0;
}
