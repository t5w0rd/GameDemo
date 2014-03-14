/* 
 * File:   LuaBinding.h
 * Author: thunderliu
 *
 * Created on 2014年3月2日, 上午11:48
 */

#ifndef __LUABINDING_H__
#define __LUABINDING_H__


// common
int luaModuleLoader4cc(lua_State *L);
void luaL_insertloader(lua_State *L, lua_CFunction loader);
void luaL_addpath(lua_State *L, const char* pPath);
bool luaL_loadscript(lua_State *L, const char* name, string& err);
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
PTYPE luaL_toobjptr(lua_State* L, int idx, PTYPE& ptr);

template <typename PTYPE>
void luaL_pushobjptr(lua_State* L, const char* name, const PTYPE _p);


CUnit* luaL_tounitptr(lua_State* L, int idx = 1);
CAbility* luaL_toabilityptr(lua_State* L, int idx);

int mrobj_getId(lua_State* L);

int unit_ctor(lua_State* L);
int unit_setMaxHp(lua_State* L);
int unit_getMaxHp(lua_State* L);
int unit_setExMaxHp(lua_State* L);
int unit_getExMaxHp(lua_State* L);
int unit_getRealMaxHp(lua_State* L);
int unit_setHp(lua_State* L);
int unit_getHp(lua_State* L);
int unit_setForceByIndex(lua_State* L);
int unit_setAlly(lua_State* L);
int unit_setBaseArmor(lua_State* L);
int unit_getBaseArmor(lua_State* L);
int unit_getRealArmorValue(lua_State* L);
int unit_setExArmorValue(lua_State* L);
int unit_getExArmorValue(lua_State* L);
int unit_suspend(lua_State* L);
int unit_isSuspended(lua_State* L);
int unit_resume(lua_State* L);
int unit_getBuffStackSize(lua_State* L);
int unit_attack(lua_State* L);
int unit_damaged(lua_State* L);
int unit_attackLow(lua_State* L);
int unit_damagedLow(lua_State* L);

int unit_addActiveAbility(lua_State* L);
int unit_addPassiveAbility(lua_State* L);
int unit_addBuffAbility(lua_State* L);
int unit_getAttackAbility(lua_State* L);

int unit2d_setBaseMoveSpeed(lua_State* L);
int unit2d_getRealMoveSpeed(lua_State* L);
int unit2d_setExMoveSpeed(lua_State* L);
int unit2d_getExMoveSpeed(lua_State* L);
int unit2d_setPosition(lua_State* L);
int unit2d_getPosition(lua_State* L);
int unit2d_getNearestEnemyInRange(lua_State* L);
int unit2d_move(lua_State* L);
int unit2d_moveAlongPath(lua_State* L);
int unit2d_castSpell(lua_State* L);
int unit2d_stop(lua_State* L);
int unit2d_setHostilityRange(lua_State* L);
int unit2d_getHostilityRange(lua_State* L);

int UnitPath_ctor(lua_State* L);
int UnitPath_addPoint(lua_State* L);
int UnitPath_getFirstPoint(lua_State* L);

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
int ability_onUnitAbilityProjectileEffect(lua_State* L);
int ability_copy(lua_State* L);
int ability_setTriggerFlags(lua_State* L);
int ability_getOwner(lua_State* L);
int ability_setInterval(lua_State* L);
int ability_getInterval(lua_State* L);
int ability_setCoolDown(lua_State* L);
int ability_getCoolDown(lua_State* L);
int ability_isCoolingDown(lua_State* L);
int ability_resetCD(lua_State* L);
int ability_coolDown(lua_State* L);
int ability_setLevel(lua_State* L);
int ability_getLevel(lua_State* L);

int ActiveAbility_ctor(lua_State* L);
int ActiveAbility_checkConditions(lua_State* L);
int ActiveAbility_onUnitCastAbility(lua_State* L);
int PassiveAbility_ctor(lua_State* L);
int BuffAbility_ctor(lua_State* L);

int AttackAct_ctor(lua_State* L);
int AttackAct_setBaseAttack(lua_State* L);
int AttackAct_getBaseAttack(lua_State* L);
int AttackAct_getRealAttackValue(lua_State* L);
int AttackAct_setExAttackValue(lua_State* L);
int AttackAct_getExAttackValue(lua_State* L);
int AttackAct_setBaseAttackInterval(lua_State* L);
int AttackAct_getBaseAttackInterval(lua_State* L);
int AttackAct_getRealAttackInterval(lua_State* L);
int AttackAct_getRealAttackSpeed(lua_State* L);
int AttackAct_setExAttackSpeed(lua_State* L);
int AttackAct_getExAttackSpeed(lua_State* L);

int AttackBuffMakerPas_ctor(lua_State* L);
int AuraPas_ctor(lua_State* L);
int VampirePas_ctor(lua_State* L);
int StunBuff_ctor(lua_State* L);
int DoubleAttackPas_ctor(lua_State* L);
int SpeedBuff_ctor(lua_State* L);
int ChangeHpPas_ctor(lua_State* L);
int ChangeHpBuff_ctor(lua_State* L);
int RebirthPas_ctor(lua_State* L);
int EvadePas_ctor(lua_State* L);
int EvadeBuff_ctor(lua_State* L);
int BuffMakerAct_ctor(lua_State* L);
int DamageBuffMakerBuff_ctor(lua_State* L);

int AttackData_ctor(lua_State* L);
int AttackData_setAttack(lua_State* L);
int AttackData_getAttack(lua_State* L);
int AttackData_setAttackType(lua_State* L);
int AttackData_getAttackType(lua_State* L);
int AttackData_setAttackValue(lua_State* L);
int AttackData_getAttackValue(lua_State* L);
int AttackData_addAttackBuff(lua_State* L);

int g_onWorldInit(lua_State* L);
int g_onWorldTick(lua_State* L);
int g_addTemplateAbility(lua_State* L);
int g_setControlUnit(lua_State* L);
int g_getControlUnit(lua_State* L);

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld);




///////////////////////////////// Inline //////////////////////////////////////
template <typename PTYPE>
PTYPE luaL_toobjptr(lua_State* L, int idx, PTYPE& ptr)
{
    lua_getfield(L, idx, "_p");
    ptr = (PTYPE)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ptr;
}

template <typename PTYPE>
void luaL_pushobjptr(lua_State* L, const char* name, const PTYPE _p)
{
    lua_newtable(L);
    int obj = lua_gettop(L);

    lua_getglobal(L, name);
    lua_setmetatable(L, obj);

    lua_pushlightuserdata(L, _p);
    lua_setfield(L, obj, "_p");
}

#endif  /* __LUABINDING_H__ */

