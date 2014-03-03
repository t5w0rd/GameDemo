/* 
 * File:   LuaBinding.h
 * Author: thunderliu
 *
 * Created on 2014年3月2日, 上午11:48
 */

#ifndef __LUABINDING_H__
#define	__LUABINDING_H__


// common
int luaL_getcopy(lua_State* L, int idx);
int luaL_setregistry(lua_State* L, int idx);
int luaL_getregistery(lua_State* L, int key);

int obj_sctor(lua_State* L);
int obj_ctor(lua_State* L);
int class_type_new(lua_State* L);
int g_class(lua_State* L);

int luaRegCommFunc(lua_State* L);

class CUnit;
class CAbility;
class CWorld;

// game
#define M_LUA_BIND_CLASS(L, name) \
    do \
    { \
        lua_getglobal((L), "class"); \
        lua_call((L), 0, 1); \
        lua_pushcfunction((L), name##_ctor); \
        lua_setfield((L), -2, "ctor"); \
        lua_setglobal((L), #name); \
    } while (false)

#define M_LUA_BIND_CLASS_EX(L, name, base) \
    do \
    { \
        lua_getglobal((L), "class"); \
        lua_getglobal((L), #base); \
        lua_call((L), 1, 1); \
        lua_pushcfunction((L), name##_ctor); \
        lua_setfield((L), -2, "ctor"); \
        lua_setglobal((L), #name); \
    } while (false)
template <typename PTYPE>
PTYPE getObjPtr(PTYPE& ptr, lua_State* L, int idx = 1);

CUnit* getUnitPtr(lua_State* L, int idx = 1);
CAbility* getAbilityPtr(lua_State* L, int idx);

int unit_ctor(lua_State* L);
int unit_setMaxHp(lua_State* L);
int unit_setHp(lua_State* L);
int unit_setForceByIndex(lua_State* L);
int unit_setAlly(lua_State* L);
int unit_setBaseArmor(lua_State* L);

int unit_addActiveAbility(lua_State* L);
int unit_addPassiveAbility(lua_State* L);
int unit_addBuffAbility(lua_State* L);

int uint2d_setBaseMoveSpeed(lua_State* L);
int unit2d_setPosition(lua_State* L);

int ability_ctor(lua_State* L);
int ability_onUnitAddAbility(lua_State* L);
int ability_onUnitDelAbility(lua_State* L);
int ability_onUnitAbilityReady(lua_State* L);
int ability_onUnitRevive(lua_State* L);
int ability_onUnitDying(lua_State* L);
int ability_onUnitDead(lua_State* L);
int ability_onUnitChangeHp(lua_State* L);
int ability_onUnitTick(lua_State* L);
int ability_onUnitInterval(lua_State* L);
int ability_onUnitAttackTarget(lua_State* L);
int ability_onUnitAttacked(lua_State* L);
int ability_onUnitDamaged(lua_State* L);
int ability_onUnitDamagedDone(lua_State* L);
int ability_onUnitDamageTargetDone(lua_State* L);
int ability_onUnitProjectileEffect(lua_State* L);

int ability_setTriggerFlags(lua_State* L);

int g_addUnit(lua_State* L);
int g_addTemplateAbility(lua_State* L);
int g_setControlUnit(lua_State* L);

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld);

int ActiveAbility_ctor(lua_State* L);
int ActiveAbility_checkConditions(lua_State* L);
int ActiveAbility_onUnitCastAbility(lua_State* L);

int PassiveAbility_ctor(lua_State* L);


int AttackAct_ctor(lua_State* L);



///////////////////////////////// Inline //////////////////////////////////////
template <typename PTYPE>
PTYPE getObjPtr( PTYPE& ptr, lua_State* L, int idx /*= 1*/ )
{
    lua_getfield(L, idx, "_p");
    PTYPE* ret = (PTYPE*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}


#endif	/* __LUABINDING_H__ */

