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
#include "AbilityLibrary.h"
#include "Archive.h"


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
    const char* cur_path = lua_tostring(L, -1);
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

const char* luaL_toutf8string(lua_State* L, int idx, char* buf)
{
    auto gbk = lua_tostring(L, idx);
    gbk_to_utf8(gbk, buf);
    
    return buf;
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

    if (lua_pcall(L, n - self, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        return luaL_throwerror(L, true, err);
    }

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

    if (lua_pcall(L, n - class_type, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        return luaL_throwerror(L, true, err);
    }

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

int g_cast(lua_State* L)
{
    int t = 1;
    int to = 2;

    lua_pushvalue(L, to);
    lua_setmetatable(L, t);

    return 0;
}

int g_loadValue(lua_State* L)
{
    auto file = lua_tostring(L, 1);
    
    CValue* v = nullptr;
    auto res = CArchive::loadValue(file, v);
    if (res == false)
    {
        lua_pushnil(L);

        return 1;
    }

    CArchive::luaPushValue(L, v);
    delete v;

    return 1;
}

int g_saveValue(lua_State* L)
{
    auto file = lua_tostring(L, 1);
    CValue* v = CArchive::luaToValue(L, 2);

    CArchive::saveValue(file, v);
    delete v;

    return 0;
}

int g_onLoadingUserData(lua_State* L)
{
    return 0;
}

int g_addTemplateAbility(lua_State* L)
{
    int n = lua_gettop(L);

    int id = 0;
    if (n == 1)
    {
        CAbility* a = luaL_toabilityptr(L, 1);
        id = CAbilityLibrary::instance()->addTemplateAbility(a);
    }
    else
    {
        int key = lua_tointeger(L, 1);
        CAbility* a = luaL_toabilityptr(L, 2);
        id = CAbilityLibrary::instance()->addTemplateAbility(key, a);
    }

    lua_pushinteger(L, id);

    return 1;
}

// game
CUnit* luaL_tounitptr(lua_State* L, int idx)
{
    if (lua_istable(L, idx))
    {
        lua_getfield(L, idx, "_p");
        CUnit* ret = (CUnit*)lua_touserdata(L, lua_gettop(L));
        lua_pop(L, 1);

        return ret;
    }

    if (lua_isnumber(L, idx))
    {
        lua_getglobal(L, "_world");
        CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
        lua_pop(L, 1);

        return w->getUnit(lua_tointeger(L, idx));
    }

    return nullptr;
}

CAbility* luaL_toabilityptr(lua_State* L, int idx)
{
    if (lua_istable(L, idx) == false)
    {
        return nullptr;
    }

    lua_getfield(L, idx, "_p");
    CAbility* ret = (CAbility*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}

int luaL_tounitid(lua_State* L, int idx)
{
    if (lua_istable(L, idx))
    {
        CUnit* _p = luaL_tounitptr(L, idx);
        return _p->getId();
    }

    return lua_tointeger(L, idx);
}

int luaL_toabilityid(lua_State* L, int idx)
{
    if (lua_istable(L, idx))
    {
        CAbility* _p = luaL_toabilityptr(L, idx);
        return _p->getId();
    }

    return lua_tointeger(L, idx);
}

luaL_Reg MRObj_funcs[] = {
    { "getId", MRObj_getId },
    { nullptr, nullptr }
};

int MRObj_getId(lua_State* L)
{
    CMultiRefObject* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getId());

    return 1;
}

luaL_Reg LevelExp_funcs[] = {
    { "setLevel", LevelExp_setLevel },
    { "getLevel", LevelExp_getLevel },
    { "setMaxLevel", LevelExp_setMaxLevel },
    { "getMaxLevel", LevelExp_getMaxLevel },
    { "getExp", LevelExp_getExp },
    { "addExp", LevelExp_addExp },
    { "setExpRange", LevelExp_setExpRange },
    { "setLevelUpdate", LevelExp_setLevelUpdate },
    { "canIncreaseExp", LevelExp_canIncreaseExp },
    { nullptr, nullptr }
};

int LevelExp_setLevel(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int lvl = lua_tointeger(L, 2);

    _p->setLevel(lvl);

    return 0;
}

int LevelExp_getLevel(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getLevel());

    return 1;
}

int LevelExp_setMaxLevel(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int maxlvl = lua_tointeger(L, 2);

    _p->setMaxLevel(maxlvl);

    return 0;
}

int LevelExp_getMaxLevel(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getMaxLevel());

    return 1;
}

int LevelExp_addExp(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int exp = lua_tointeger(L, 2);

    _p->addExp(exp);

    return 0;
}

int LevelExp_getExp(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getExp());

    return 1;
}

int LevelExp_setExpRange(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int from = lua_tointeger(L, 2);
    int to = lua_tointeger(L, 3);

    _p->setExpRange(from, to);

    return 0;
}

int LevelExp_setLevelUpdate(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    CLevelUpdate* lu = nullptr;
    luaL_toobjptr(L, 2, lu);

    _p->setLevelUpdate(lu);
    _p->updateExpRange();

    return 0;
}

int LevelExp_canIncreaseExp(lua_State* L)
{
    CLevelExp* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushboolean(L, _p->canIncreaseExp());

    return 1;
}

luaL_Reg LevelUpdate_funcs[] = {
    { "ctor", LevelUpdate_ctor },
    { "updateExpRange", LevelUpdate_updateExpRange },
    { "onLevelChanged", LevelUpdate_onLevelChanged },
    { "calcExp", LevelUpdate_calcExp },
    { nullptr, nullptr }
};

int LevelUpdate_ctor(lua_State* L)
{
    CLevelUpdate* _p = new CLevelUpdate;
    _p->setDbgClassName("CLevelUpdate[Lua]");
    _p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int LevelUpdate_updateExpRange(lua_State* L)
{
    CLevelUpdate* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    CLevelExp* le = nullptr;
    luaL_toobjptr(L, 2, le);
    
    int lvl = le->getLevel();
    le->setExpRange(_p->calcExp(lvl - 1), _p->calcExp(lvl));

    return 0;
}

int LevelUpdate_onLevelChanged(lua_State* L)
{
    return 0;
}

int LevelUpdate_calcExp(lua_State* L)
{
    int lvl = lua_tointeger(L, 2);

    lua_pushinteger(L, lvl);

    return 1;
}

luaL_Reg AttackData_funcs[] = {
    { "ctor", AttackData_ctor },
    { "setAttack", AttackData_setAttack },
    { "getAttack", AttackData_getAttack },
    { "setAttackType", AttackData_setAttackType },
    { "getAttackType", AttackData_getAttackType },
    { "setAttackValue", AttackData_setAttackValue },
    { "getAttackValue", AttackData_getAttackValue },
    { "addAttackBuff", AttackData_addAttackBuff },
    { nullptr, nullptr }
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

    CAttackData* _p = nullptr;
    luaL_toobjptr(L, 1, _p)->setAttackValue(type, value);

    return 0;
}

int AttackData_getAttack(lua_State* L)
{
    CAttackData* _p = nullptr;
    const CAttackValue& av = luaL_toobjptr(L, 1, _p)->getAttackValue();

    lua_pushinteger(L, av.getType());
    lua_pushinteger(L, av.getValue());

    return 2;
}

int AttackData_setAttackType(lua_State* L)
{
    int type = lua_tointeger(L, 2);

    CAttackData* _p = nullptr;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setType(type);

    return 0;
}

int AttackData_getAttackType(lua_State* L)
{
    CAttackData* _p = nullptr;
    int type = luaL_toobjptr(L, 1, _p)->getAttackValue().getType();

    lua_pushnumber(L, type);

    return 1;
}

int AttackData_setAttackValue(lua_State* L)
{
    float value = lua_tonumber(L, 2);

    CAttackData* _p = nullptr;
    luaL_toobjptr(L, 1, _p)->getAttackValue().setValue(value);

    return 0;
}

int AttackData_getAttackValue(lua_State* L)
{
    CAttackData* _p = nullptr;
    float value = luaL_toobjptr(L, 1, _p)->getAttackValue().getValue();

    lua_pushnumber(L, value);

    return 1;
}

int AttackData_addAttackBuff(lua_State* L)
{
    int buff = lua_tointeger(L, 2);
    int level = lua_tointeger(L, 3);

    CAttackData* _p = nullptr;
    luaL_toobjptr(L, 1, _p)->addAttackBuff(CAttackBuff(buff, level));

    return 0;
}

luaL_Reg Unit_funcs[] = {
    { "ctor", Unit_ctor },
    { "setName", Unit_setName },
    { "getName", Unit_getName },
    { "setMaxHp", Unit_setMaxHp },
    { "getMaxHp", Unit_getMaxHp },
    { "setExMaxHp", Unit_setExMaxHp },
    { "getExMaxHp", Unit_getExMaxHp },
    { "getRealMaxHp", Unit_getRealMaxHp },
    { "setHp", Unit_setHp },
    { "getHp", Unit_getHp },
    { "isDead", Unit_isDead },
    { "setForceByIndex", Unit_setForceByIndex },
    { "setForce", Unit_setForce },
    { "getForce", Unit_getForce },
    { "setAlly", Unit_setAlly },
    { "getAlly", Unit_getAlly },
    { "isMyAlly", Unit_isMyAlly },
    { "isMyEnemy", Unit_isMyEnemy },
    { "canEffect", Unit_canEffect },
    { "setBaseArmor", Unit_setBaseArmor },
    { "getBaseArmor", Unit_getBaseArmor },
    { "getRealArmorValue", Unit_getRealArmorValue },
    { "setExArmorValue", Unit_setExArmorValue },
    { "getExArmorValue", Unit_getExArmorValue },
    { "suspend", Unit_suspend },
    { "isSuspended", Unit_isSuspended },
    { "resume", Unit_resume },
    { "getBuffStackSize", Unit_getBuffStackSize },
    { "attack", Unit_attack },
    { "damaged", Unit_damaged },
    { "attackLow", Unit_attackLow },
    { "damagedLow", Unit_damagedLow },
    { "setRewardGold", Unit_setRewardGold },
    { "getRewardGold", Unit_getRewardGold },
    { "setRewardExp", Unit_setRewardExp },
    { "getRewardExp", Unit_getRewardExp },
    { "setAI", Unit_setAI },
    { "say", Unit_say },
    { "setGhost", Unit_setGhost },
    { "setEnergy", Unit_setEnergy },
    { "getEnergy", Unit_getEnergy },
    { "setVisible", Unit_setVisible },
    { "isVisible", Unit_isVisible },
    { "addGold", Unit_addGold },
    { "getKiller", Unit_getKiller },

    { "startDoing", Unit_startDoing },
    { "endDoing", Unit_endDoing },
    { "isDoingOr", Unit_isDoingOr },
    { "isDoingAnd", Unit_isDoingAnd },
    { "isDoingNothing", Unit_isDoingNothing },

    { "addActiveAbility", Unit_addActiveAbility },
    { "addPassiveAbility", Unit_addPassiveAbility },
    { "addBuffAbility", Unit_addBuffAbility },
    { "delActiveAbility", Unit_delActiveAbility },
    { "delPassiveAbility", Unit_delPassiveAbility },
    { "delBuffAbility", Unit_delBuffAbility },
    { "getAttackAbility", Unit_getAttackAbility },
    { "getActiveAbility", Unit_getActiveAbility },
    { "getActiveAbilities", Unit_getActiveAbilities },
    { "getBuffAbility", Unit_getBuffAbility },

    { "setBaseMoveSpeed", Unit2D_setBaseMoveSpeed },
    { "getBaseMoveSpeed", Unit2D_getBaseMoveSpeed },
    { "getRealMoveSpeed", Unit2D_getRealMoveSpeed },
    { "setExMoveSpeed", Unit2D_setExMoveSpeed },
    { "getExMoveSpeed", Unit2D_getExMoveSpeed },
    { "getHalfOfWidth", Unit2D_getHalfOfWidth },
    { "getHalfOfHeight", Unit2D_getHalfOfHeight },
    { "setPosition", Unit2D_setPosition },
    { "getPosition", Unit2D_getPosition },
    { "setFlippedX", Unit2D_setFlippedX },
    { "isFlippedX", Unit2D_isFlippedX },
    { "getNearestEnemyInRange", Unit2D_getNearestEnemyInRange },
    { "getNearestUnitInRange", Unit2D_getNearestUnitInRange },
    { "move", Unit2D_move },
    { "moveAlongPath", Unit2D_moveAlongPath },
    { "setCastTarget", Unit2D_setCastTarget },
    { "getCastTarget", Unit2D_getCastTarget },
    { "castSpellWithoutTarget", Unit2D_castSpellWithoutTarget },
    { "castSpellWithTargetUnit", Unit2D_castSpellWithTargetUnit },
    { "castSpellWithTargetPoint", Unit2D_castSpellWithTargetPoint },
    { "stop", Unit2D_stop },
    { "setHostilityRange", Unit2D_setHostilityRange },
    { "getHostilityRange", Unit2D_getHostilityRange },
    { "setFixed", Unit2D_setFixed },
    { "isFixed", Unit2D_isFixed },
    { "isDoingCastingAction", Unit2D_isDoingCastingAction },
    { "getCastingActiveAbility", Unit2D_getCastingActiveAbility },
    { "getDistance", Unit2D_getDistance },
    { "getTouchDistance", Unit2D_getTouchDistance },
    { "getAttackingTarget", Unit2D_getAttackingTarget },
    { "doAnimation", Unit2D_doAnimation },
    { "stopAction", Unit2D_stopAction },

    { nullptr, nullptr }
};

int Unit_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);

    assert(false);

    CUnitDraw* d = new CUnitDraw;
    CUnit* u = new CUnit(root, d);
    u->setName(name);

    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    //lua_getglobal(L, "_world");
    //CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    //lua_pop(L, 1);

    //w->addUnit(u);

    return 0;
}

int Unit_setName(lua_State* L)
{
    auto nm = lua_tostring(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setName(nm);

    return 0;
}

int Unit_getName(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushstring(L, u->getName());

    return 1;
}

int Unit_setMaxHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setMaxHp(n);

    return 0;
}

int Unit_getMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getMaxHp());

    return 1;
}

int Unit_setExMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    u->setExMaxHp(CExtraCoeff(exA, exB));

    return 0;
}

int Unit_getExMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    const CExtraCoeff& ex = u->getExMaxHp();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int Unit_getRealMaxHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getRealMaxHp());

    return 1;
}

int Unit_setHp(lua_State* L)
{
    float n = lua_tonumber(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setHp(n);

    return 0;
}

int Unit_getHp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getHp());

    return 1;
}

int Unit_isDead(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushboolean(L, u->isDead());

    return 1;
}

int Unit_setForceByIndex(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int n = lua_tonumber(L, 2);

    u->setForceByIndex(n);

    return 0;
}

int Unit_setForce(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    unsigned int n = lua_tounsigned(L, 2);

    u->setForce(n);

    return 0;
}

int Unit_getForce(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushunsigned(L, u->getForce());

    return 1;
}

int Unit_setAlly(lua_State* L)
{
    unsigned int ally = lua_tounsigned(L, 2);

    CUnit* u = luaL_tounitptr(L);
    u->setAlly(ally);

    return 0;
}

int Unit_getAlly(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushunsigned(L, u->getAlly());

    return 1;
}

int Unit_isMyAlly(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnit* u2 = luaL_tounitptr(L, 2);

    lua_pushboolean(L, u->isMyAlly(DCAST(u2, CUnitForce*)));

    return 1;
}

int Unit_isMyEnemy(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnit* u2 = luaL_tounitptr(L, 2);

    lua_pushboolean(L, u->isMyEnemy(DCAST(u2, CUnitForce*)));

    return 1;
}

int Unit_canEffect(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnit* u2 = luaL_tounitptr(L, 2);
    auto eff = lua_tounsigned(L, 3);

    lua_pushboolean(L, u->canEffect(DCAST(u2, CUnitForce*), eff));

    return 1;
}

int Unit_setBaseArmor(lua_State* L)
{
    int armorType = lua_tointeger(L, 2);
    float armorValue = lua_tonumber(L, 3);

    CUnit* u = luaL_tounitptr(L);
    u->setBaseArmor(CArmorValue(armorType, armorValue));

    return 0;
}

int Unit_getBaseArmor(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const CArmorValue& av = u->getBaseArmor();

    lua_pushinteger(L, av.getType());
    lua_pushnumber(L, av.getValue());

    return 2;
}

int Unit_getRealArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushnumber(L, u->getRealArmorValue());

    return 1;
}

int Unit_setExArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    u->getExArmorValue().setMulriple(exA);
    u->getExArmorValue().setAddend(exB);

    return 0;
}

int Unit_getExArmorValue(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    const CExtraCoeff& ex = u->getExArmorValue();
    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int Unit_suspend(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    u->suspend();

    return 0;
}

int Unit_isSuspended(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    lua_pushboolean(L, u->isSuspended());

    return 1;
}

int Unit_resume(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    u->resume();

    return 0;
}

int Unit_getBuffStackSize(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const char* root = lua_tostring(L, 2);

    int count = 0;
    CUnit::MAP_BUFF_ABILITIES& mapBuffs = u->getBuffAbilities();
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

int Unit_attack(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = nullptr;
    luaL_toobjptr(L, 2, ad);
    CUnit* t = luaL_tounitptr(L, 3);
    unsigned int mask = luaL_tounsigneddef(L, 4, 0);

    lua_pushboolean(L, u->attack(ad, t, mask));

    return 1;
}

int Unit_damaged(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = nullptr;
    luaL_toobjptr(L, 2, ad);
    CUnit* s = luaL_tounitptr(L, 3);
    unsigned int mask = luaL_tounsigneddef(L, 4, 0);

    u->damaged(ad, s, mask);

    return 0;
}

int Unit_attackLow(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackData* ad = nullptr;
    luaL_toobjptr(L, 2, ad);
    CUnit* t = luaL_tounitptr(L, 3);
    unsigned int mask = luaL_tounsigneddef(L, 4, 0);

    u->attackLow(ad, t, mask);

    return 0;
}

int Unit_damagedLow(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float dmg = lua_tonumber(L, 2);
    CUnit* s = luaL_tounitptr(L, 3);
    unsigned int mask = luaL_tounsigneddef(L, 4, 0);

    u->damagedLow(dmg, s, mask);

    return 0;
}

int Unit_setRewardGold(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int gold = lua_tointeger(L, 2);

    u->setRewardGold(gold);

    return 0;
}

int Unit_getRewardGold(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushinteger(L, u->getRewardGold());

    return 1;
}

int Unit_setRewardExp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int exp = lua_tointeger(L, 2);

    u->setRewardExp(exp);

    return 0;
}

int Unit_getRewardExp(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushinteger(L, u->getRewardExp());

    return 1;
}

int Unit_setAI(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnitAI* ai = nullptr;
    luaL_toobjptr(L, 2, ai);

    u->setAI(ai);

    return 0;
}

int Unit_say(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const char* s = lua_tostring(L, 2);

    CUnitDraw* d = u->getDraw();
    d->say(s);

    return 0;
}

int Unit_setGhost(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    if (lua_gettop(L) < 2)
    {
        u->setGhost(u->getId());
    }
    else
    {
        int owner = luaL_tounitid(L, 2);
        u->setGhost(owner);
    }

    return 0;
}

int Unit_setEnergy(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    auto energy = lua_tointeger(L, 2);

    u->setEnergy(energy);

    return 0;
}

int Unit_getEnergy(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushinteger(L, u->getEnergy());

    return 1;
}

int Unit_setVisible(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    bool v = lua_toboolean(L, 2) != 0;

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setVisible(v);

    return 0;
}

int Unit_isVisible(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    lua_pushboolean(L, d->isVisible());

    return 1;
}

int Unit_addGold(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    auto gold = lua_tointeger(L, 2);

    auto pRes = _p->getResource();
    if (pRes != nullptr)
    {
        pRes->changeGold(gold);
    }

    return 0;
}

int Unit_getKiller(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);

    auto k = _p->getKiller();
    if (k == 0)
    {
        lua_pushnil(L);
    }
    else
    {
        lua_getglobal(L, "_world");
        auto w = (CWorld*)lua_touserdata(L, lua_gettop(L));
        auto u = w->getUnit(k);
        lua_pop(L, 1);
        luaL_pushobjptr(L, "Unit", u);
    }

    return 1;
}

int Unit_startDoing(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    unsigned int doing = lua_tounsigned(L, 2);

    u->startDoing(doing);

    return 0;
}

int Unit_endDoing(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    unsigned int doing = lua_tounsigned(L, 2);

    u->endDoing(doing);

    return 0;
}

int Unit_isDoingOr(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    unsigned int doing = lua_tounsigned(L, 2);

    lua_pushboolean(L, u->isDoingOr(doing));

    return 1;
}

int Unit_isDoingAnd(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    unsigned int doing = lua_tounsigned(L, 2);

    lua_pushboolean(L, u->isDoingAnd(doing));

    return 1;
}

int Unit_isDoingNothing(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_pushboolean(L, u->isDoingNothing());

    return 1;
}

int Unit_addActiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CActiveAbility* a = DCAST(luaL_toabilityptr(L, 2), CActiveAbility*);
        u->addActiveAbility(a, luaL_tobooleandef(L, 3, true));
    }
    else
    {
        int id = lua_tointeger(L, 2);
        u->addActiveAbility(id, luaL_tointegerdef(L, 3, 1), luaL_tobooleandef(L, 4, true));
    }

    return 0;
}

int Unit_addPassiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CPassiveAbility* a = DCAST(luaL_toabilityptr(L, 2), CPassiveAbility*);
        u->addPassiveAbility(a, luaL_tobooleandef(L, 3, true));
    }
    else
    {
        int id = lua_tointeger(L, 2);
        u->addPassiveAbility(id, luaL_tointegerdef(L, 3, 1), luaL_tobooleandef(L, 4, true));
    }

    return 0;
}

int Unit_addBuffAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    if (lua_istable(L, 2))
    {
        CBuffAbility* a = DCAST(luaL_toabilityptr(L, 2), CBuffAbility*);
        u->addBuffAbility(a, luaL_tobooleandef(L, 3, true));
    }
    else
    {
        int id = lua_tointeger(L, 2);
        int src = luaL_tounitid(L, 3);
        u->addBuffAbility(id, src, luaL_tointegerdef(L, 4, 1), luaL_tobooleandef(L, 5, true));
    }

    return 0;
}

int Unit_delActiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);

    u->delActiveAbility(id);

    return 0;
}

int Unit_delPassiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);

    u->delPassiveAbility(id);

    return 0;
}

int Unit_delBuffAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);

    u->delBuffAbility(id);

    return 0;
}

int Unit_getAttackAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CAttackAct* _p = nullptr;
    u->getActiveAbility(u->getAttackAbilityId())->dcast(_p);
    if (_p == nullptr)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "AttackAct", _p);
    }

    return 1;
}

int Unit_getActiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const char* name = lua_tostring(L, 2);

    CActiveAbility* _p = u->getActiveAbility(name);
    if (_p == nullptr)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "ActiveAbility", _p);
    }

    return 1;
}

int Unit_getActiveAbilities(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_newtable(L);
    int t = lua_gettop(L);

    auto& am = u->getActiveAbilities();

    int i = 1;
    M_MAP_FOREACH(am)
    {
        CActiveAbility* a = M_MAP_EACH;
        M_MAP_NEXT;

        if (a->isTemporary())
        {
            continue;
        }

        luaL_pushobjptr(L, "ActiveAbility", a);
        lua_rawseti(L, t, i);
        ++i;
    }

    return 1;
}

int Unit_getBuffAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    const char* name = lua_tostring(L, 2);

    CBuffAbility* _p = u->getBuffAbility(name);
    if (_p == nullptr)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "BuffAbility", _p);
    }

    return 1;
}

int Unit2D_setBaseMoveSpeed(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float n = lua_tonumber(L, 2);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setBaseMoveSpeed(n);

    return 0;
}

int Unit2D_getBaseMoveSpeed(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    
    lua_pushnumber(L, d->getBaseMoveSpeed());

    return 1;
}

int Unit2D_getRealMoveSpeed(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);

    lua_pushnumber(L, d->getRealMoveSpeed());

    return 1;
}

int Unit2D_setExMoveSpeed(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    float a = lua_tonumber(L, 2);
    float b = lua_tonumber(L, 3);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setExMoveSpeed(CExtraCoeff(a, b));

    return 0;
}

int Unit2D_getExMoveSpeed(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    CExtraCoeff& ex = d->getExMoveSpeed();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int Unit2D_getHalfOfWidth(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);

    lua_pushnumber(L, d->getHalfOfWidth());

    return 1;
}

int Unit2D_getHalfOfHeight(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    
    lua_pushnumber(L, d->getHalfOfHeight());

    return 1;
}

int Unit2D_setPosition(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CPoint p(lua_tonumber(L, 2), lua_tonumber(L, 3));

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setPosition(p);

    return 0;
}

int Unit2D_getPosition(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    CPoint& p = d->getPosition();

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Unit2D_setFlippedX(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    bool flipped = luaL_tobooleandef(L, 2, true);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->setFlippedX(flipped);

    return 0;
}

int Unit2D_isFlippedX(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);

    lua_pushboolean(L, d->isFlippedX());

    return 1;
}

int Unit2D_getNearestEnemyInRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float r = lua_tonumber(L, 2);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = CUnitGroup::getNearestUnitInRange(w, d->getPosition(), r, CUnitGroup::matchLivingEnemy(u));
    lua_pop(L, 1);

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int Unit2D_getNearestUnitInRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float r = lua_tonumber(L, 2);
    int matchfunc = 3;

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = CUnitGroup::getNearestUnitInRange(w, d->getPosition(), r, [L, matchfunc](CUnit* u)->bool{
        if (lua_isfunction(L, matchfunc))
        {
            lua_pushvalue(L, matchfunc);
            luaL_pushobjptr(L, "Unit", u);

            bool res = false;
            if (lua_pcall(L, 1, 1, 0) != LUA_OK)
            {
                const char* err = lua_tostring(L, -1);
                lua_getglobal(L, "log");
                lua_pushvalue(L, -2);
                lua_call(L, 1, 0);
                lua_pop(L, 1);
            }
            else
            {
                res = lua_toboolean(L, -1) != 0;
            }

            lua_pop(L, 1);
            
            return res;
        }
        else
        {
            return true;
        }
    });
    lua_pop(L, 1);

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int Unit2D_move(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    bool ob = luaL_tobooleandef(L, 4, true);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdMove(CPoint(x, y), ob);

    return 0;
}

int Unit2D_moveAlongPath(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    CUnitPath* up = nullptr;
    luaL_toobjptr(L, 2, up);
    bool ob = luaL_tobooleandef(L, 3, true);
    float ba = luaL_tonumberdef(L, 4, 5.0f);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdMoveAlongPath(up, ob, ba);

    return 0;
}

int Unit2D_setCastTarget(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    int n = lua_gettop(L);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    switch (n)
    {
    case 1:
        d->setCastTarget(CCommandTarget());
        break;

    case 2:
        d->setCastTarget(CCommandTarget(luaL_tounitid(L, 2)));
        break;

    case 3:
        d->setCastTarget(CCommandTarget(CPoint(lua_tonumber(L, 2), lua_tonumber(L, 3))));
        break;
    }

    return 0;
}

int Unit2D_getCastTarget(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    const CCommandTarget& rTarget = d->getCastTarget();

    lua_newtable(L);
    int t = lua_gettop(L);
    lua_pushinteger(L, rTarget.getTargetType());
    lua_setfield(L, t, "type");
    switch (rTarget.getTargetType())
    {
    case CCommandTarget::kNoTarget:
        break;

    case CCommandTarget::kUnitTarget:
        lua_pushinteger(L, rTarget.getTargetUnit());
        lua_setfield(L, t, "unit");
        break;

    case CCommandTarget::kPointTarget:
        lua_pushnumber(L, rTarget.getTargetPoint().x);
        lua_setfield(L, t, "x");
        lua_pushnumber(L, rTarget.getTargetPoint().y);
        lua_setfield(L, t, "y");
        break;
    }

    return 1;
}

int Unit2D_castSpellWithoutTarget(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);
    bool ob = luaL_tobooleandef(L, 3, true);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    int res = d->cmdCastSpell(CCommandTarget(), id, ob);

    lua_pushinteger(L, res);

    return 1;
}

int Unit2D_castSpellWithTargetUnit(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);
    int t = luaL_tounitid(L, 3);
    bool ob = luaL_tobooleandef(L, 4, true);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    int res = d->cmdCastSpell(CCommandTarget(t), id, ob);

    lua_pushinteger(L, res);

    return 1;
}

int Unit2D_castSpellWithTargetPoint(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    int id = luaL_toabilityid(L, 2);
    bool ob = luaL_tobooleandef(L, 5, true);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    int res = d->cmdCastSpell(CCommandTarget(CPoint(lua_tonumber(L, 3), lua_tonumber(L, 4))), id, ob);

    lua_pushinteger(L, res);

    return 1;
}

int Unit2D_stop(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->cmdStop();

    return 0;
}

int Unit2D_setHostilityRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float h = lua_tonumber(L, 2);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setHostilityRange(h);

    return 0;
}

int Unit2D_getHostilityRange(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

    lua_pushnumber(L, d->getHostilityRange());

    return 1;
}

int Unit2D_setFixed(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    bool fx = luaL_tobooleandef(L, 2, true);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    d->setFixed(fx);

    return 0;
}

int Unit2D_isFixed(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

    lua_pushboolean(L, d->isFixed());

    return 1;
}

int Unit2D_isDoingCastingAction(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

    lua_pushboolean(L, d->getCastActionId());

    return 1;
}

int Unit2D_getCastingActiveAbility(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    auto a = u->getActiveAbility(d->getCastActiveAbilityId());
    if (a == nullptr)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "ActiveAbility", a);
    }

    return 1;
}

int Unit2D_getDistance(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

    if (lua_gettop(L) == 2)
    {
        CUnit* t = luaL_tounitptr(L, 2);
        
        CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
        
        lua_pushnumber(L, d->getPosition().getDistance(td->getPosition()));
    }
    else
    {
        auto x = lua_tonumber(L, 2);
        auto y = lua_tonumber(L, 3);

        lua_pushnumber(L, d->getPosition().getDistance(CPoint(x, y)));
    }

    return 1;
}

int Unit2D_getTouchDistance(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

    if (lua_gettop(L) == 2)
    {
        CUnit* t = luaL_tounitptr(L, 2);
        CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);

        lua_pushnumber(L, d->getPosition().getDistance(td->getPosition()) - d->getHalfOfWidth() - td->getHalfOfWidth());
    }
    else
    {
        auto x = lua_tonumber(L, 2);
        auto y = lua_tonumber(L, 3);

        lua_pushnumber(L, d->getPosition().getDistance(CPoint(x, y)) - d->getHalfOfWidth());
    }
    

    return 1;
}

int Unit2D_getAttackingTarget(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    if (_p->getAttackAbilityId() != 0 && _p->getAttackAbilityId() == d->getCastActiveAbilityId() && d->getCastTarget().getTargetType() == CCommandTarget::kUnitTarget)
    {
        CUnit* t = _p->getUnit(d->getCastTarget().getTargetUnit());
        if (t != nullptr)
        {
            luaL_pushobjptr(L, "Unit", t);

            return 1;
        }
    }

    lua_pushnil(L);

    return 1;
}

int Unit2D_doAnimation(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    int id = lua_tointeger(L, 2);
    int iRepeatTimes = lua_tointeger(L, 3);
    float fSpeed = luaL_tonumberdef(L, 4, 1.0f);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    auto tag = d->doAnimation(id, nullptr, iRepeatTimes, nullptr, fSpeed);

    lua_pushinteger(L, tag);

    return 1;
}

int Unit2D_stopAction(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    int tag = lua_tointeger(L, 2);

    CUnitDraw2D* d = DCAST(_p->getDraw(), CUnitDraw2D*);
    d->stopAction(tag);

    return 0;
}

luaL_Reg UnitPath_funcs[] = {
    { "ctor", UnitPath_ctor },
    { "addPoint", UnitPath_addPoint },
    { "getFirstPoint", UnitPath_getFirstPoint },
    { nullptr, nullptr }
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
    CUnitPath* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);

    _p->addPoint(CPoint(x, y));

    return 0;
}

int UnitPath_getFirstPoint(lua_State* L)
{
    CUnitPath* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CPoint* p = _p->getCurTargetPoint(0);
    if (p != nullptr)
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

luaL_Reg UnitAI_funcs[] = {
    { "ctor", UnitAI_ctor },
    { "onUnitHpChanged", UnitAI_onUnitHpChanged },
    { "onUnitTick", UnitAI_onUnitTick },
    { "onUnitDamagedDone", UnitAI_onUnitDamagedDone },
    { "onUnitDamageTargetDone", UnitAI_onUnitDamageTargetDone },
    { "onUnitAddActiveAbility", UnitAI_onUnitAddActiveAbility },
    { "onUnitDelActiveAbility", UnitAI_onUnitDelActiveAbility },
    { "onUnitAddPassiveAbility", UnitAI_onUnitAddPassiveAbility },
    { "onUnitDelPassiveAbility", UnitAI_onUnitDelPassiveAbility },
    { "onUnitAddBuffAbility", UnitAI_onUnitAddBuffAbility },
    { "onUnitDelBuffAbility", UnitAI_onUnitDelBuffAbility },
    { "onUnitAbilityReady", UnitAI_onUnitAbilityReady },
    { nullptr, nullptr }
};

int UnitAI_ctor(lua_State* L)
{
    CUnitAI* _p = new CUnitAI;
    _p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int UnitAI_onUnitHpChanged(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitTick(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitDamagedDone(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitDamageTargetDone(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitAddActiveAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitDelActiveAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitAddPassiveAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitDelPassiveAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitAddBuffAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitDelBuffAbility(lua_State* L)
{
    return 0;
}

int UnitAI_onUnitAbilityReady(lua_State* L)
{
    return 0;
}

luaL_Reg Projectile_funcs[] = {
    { "ctor", Projectile_ctor },
    { "setSrcUnit", Projectile_setSrcUnit },
    { "setFromUnit", Projectile_setFromUnit },
    { "setToUnit", Projectile_setToUnit },
    { "setFromPoint", Projectile_setFromPoint },
    { "setToPoint", Projectile_setToPoint },
    { "setFromToType", Projectile_setFromToType },
    { "setFireType", Projectile_setFireType },
    { "setPenaltyFlags", Projectile_setPenaltyFlags },
    { "setEffectiveTypeFlags", Projectile_setEffectiveTypeFlags },
    { "getEffectiveTypeFlags", Projectile_getEffectiveTypeFlags },
    { "fire", Projectile_fire },
    { "die", Projectile_die },
    { "setPosition", Projectile_setPosition },
    { "getPosition", Projectile_getPosition },
    { "setHeight", Projectile_setHeight },
    { "getHeight", Projectile_getHeight },
    { "setMoveSpeed", Projectile_setMoveSpeed },
    { "getMoveSpeed", Projectile_getMoveSpeed },
    { "setMaxHeightDelta", Projectile_setMaxHeightDelta },
    { "getMaxHeightDelta", Projectile_getMaxHeightDelta },
    { "setVisible", Projectile_setVisible },
    { "addFireSound", Projectile_addFireSound },
    { "addEffectSound", Projectile_addEffectSound },
    { "setAttackData", Projectile_setAttackData },
    { "getAttackData", Projectile_getAttackData },
    { "setSrcAbility", Projectile_setSrcAbility },
    { "setContactLeft", Projectile_setContactLeft },
    { "getContactLeft", Projectile_getContactLeft },
    { "decContactLeft", Projectile_decContactLeft },
    { nullptr, nullptr }
};

int Projectile_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);

    assert(false);

    CProjectile* p = new CProjectile;

    lua_pushlightuserdata(L, p);
    lua_setfield(L, 1, "_p");

    //lua_getglobal(L, "_world");
    //CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    //lua_pop(L, 1);

    //w->addProjectile(p);

    return 0;
}

int Projectile_setSrcUnit(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setSrcUnit(luaL_tounitid(L, 2));

    return 0;
}

int Projectile_setFromUnit(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setFromUnit(luaL_tounitid(L, 2));

    return 0;
}

int Projectile_setToUnit(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setToUnit(luaL_tounitid(L, 2));

    return 0;
}

int Projectile_setFromPoint(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setFromPoint(CPoint(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    return 0;
}

int Projectile_setToPoint(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setToPoint(CPoint(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    return 0;
}

int Projectile_setFromToType(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setFromToType((CProjectile::FROM_TO_TYPE)lua_tointeger(L, 2));

    return 0;
}

int Projectile_setFireType(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setFireType((CProjectile::FIRE_TYPE)lua_tointeger(L, 2));

    return 0;
}

int Projectile_setPenaltyFlags(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setPenaltyFlags(lua_tounsigned(L, 2));

    return 0;
}

int Projectile_setEffectiveTypeFlags(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setEffectiveTypeFlags(lua_tounsigned(L, 2));

    return 0;
}

int Projectile_getEffectiveTypeFlags(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushunsigned(L, _p->getEffectiveTypeFlags());

    return 1;
}

int Projectile_fire(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->fire();

    return 0;
}

int Projectile_die(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->die();

    return 0;
}

int Projectile_redirect(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->redirect();

    return 0;
}

int Projectile_setPosition(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setPosition(CPoint(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    return 0;
}

int Projectile_getPosition(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CPoint& p = _p->getPosition();
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Projectile_setHeight(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setHeight(lua_tonumber(L, 2));

    return 0;
}

int Projectile_getHeight(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getHeight());

    return 1;
}

int Projectile_setMoveSpeed(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setMoveSpeed(lua_tonumber(L, 2));

    return 0;
}

int Projectile_getMoveSpeed(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getMoveSpeed());

    return 1;
}

int Projectile_setMaxHeightDelta(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setMaxHeightDelta(lua_tonumber(L, 2));

    return 0;
}

int Projectile_getMaxHeightDelta(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getMaxHeightDelta());

    return 1;
}

int Projectile_setVisible(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->setVisible(lua_toboolean(L, 2) != 0);

    return 0;
}

int Projectile_addFireSound(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->addFireSound(lua_tostring(L, 2));

    return 0;
}

int Projectile_addEffectSound(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    _p->addEffectSound(lua_tostring(L, 2));

    return 0;
}

int Projectile_setAttackData(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    CAttackData* ad = nullptr;
    luaL_toobjptr(L, 2, ad);

    _p->setAttackData(ad);

    return 0;
}

int Projectile_getAttackData(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    luaL_pushobjptr(L, "AttackData", _p->getAttackData());

    return 1;
}

int Projectile_setSrcAbility(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    CAbility* a = luaL_toabilityptr(L, 2);

    _p->setSrcAbility(a);

    return 0;
}

int Projectile_setContactLeft(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int left = lua_tointeger(L, 2);

    _p->setContactLeft(left);

    return 0;
}

int Projectile_getContactLeft(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getContactLeft());

    return 1;
}

int Projectile_decContactLeft(lua_State* L)
{
    CProjectile* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int dec = luaL_tointegerdef(L, 2, 1);

    _p->decContactLeft(dec);

    return 0;
}

luaL_Reg Ability_funcs[] = {
    { "ctor", Ability_ctor },
    { "onCopy", Ability_onCopy },
    { "onLevelChanged", Ability_onLevelChanged },
    { "onUnitAddAbility", Ability_onUnitAddAbility },
    { "onUnitDelAbility", Ability_onUnitDelAbility },
    { "onUnitAbilityReady", Ability_onUnitAbilityReady },
    { "onUnitRevive", Ability_onUnitRevive },
    { "onUnitDying", Ability_onUnitDying },
    { "onUnitDead", Ability_onUnitDead },
    { "onUnitHpChanged", Ability_onUnitHpChanged },
    { "onUnitTick", Ability_onUnitTick },
    { "onUnitInterval", Ability_onUnitInterval },
    { "onUnitAttackTarget", Ability_onUnitAttackTarget },
    { "onUnitAttacked", Ability_onUnitAttacked },
    { "onUnitDamaged", Ability_onUnitDamaged },
    { "onUnitDamagedDone", Ability_onUnitDamagedDone },
    { "onUnitDamageTargetDone", Ability_onUnitDamageTargetDone },
    { "onUnitProjectileEffect", Ability_onUnitProjectileEffect },
    { "onUnitProjectileArrive", Ability_onUnitProjectileArrive },
    { "onUnitAbilityEffect", Ability_onUnitAbilityEffect },
    { "onUnitCalcDamageTarget", Ability_onUnitCalcDamageTarget },
    { "copy", Ability_copy },
    { "setTriggerFlags", Ability_setTriggerFlags },
    { "getName", Ability_getName },
    { "getOwner", Ability_getOwner },
    { "setInterval", Ability_setInterval },
    { "getInterval", Ability_getInterval },
    { "setCoolDown", Ability_setCoolDown },
    { "getCoolDown", Ability_getCoolDown },
    { "isCoolingDown", Ability_isCoolingDown },
    { "resetCD", Ability_resetCD },
    { "coolDown", Ability_coolDown },
    { "setLevel", Ability_setLevel },
    { "getLevel", Ability_getLevel },
    { "addEffectSound", Ability_addEffectSound },
    { "setImageName", Ability_setImageName },
    { "setDescribe", Ability_setDescribe },
    { "getDescribe", Ability_getDescribe },
    { "setLevelInfo", Ability_setLevelInfo },
    { "setGrade", Ability_setGrade },
    { "getGrade", Ability_getGrade },
    { "setCost", Ability_setCost },
    { "getCost", Ability_getCost },
    { "addCastAnimation", Ability_addCastAnimation },
    { "getCastRandomAnimation", Ability_getCastRandomAnimation },
    { nullptr, nullptr }
};

int Ability_ctor(lua_State* L)
{
    return 0;
}

int Ability_onLevelChanged(lua_State* L)
{
    return 0;
}

int Ability_onCopy(lua_State* L)
{
    return 0;
}

int Ability_onUnitAddAbility(lua_State* L)
{
    return 0;
}

int Ability_onUnitDelAbility(lua_State* L)
{
    return 0;
}

int Ability_onUnitAbilityReady(lua_State* L)
{
    return 0;
}

int Ability_onUnitRevive(lua_State* L)
{
    return 0;
}

int Ability_onUnitDying(lua_State* L)
{
    return 0;
}

int Ability_onUnitDead(lua_State* L)
{
    return 0;
}

int Ability_onUnitHpChanged(lua_State* L)
{
    return 0;
}

int Ability_onUnitTick(lua_State* L)
{
    return 0;
}

int Ability_onUnitInterval(lua_State* L)
{
    return 0;
}

int Ability_onUnitAttackTarget(lua_State* L)
{
    return 0;
}

int Ability_onUnitAttacked(lua_State* L)
{
    lua_pushboolean(L, true);

    return 1;
}

int Ability_onUnitDamaged(lua_State* L)
{
    return 0;
}

int Ability_onUnitDamagedDone(lua_State* L)
{
    return 0;
}

int Ability_onUnitDamageTargetDone(lua_State* L)
{
    return 0;
}

int Ability_onUnitProjectileEffect(lua_State* L)
{
    return 0;
}

int Ability_onUnitProjectileArrive(lua_State* L)
{
    return 0;
}

int Ability_onUnitAbilityEffect(lua_State* L)
{
    return 0;
}

int Ability_onUnitCalcDamageTarget(lua_State* L)
{
    return 0;
}

int Ability_copy(lua_State* L)
{
    CAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    CAbility* _p_ = nullptr;
    _p->copy()->dcast(_p_);

    luaL_getregistery(L, _p_->getScriptHandler());

    return 1;
}

int Ability_setTriggerFlags(lua_State* L)
{
    unsigned int trigger = lua_tounsigned(L, 2);

    CAbility* _p = luaL_toabilityptr(L, 1);
    _p->setTriggerFlags(trigger);

    return 0;
}

int Ability_getName(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushstring(L, _p->getName());

    return 1;
}

int Ability_getOwner(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    CUnit* u = _p->getOwner();

    luaL_pushobjptr(L, "Unit", u);

    return 1;
}

int Ability_setInterval(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    float fInterval = lua_tonumber(L, 2);

    _p->setInterval(fInterval);

    return 0;
}

int Ability_getInterval(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    _p->getInterval();

    return 1;
}

int Ability_setCoolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    float cd = lua_tonumber(L, 2);

    _p->setCoolDown(cd);

    return 1;
}

int Ability_getCoolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushnumber(L, _p->getCoolDown());

    return 1;
}

int Ability_isCoolingDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushboolean(L, _p->isCoolingDown());

    return 1;
}

int Ability_resetCD(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    _p->resetCD();

    return 0;
}

int Ability_coolDown(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    float from = luaL_tonumberdef(L, 2, 0.0f);

    _p->coolDown(from);

    return 0;
}

int Ability_setLevel(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    int lvl = lua_tointeger(L, 2);

    _p->setLevel(lvl);

    return 0;
}

int Ability_getLevel(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushinteger(L, _p->getLevel());

    return 1;
}

int Ability_addEffectSound(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    const char* sound = lua_tostring(L, 2);

    _p->addEffectSound(sound);

    return 0;
}

int Ability_setImageName(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    const char* image = lua_tostring(L, 2);

    _p->setImageName(image);

    return 0;
}

int Ability_setDescribe(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    auto desc = lua_tostring(L, 2);
    
    _p->setDescribe(desc);

    return 0;
}

int Ability_getDescribe(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushstring(L, _p->getDescribe());

    return 1;
}

int Ability_setLevelInfo(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    auto lvl = lua_tointeger(L, 2);
    auto type = lua_tointeger(L, 3);
    auto desc = lua_tostring(L, 4);

    _p->setLevelInfo(lvl, type, desc);

    return 0;
}

int Ability_setGrade(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    auto grade = lua_tointeger(L, 2);

    _p->setGrade((CAbility::GRADE)grade);

    return 0;
}

int Ability_getGrade(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushinteger(L, _p->getGrade());

    return 1;
}

int Ability_setCost(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    auto cost = lua_tointeger(L, 2);

    _p->setCost(cost);

    return 0;
}

int Ability_getCost(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushinteger(L, _p->getCost());

    return 1;
}

int Ability_addCastAnimation(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);
    int id = lua_tointeger(L, 2);

    _p->addCastAnimation(id);

    return 0;
}

int Ability_getCastRandomAnimation(lua_State* L)
{
    CAbility* _p = luaL_toabilityptr(L, 1);

    lua_pushinteger(L, _p->getCastRandomAnimation());

    return 1;
}

luaL_Reg ActiveAbility_funcs[] = {
    { "ctor", ActiveAbility_ctor },
    { "checkConditions", ActiveAbility_checkConditions },
    { "onUnitCastAbility", ActiveAbility_onUnitCastAbility },
    { "onUnitAbilityEffect", ActiveAbility_onUnitAbilityEffect },
    { "setEffectiveTypeFlags", ActiveAbility_setEffectiveTypeFlags },
    { "getEffectiveTypeFlags", ActiveAbility_getEffectiveTypeFlags },
    { "setCastRange", ActiveAbility_setCastRange },
    { "getCastRange", ActiveAbility_getCastRange },
    { "setCastMinRange", ActiveAbility_setCastMinRange },
    { "getCastMinRange", ActiveAbility_getCastMinRange },
    { "setCastTargetRadius", ActiveAbility_setCastTargetRadius },
    { "getCastTargetRadius", ActiveAbility_getCastTargetRadius },
    { "getCastTargetType", ActiveAbility_getCastTargetType },
    { "setTemplateProjectile", ActiveAbility_setTemplateProjectile },
    { "getTemplateProjectile", ActiveAbility_getTemplateProjectile },
    { "setCastHorizontal", ActiveAbility_setCastHorizontal },
    { "isCastHorizontal", ActiveAbility_isCastHorizontal },
    { "getAbilityEffectPoint", ActiveAbility_getAbilityEffectPoint },

    { nullptr, nullptr }
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

int ActiveAbility_onUnitAbilityEffect(lua_State* L)
{
    return 0;
}

int ActiveAbility_setEffectiveTypeFlags(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    unsigned int eff = lua_tounsigned(L, 2);

    _p->setEffectiveTypeFlags(eff);

    return 0;
}

int ActiveAbility_getEffectiveTypeFlags(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushunsigned(L, _p->getEffectiveTypeFlags());

    return 1;
}

int ActiveAbility_setCastRange(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float range = lua_tonumber(L, 2);

    _p->setCastRange(range);

    return 0;
}

int ActiveAbility_getCastRange(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getCastRange());

    return 1;
}

int ActiveAbility_setCastMinRange(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float range = lua_tonumber(L, 2);

    _p->setCastMinRange(range);

    return 0;
}

int ActiveAbility_getCastMinRange(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getCastMinRange());

    return 1;
}

int ActiveAbility_setCastTargetRadius(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float radius = lua_tonumber(L, 2);

    _p->setCastTargetRadius(radius);

    return 0;
}

int ActiveAbility_getCastTargetRadius(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getCastTargetRadius());

    return 1;
}

int ActiveAbility_getCastTargetType(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getCastTargetType());

    return 1;
}

int ActiveAbility_setTemplateProjectile(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int proj = lua_tointeger(L, 2);

    _p->setTemplateProjectile(proj);

    return 0;
}

int ActiveAbility_getTemplateProjectile(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushinteger(L, _p->getTemplateProjectile());

    return 1;
}

int ActiveAbility_setCastHorizontal(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    bool hor = lua_toboolean(L, 2) != 0;

    _p->setCastHorizontal(hor);

    return 0;
}

int ActiveAbility_isCastHorizontal(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushboolean(L, _p->isCastHorizontal());

    return 1;
}

int ActiveAbility_getAbilityEffectPoint(lua_State* L)
{
    CActiveAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    CProjectile* proj = nullptr;
    luaL_toobjptr(L, 2, proj);

    CUnit* target = nullptr;
    luaL_toobjptr(L, 3, target);

    const CPoint& p = _p->getAbilityEffectPoint(proj, target);

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
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

luaL_Reg BuffAbility_funcs[] = {
    { "ctor", BuffAbility_ctor },
    { "onUnitDisplaceAbility", BuffAbility_onUnitDisplaceAbility },
    { "setSrcUnit", BuffAbility_setSrcUnit },
    { "getSrcUnit", BuffAbility_getSrcUnit },
    { "setAppendBuff", BuffAbility_setAppendBuff },
    { "setDuration", BuffAbility_setDuration },
    { "getDuration", BuffAbility_getDuration },
    { nullptr, nullptr }
};

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

int BuffAbility_onUnitDisplaceAbility(lua_State* L)
{
    return 0;
}

int BuffAbility_setSrcUnit(lua_State* L)
{
    CBuffAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int s = luaL_tounitid(L, 2);
    
    _p->setSrcUnit(s);

    return 0;
}

int BuffAbility_getSrcUnit(lua_State* L)
{
    CBuffAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    CUnit* s = _p->getOwner()->getUnit(_p->getSrcUnit());
    if (s == nullptr)
    {
        lua_pushnil(L);
    }
    else
    {
        luaL_pushobjptr(L, "Unit", s);
    }

    return 1;
}

int BuffAbility_setAppendBuff(lua_State* L)
{
    CBuffAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int id = luaL_toabilityid(L, 2);

    _p->setAppendBuff(id);

    return 0;
}

int BuffAbility_setDuration(lua_State* L)
{
    CBuffAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float dur = lua_tonumber(L, 2);

    _p->setDuration(dur);

    return 0;
}

int BuffAbility_getDuration(lua_State* L)
{
    CBuffAbility* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getDuration());

    return 1;
}

luaL_Reg AttackAct_funcs[] = {
    { "ctor", AttackAct_ctor },
    { "setBaseAttack", AttackAct_setBaseAttack },
    { "getBaseAttack", AttackAct_getBaseAttack },
    { "getRealAttackValue", AttackAct_getRealAttackValue },
    { "setExAttackValue", AttackAct_setExAttackValue },
    { "getExAttackValue", AttackAct_getExAttackValue },
    { "setBaseAttackInterval", AttackAct_setBaseAttackInterval },
    { "getBaseAttackInterval", AttackAct_getBaseAttackInterval },
    { "getRealAttackInterval", AttackAct_getRealAttackInterval },
    { "getRealAttackSpeed", AttackAct_getRealAttackSpeed },
    { "setExAttackSpeed", AttackAct_setExAttackSpeed },
    { "getExAttackSpeed", AttackAct_getExAttackSpeed },
    { "setAttackValueRandomRange", AttackAct_setAttackValueRandomRange },
    { "getAttackValueRandomRange", AttackAct_getAttackValueRandomRange },
    { "setExAttackValueRandomRange", AttackAct_setExAttackValueRandomRange },
    { "getExAttackValueRandomRange", AttackAct_getExAttackValueRandomRange },
    
    { nullptr, nullptr }
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

    int lastArg = lua_gettop(L);
    assert(lastArg >= 9);


    CAttackAct* _p = new CAttackAct("Attack", name, cd, CAttackValue(at, av), ar);
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    _p->setCastMinRange(minRange);
    _p->setCastRange(maxRange);
    _p->setCastHorizontal(hor);
    for (int i = 10; i <= lastArg; ++i)
    {
        _p->addCastAnimation(lua_tointeger(L, i));
    }

    return 0;
}

int AttackAct_setBaseAttack(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    int type = lua_tointeger(L, 2);
    float value = lua_tonumber(L, 3);

    _p->setBaseAttack(CAttackValue(type, value));

    return 0;
}

int AttackAct_getBaseAttack(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CAttackValue& av = _p->getBaseAttack();
    lua_pushinteger(L, av.getType());
    lua_pushnumber(L, av.getValue());

    return 2;
}

int AttackAct_getRealAttackValue(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    bool rnd = luaL_tobooleandef(L, 2, true);

    lua_pushnumber(L, _p->getRealAttackValue(rnd));

    return 1;
}

int AttackAct_setExAttackValue(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    _p->setExAttackValue(CExtraCoeff(exA, exB));

    return 0;
}

int AttackAct_getExAttackValue(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CExtraCoeff& ex = _p->getExAttackValue();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int AttackAct_setBaseAttackInterval(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float cd = lua_tonumber(L, 2);

    _p->setBaseAttackInterval(cd);

    return 0;
}

int AttackAct_getBaseAttackInterval(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getBaseAttackInterval());

    return 1;
}

int AttackAct_getRealAttackInterval(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getRealAttackInterval());

    return 1;
}

int AttackAct_getRealAttackSpeed(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getRealAttackSpeed());

    return 1;
}

int AttackAct_setExAttackSpeed(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    _p->setExAttackSpeed(CExtraCoeff(exA, exB));

    return 0;
}

int AttackAct_getExAttackSpeed(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CExtraCoeff& ex = _p->getExAttackSpeed();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

int AttackAct_setAttackValueRandomRange(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float r = lua_tonumber(L, 2);

    _p->setAttackValueRandomRange(r);

    return 0;
}

int AttackAct_getAttackValueRandomRange(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_pushnumber(L, _p->getAttackValueRandomRange());

    return 1;
}

int AttackAct_setExAttackValueRandomRange(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    float exA = lua_tonumber(L, 2);
    float exB = lua_tonumber(L, 3);

    _p->setExAttackValueRandomRange(CExtraCoeff(exA, exB));

    return 0;
}

int AttackAct_getExAttackValueRandomRange(lua_State* L)
{
    CAttackAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    const CExtraCoeff& ex = _p->getExAttackValueRandomRange();

    lua_pushnumber(L, ex.getMulriple());
    lua_pushnumber(L, ex.getAddend());

    return 2;
}

luaL_Reg BuffMakerAct_funcs[] = {
    { "ctor", BuffMakerAct_ctor },
    { "setTemplateBuff", BuffMakerAct_setTemplateBuff },
    { nullptr, nullptr }
};

int BuffMakerAct_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float cd = lua_tonumber(L, 3);
    int target = lua_tointeger(L, 4);
    unsigned int effect = lua_tounsigned(L, 5);
    int buff = lua_tointeger(L, 6);

    CBuffMakerAct* _p = new CBuffMakerAct("BM", name, cd, (CCommandTarget::TARGET_TYPE)target, effect, buff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int BuffMakerAct_setTemplateBuff(lua_State* L)
{
    CBuffMakerAct* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    auto buff = lua_tointeger(L, 2);
    
    _p->setTemplateBuff(buff);

    return 0;
}

luaL_Reg AttackBuffMakerPas_funcs[] = {
    { "ctor", AttackBuffMakerPas_ctor },
    { "setChance", AttackBuffMakerPas_setChance },
    { "setTemplateBuff", AttackBuffMakerPas_setTemplateBuff },
    { "setExAttackValue", AttackBuffMakerPas_setExAttackValue },
    { "setTemplateAct", AttackBuffMakerPas_setTemplateAct },

    { nullptr, nullptr }
};

int AttackBuffMakerPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float chance = lua_tonumber(L, 3);
    int buff = lua_tointeger(L, 4);
    bool toself = lua_toboolean(L, 5) != 0;
    float exAttackA = lua_tonumber(L, 6);
    float exAttackB = lua_tonumber(L, 7);
    int act = lua_tointeger(L, 8);

    CAttackBuffMakerPas* _p = new CAttackBuffMakerPas("ABM", name, chance, buff, toself, CExtraCoeff(exAttackA, exAttackB), act);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int AttackBuffMakerPas_setChance(lua_State* L)
{
    CAttackBuffMakerPas* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    auto chance = lua_tonumber(L, 2);

    _p->setChance(chance);

    return 0;
}

int AttackBuffMakerPas_setTemplateBuff(lua_State* L)
{
    CAttackBuffMakerPas* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    auto buff = lua_tointeger(L, 2);

    _p->setTemplateBuff(buff);

    return 0;
}

int AttackBuffMakerPas_setExAttackValue(lua_State* L)
{
    CAttackBuffMakerPas* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    auto exAvA = lua_tonumber(L, 2);
    auto exAvB = lua_tonumber(L, 3);

    _p->setExAttackValue(CExtraCoeff(exAvA, exAvB));

    return 0;
}

int AttackBuffMakerPas_setTemplateAct(lua_State* L)
{
    CAttackBuffMakerPas* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    auto act = lua_tointeger(L, 2);

    _p->setTemplateAct(act);

    return 0;
}

int AuraPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float interval = lua_tonumber(L, 3);
    int buff = lua_tointeger(L, 4);
    float range = lua_tonumber(L, 5);
    unsigned int effect = lua_tounsigned(L, 6);
    bool effectFixed = lua_toboolean(L, 7) != 0;

    CAuraPas* _p = new CAuraPas("Aura", name, interval, buff, range, effect, effectFixed);
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

int DamageBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    int damageType = lua_tointeger(L, 3);
    float damageValue = lua_tonumber(L, 4);
    float exAvA = lua_tonumber(L, 5);
    float exAvB = lua_tonumber(L, 6);
    uint32_t mask = lua_tounsigned(L, 7);
    bool attack = luaL_tobooleandef(L, 8, false);
    bool forceAt = luaL_tobooleandef(L, 9, false);
    float effRadius = luaL_tonumberdef(L, 10, 0.0f);

    CDamageBuff* _p = new CDamageBuff(name, CAttackValue(damageType, damageValue), CExtraCoeff(exAvA, exAvB), mask, attack, forceAt, effRadius);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int TransitiveLinkBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    float range = lua_tonumber(L, 4);
    int maxTimes = lua_tointeger(L, 5);
    int minIvl = lua_tointeger(L, 6);
    uint32_t eff = lua_tounsigned(L, 7);
    bool nearest = luaL_tobooleandef(L, 8, true);
    int projectile = luaL_tointegerdef(L, 9, 0);

    CTransitiveLinkBuff* _p = new CTransitiveLinkBuff(name, duration, range, maxTimes, minIvl, eff, nearest);
    _p->setTemplateProjectile(projectile);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int TransitiveBlinkBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float range = lua_tonumber(L, 3);
    int maxTimes = lua_tointeger(L, 4);
    int minIvl = lua_tointeger(L, 5);
    int ani = lua_tointeger(L, 6);

    CTransitiveBlinkBuff* _p = new CTransitiveBlinkBuff(name, range, maxTimes, minIvl, ani);
    int n = lua_gettop(L);
    for (int i = 7; i <= n; ++i)
    {
        ani = lua_tointeger(L, i);
        _p->addCastAnimation(ani);
    }
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int SplashPas_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float nearRange = lua_tonumber(L, 3);
    float exNearA = lua_tonumber(L, 4);
    float exNearB = lua_tonumber(L, 5);
    float farRange = lua_tonumber(L, 6);
    float exFarA = lua_tonumber(L, 7);
    float exFarB = lua_tonumber(L, 8);
    uint32_t mask = luaL_tounsigneddef(L, 9, CUnit::kOnAttackTargetTrigger);
    uint32_t eff = luaL_tounsigneddef(L, 10, CUnitForce::kEnemy);

    CSplashPas* _p = new CSplashPas(name, nearRange, CExtraCoeff(exNearA, exNearB), farRange, CExtraCoeff(exFarA, exFarB), mask, eff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int KnockBackBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    float distance = lua_tonumber(L, 4);
    bool usingRefPos = lua_gettop(L) == 4;
    
    CKnockBackBuff* _p = nullptr;
    if (usingRefPos)
    {
        _p = new CKnockBackBuff(name, duration, distance);
    }
    else
    {
        float x = lua_tonumber(L, 5);
        float y = lua_tonumber(L, 6);
        _p = new CKnockBackBuff(name, duration, distance, CPoint(x, y));
    }
    
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int AttractBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    float distance = lua_tonumber(L, 4);

    CAttractBuff* _p = new CAttractBuff(name, duration, distance);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int ReflectBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);

    CReflectBuff* _p = new CReflectBuff(root, name, duration);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int LimitedLifeBuff_ctor(lua_State* L)
{
    const char* root = lua_tostring(L, 2);
    const char* name = lua_tostring(L, 3);
    float duration = lua_tonumber(L, 4);

    CLimitedLifeBuff* _p = new CLimitedLifeBuff(root, name, duration);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int ChargeJumpBuff_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    float exA = lua_tonumber(L, 4);
    float exB = lua_tonumber(L, 5);
    float dmgCoeff = lua_tonumber(L, 6);
    int buff = lua_tointeger(L, 7);

    CChargeJumpBuff* _p = new CChargeJumpBuff(name, duration, CExtraCoeff(exA, exB), dmgCoeff, buff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int LimitedPasBuff_ctor(lua_State* L)
{
    auto name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    int pas = lua_tointeger(L, 4);

    CLimitedPasBuff* _p = new CLimitedPasBuff(name, duration, pas);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int FastMoveToBackBuff_ctor(lua_State* L)
{
    auto name = lua_tostring(L, 2);
    float duration = lua_tonumber(L, 3);
    float cd = lua_tonumber(L, 4);
    float r = lua_tonumber(L, 5);
    float buff = lua_tointeger(L, 6);

    auto _p = new CFastMoveToBackBuff(name, duration, cd, r, buff);
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}

int luaRegCommFuncs(lua_State* L)
{
    // TODO: reg global funcs
    lua_register(L, "class", g_class);
    lua_register(L, "cast", g_cast);
    lua_register(L, "loadValue", g_loadValue);
    lua_register(L, "saveValue", g_saveValue);

    lua_register(L, "onLoadingUserData", g_onLoadingUserData);

    lua_register(L, "addTemplateAbility", g_addTemplateAbility);
    

    // TODO: reg global classes
    M_LUA_BIND_CLASS_WITH_FUNCS(L, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, LevelExp, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, LevelUpdate, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, AttackData, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, Unit, LevelExp);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, UnitPath, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, UnitAI, MRObj);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, Projectile, MRObj);

    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, Ability, LevelExp);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, ActiveAbility, Ability);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, PassiveAbility, Ability);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, BuffAbility, Ability);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, AttackAct, ActiveAbility);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, BuffMakerAct, ActiveAbility);
    M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, AttackBuffMakerPas, PassiveAbility);
    
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, AuraPas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, VampirePas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, StunBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, DoubleAttackPas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, SpeedBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, ChangeHpPas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, ChangeHpBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, RebirthPas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, EvadePas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, EvadeBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, DamageBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, TransitiveLinkBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, TransitiveBlinkBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, SplashPas, PassiveAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, KnockBackBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, AttractBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, ReflectBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, LimitedLifeBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, ChargeJumpBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, LimitedPasBuff, BuffAbility);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, FastMoveToBackBuff, BuffAbility);

    return 0;
}

int g_onWorldInit(lua_State* L)
{
    return 0;
}

int g_onWorldTick(lua_State* L)
{
    return 0;
}

int g_onUnitDead(lua_State* L)
{
    return 0;
}

int g_setCtrlUnit(lua_State* L)
{
    auto u = luaL_tounitptr(L, 1);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    w->setCtrlUnit(u);
    lua_pop(L, 1);

    return 0;
}

int g_getCtrlUnit(lua_State* L)
{
    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = w->getUnit(w->getCtrlUnit());
    lua_pop(L, 1);

    if (_p != nullptr)
    {
        luaL_pushobjptr(L, "Unit", _p);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

int g_getUnit(lua_State* L)
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CUnit* _p = w->getUnit(id);
    lua_pop(L, 1);

    if (_p != nullptr)
    {
        luaL_pushobjptr(L, "Unit", _p);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

int g_getUnits(lua_State* L)
{
    int matchfunc = 1;
    bool ghost = luaL_tobooleandef(L, 2, false);

    lua_newtable(L);
    int t = lua_gettop(L);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    CWorld::MAP_UNITS& units = w->getUnits();

    int i = 1;
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;

        if (ghost == false && u->isGhost())
        {
            continue;
        }

        if (lua_isfunction(L, matchfunc))
        {
            lua_pushvalue(L, matchfunc);
            luaL_pushobjptr(L, "Unit", u);

            bool res = false;
            if (lua_pcall(L, 1, 1, 0) != LUA_OK)
            {
                const char* err = lua_tostring(L, -1);
                return luaL_throwerror(L, true, err);
            }
            else
            {
                res = lua_toboolean(L, -1) != 0;
            }

            lua_pop(L, 1);
            if (res == false)
            {
                continue;
            }
        }

        luaL_pushobjptr(L, "Unit", u);
        lua_rawseti(L, t, i);
        ++i;
    }

    lua_pop(L, 1);

    return 1;
}

int g_addUnit(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addUnit(u);

    return 0;
}

int g_addProjectile(lua_State* L)
{
    CProjectile* p = nullptr;
    luaL_toobjptr(L, 1, p);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addProjectile(p);

    return 0;
}

int g_retainMRObj(lua_State* L)
{
    CMultiRefObject* _p = nullptr;
    luaL_toobjptr(L, 1, _p);

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->getMRObjsKeeper().addObject(_p);

    return 0;
}

int g_releaseMRObj(lua_State* L)
{
    int id = 0;
    if (lua_istable(L, 1))
    {
        CMultiRefObject* _p = nullptr;
        luaL_toobjptr(L, 1, _p);
        id = _p->getId();
    }
    else
    {
        id = lua_tointeger(L, 1);
    }

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->getMRObjsKeeper().delObject(id);

    return 0;
}

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars
    lua_pushlightuserdata(L, pWorld);
    lua_setglobal(L, "_world");

    // TODO: reg global funcs
    lua_register(L, "onWorldInit", g_onWorldInit);
    lua_register(L, "onWorldTick", g_onWorldTick);
    lua_register(L, "onUnitDead", g_onUnitDead);
    lua_register(L, "setCtrlUnit", g_setCtrlUnit);
    lua_register(L, "getCtrlUnit", g_getCtrlUnit);
    lua_register(L, "getUnit", g_getUnit);
    lua_register(L, "getUnits", g_getUnits);
    lua_register(L, "addUnit", g_addUnit);
    lua_register(L, "addProjectile", g_addProjectile);
    lua_register(L, "retainMRObj", g_retainMRObj);
    lua_register(L, "releaseMRObj", g_releaseMRObj);

    return 0;
}
