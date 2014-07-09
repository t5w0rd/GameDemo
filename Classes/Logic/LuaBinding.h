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
const char* luaL_toutf8string(lua_State* L, int idx, char* buf);


class CUnit;
class CAbility;
class CWorld;


#define M_LUA_BIND_CLASS_WITH_FUNCS(L, name) \
do \
{ \
    lua_getglobal((L), "class"); \
    lua_call((L), 0, 1); \
    luaL_setfuncs((L), name##_funcs, 0); \
    lua_setglobal((L), #name); \
} while (false)

#define M_LUA_BIND_CLASS_WITH_FUNCS_EX(L, name, base) \
do \
{ \
    lua_getglobal((L), "class"); \
    lua_getglobal((L), #base); \
    lua_call((L), 1, 1); \
    luaL_setfuncs((L), name##_funcs, 0); \
    lua_setglobal((L), #name); \
} while (false)

#define M_LUA_BIND_CLASS_WITH_CTOR(L, name) \
do \
{ \
    lua_getglobal((L), "class"); \
    lua_call((L), 0, 1); \
    lua_pushcfunction((L), name##_ctor); \
    lua_setfield((L), -2, "ctor"); \
    lua_setglobal((L), #name); \
} while (false)

#define M_LUA_BIND_CLASS_WITH_CTOR_EX(L, name, base) \
do \
{ \
    lua_getglobal((L), "class"); \
    lua_getglobal((L), #base); \
    lua_call((L), 1, 1); \
    lua_pushcfunction((L), name##_ctor); \
    lua_setfield((L), -2, "ctor"); \
    lua_setglobal((L), #name); \
} while (false)

#define M_LUA_PATCH_CLASS_WITH_FUNCS(L, name, patch) \
do \
{ \
    lua_getglobal((L), #name); \
    luaL_setfuncs((L), patch##_funcs, 0); \
    lua_pop(L, 1); \
} while (false)


int obj_sctor(lua_State* L);
int obj_ctor(lua_State* L);
int class_type_new(lua_State* L);
int g_class(lua_State* L);
int g_cast(lua_State* L);
int g_addTemplateAbility(lua_State* L);

int g_loadTableFromFile(lua_State* L);
int g_saveTableToFile(lua_State* L);

template <typename PTYPE>
PTYPE luaL_toobjptr(lua_State* L, int idx, PTYPE& ptr);

template <typename PTYPE>
void luaL_pushobjptr(lua_State* L, const char* name, const PTYPE _p);

CUnit* luaL_tounitptr(lua_State* L, int idx = 1);
CAbility* luaL_toabilityptr(lua_State* L, int idx);
int luaL_tounitid(lua_State* L, int idx);
int luaL_toabilityid(lua_State* L, int idx);

int MRObj_getId(lua_State* L);

int LevelExp_ctor(lua_State* L);
int LevelExp_setLevel(lua_State* L);
int LevelExp_getLevel(lua_State* L);
int LevelExp_setMaxLevel(lua_State* L);
int LevelExp_getMaxLevel(lua_State* L);
int LevelExp_addExp(lua_State* L);
int LevelExp_getExp(lua_State* L);
int LevelExp_setExpRange(lua_State* L);
int LevelExp_setLevelUpdate(lua_State* L);

int LevelUpdate_ctor(lua_State* L);
int LevelUpdate_updateExpRange(lua_State* L);
int LevelUpdate_onChangeLevel(lua_State* L);
int LevelUpdate_calcExp(lua_State* L);

int AttackData_ctor(lua_State* L);
int AttackData_setAttack(lua_State* L);
int AttackData_getAttack(lua_State* L);
int AttackData_setAttackType(lua_State* L);
int AttackData_getAttackType(lua_State* L);
int AttackData_setAttackValue(lua_State* L);
int AttackData_getAttackValue(lua_State* L);
int AttackData_addAttackBuff(lua_State* L);

int Unit_ctor(lua_State* L);
int Unit_setName(lua_State* L);
int Unit_getName(lua_State* L);
int Unit_setMaxHp(lua_State* L);
int Unit_getMaxHp(lua_State* L);
int Unit_setExMaxHp(lua_State* L);
int Unit_getExMaxHp(lua_State* L);
int Unit_getRealMaxHp(lua_State* L);
int Unit_setHp(lua_State* L);
int Unit_getHp(lua_State* L);
int Unit_isDead(lua_State* L);
int Unit_setForceByIndex(lua_State* L);
int Unit_setForce(lua_State* L);
int Unit_getForce(lua_State* L);
int Unit_setAlly(lua_State* L);
int Unit_getAlly(lua_State* L);
int Unit_isMyAlly(lua_State* L);
int Unit_isMyEnemy(lua_State* L);
int Unit_canEffect(lua_State* L);
int Unit_setBaseArmor(lua_State* L);
int Unit_getBaseArmor(lua_State* L);
int Unit_getRealArmorValue(lua_State* L);
int Unit_setExArmorValue(lua_State* L);
int Unit_getExArmorValue(lua_State* L);
int Unit_suspend(lua_State* L);
int Unit_isSuspended(lua_State* L);
int Unit_resume(lua_State* L);
int Unit_getBuffStackSize(lua_State* L);
int Unit_attack(lua_State* L);
int Unit_damaged(lua_State* L);
int Unit_attackLow(lua_State* L);
int Unit_damagedLow(lua_State* L);
int Unit_setRewardGold(lua_State* L);
int Unit_getRewardGold(lua_State* L);
int Unit_setRewardExp(lua_State* L);
int Unit_getRewardExp(lua_State* L);
int Unit_setAI(lua_State* L);
int Unit_say(lua_State* L);
int Unit_setGhost(lua_State* L);
int Unit_setEnergy(lua_State* L);
int Unit_getEnergy(lua_State* L);
int Unit_setVisible(lua_State* L);
int Unit_isVisible(lua_State* L);

int Unit_startDoing(lua_State* L);
int Unit_endDoing(lua_State* L);
int Unit_isDoingOr(lua_State* L);
int Unit_isDoingAnd(lua_State* L);
int Unit_isDoingNothing(lua_State* L);

int Unit_addActiveAbility(lua_State* L);
int Unit_addPassiveAbility(lua_State* L);
int Unit_addBuffAbility(lua_State* L);
int Unit_delActiveAbility(lua_State* L);
int Unit_delPassiveAbility(lua_State* L);
int Unit_delBuffAbility(lua_State* L);
int Unit_getAttackAbility(lua_State* L);
int Unit_getActiveAbility(lua_State* L);
int Unit_getBuffAbility(lua_State* L);

int Unit2D_setBaseMoveSpeed(lua_State* L);
int Unit2D_getBaseMoveSpeed(lua_State* L);
int Unit2D_getRealMoveSpeed(lua_State* L);
int Unit2D_setExMoveSpeed(lua_State* L);
int Unit2D_getExMoveSpeed(lua_State* L);
int Unit2D_getHalfOfWidth(lua_State* L);
int Unit2D_getHalfOfHeight(lua_State* L);
int Unit2D_setPosition(lua_State* L);
int Unit2D_getPosition(lua_State* L);
int Unit2D_getNearestEnemyInRange(lua_State* L);
int Unit2D_move(lua_State* L);
int Unit2D_moveAlongPath(lua_State* L);
int Unit2D_setCastTarget(lua_State* L);
int Unit2D_getCastTarget(lua_State* L);
int Unit2D_castSpellWithoutTarget(lua_State* L);
int Unit2D_castSpellWithTargetUnit(lua_State* L);
int Unit2D_castSpellWithTargetPoint(lua_State* L);
int Unit2D_stop(lua_State* L);
int Unit2D_setHostilityRange(lua_State* L);
int Unit2D_getHostilityRange(lua_State* L);
int Unit2D_setFixed(lua_State* L);
int Unit2D_isFixed(lua_State* L);
int Unit2D_isDoingCastingAction(lua_State* L);
int Unit2D_getDistance(lua_State* L);
int Unit2D_getTouchDistance(lua_State* L);
int Unit2D_getAttackingTarget(lua_State* L);
int Unit2D_doAnimation(lua_State* L);

int UnitPath_ctor(lua_State* L);
int UnitPath_addPoint(lua_State* L);
int UnitPath_getFirstPoint(lua_State* L);

int UnitAI_ctor(lua_State* L);
int UnitAI_onUnitChangeHp(lua_State* L);
int UnitAI_onUnitTick(lua_State* L);
int UnitAI_onUnitDamagedDone(lua_State* L);
int UnitAI_onUnitDamageTargetDone(lua_State* L);
int UnitAI_onUnitAddBuffAbility(lua_State* L);
int UnitAI_onUnitDelBuffAbility(lua_State* L);
int UnitAI_onUnitAbilityReady(lua_State* L);

int Projectile_ctor(lua_State* L);
int Projectile_setSrcUnit(lua_State* L);
int Projectile_setFromUnit(lua_State* L);
int Projectile_setToUnit(lua_State* L);
int Projectile_setFromPoint(lua_State* L);
int Projectile_setToPoint(lua_State* L);
int Projectile_setFromToType(lua_State* L);
int Projectile_setFireType(lua_State* L);
int Projectile_setPenaltyFlags(lua_State* L);
int Projectile_fire(lua_State* L);
int Projectile_redirect(lua_State* L);
int Projectile_die(lua_State* L);
int Projectile_setPosition(lua_State* L);
int Projectile_getPosition(lua_State* L);
int Projectile_setHeight(lua_State* L);
int Projectile_getHeight(lua_State* L);
int Projectile_setMoveSpeed(lua_State* L);
int Projectile_getMoveSpeed(lua_State* L);
int Projectile_setMaxHeightDelta(lua_State* L);
int Projectile_getMaxHeightDelta(lua_State* L);
int Projectile_setVisible(lua_State* L);
int Projectile_addFireSound(lua_State* L);
int Projectile_addEffectSound(lua_State* L);
int Projectile_setAttackData(lua_State* L);
int Projectile_getAttackData(lua_State* L);
int Projectile_setSrcAbility(lua_State* L);
int Projectile_setContactLeft(lua_State* L);
int Projectile_getContactLeft(lua_State* L);
int Projectile_decContactLeft(lua_State* L);

int Ability_ctor(lua_State* L);
int Ability_onChangeLevel(lua_State* L);
int Ability_onCopy(lua_State* L);
int Ability_onUnitAddAbility(lua_State* L);
int Ability_onUnitDelAbility(lua_State* L);
int Ability_onUnitAbilityReady(lua_State* L);
int Ability_onUnitRevive(lua_State* L);
int Ability_onUnitDying(lua_State* L);
int Ability_onUnitDead(lua_State* L);
int Ability_onUnitChangeHp(lua_State* L);
int Ability_onUnitTick(lua_State* L);
int Ability_onUnitInterval(lua_State* L);
int Ability_onUnitAttackTarget(lua_State* L);
int Ability_onUnitAttacked(lua_State* L);
int Ability_onUnitDamaged(lua_State* L);
int Ability_onUnitDamagedDone(lua_State* L);
int Ability_onUnitDamageTargetDone(lua_State* L);
int Ability_onUnitProjectileEffect(lua_State* L);
int Ability_onUnitProjectileArrive(lua_State* L);
int Ability_onUnitAbilityEffect(lua_State* L);
int Ability_onUnitCalcDamageTarget(lua_State* L);
int Ability_copy(lua_State* L);
int Ability_setTriggerFlags(lua_State* L);
int Ability_getName(lua_State* L);
int Ability_getOwner(lua_State* L);
int Ability_setInterval(lua_State* L);
int Ability_getInterval(lua_State* L);
int Ability_setCoolDown(lua_State* L);
int Ability_getCoolDown(lua_State* L);
int Ability_isCoolingDown(lua_State* L);
int Ability_resetCD(lua_State* L);
int Ability_coolDown(lua_State* L);
int Ability_setLevel(lua_State* L);
int Ability_getLevel(lua_State* L);
int Ability_addEffectSound(lua_State* L);
int Ability_setImageName(lua_State* L);
int Ability_setDescribe(lua_State* L);
int Ability_getDescribe(lua_State* L);
int Ability_setLevelInfo(lua_State* L);
int Ability_setGrade(lua_State* L);
int Ability_getGrade(lua_State* L);
int Ability_setCost(lua_State* L);
int Ability_getCost(lua_State* L);
int Ability_addCastAnimation(lua_State* L);

int ActiveAbility_ctor(lua_State* L);
int ActiveAbility_checkConditions(lua_State* L);
int ActiveAbility_onUnitCastAbility(lua_State* L);
int ActiveAbility_onUnitAbilityEffect(lua_State* L);
int ActiveAbility_setEffectiveTypeFlags(lua_State* L);
int ActiveAbility_getEffectiveTypeFlags(lua_State* L);
int ActiveAbility_setCastRange(lua_State* L);
int ActiveAbility_getCastRange(lua_State* L);
int ActiveAbility_setCastMinRange(lua_State* L);
int ActiveAbility_getCastMinRange(lua_State* L);
int ActiveAbility_setCastTargetRadius(lua_State* L);
int ActiveAbility_getCastTargetRadius(lua_State* L);
int ActiveAbility_setTemplateProjectile(lua_State* L);
int ActiveAbility_setCastHorizontal(lua_State* L);
int ActiveAbility_isCastHorizontal(lua_State* L);
int ActiveAbility_getAbilityEffectPoint(lua_State* L);

int PassiveAbility_ctor(lua_State* L);

int BuffAbility_ctor(lua_State* L);
int BuffAbility_onUnitDisplaceAbility(lua_State* L);
int BuffAbility_getSrcUnit(lua_State* L);
int BuffAbility_setAppendBuff(lua_State* L);
int BuffAbility_setDuration(lua_State* L);
int BuffAbility_getDuration(lua_State* L);

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

int BuffMakerAct_ctor(lua_State* L);
int BuffMakerAct_setTemplateBuff(lua_State* L);

int AttackBuffMakerPas_ctor(lua_State* L);
int AttackBuffMakerPas_setChance(lua_State* L);
int AttackBuffMakerPas_setTemplateBuff(lua_State* L);
int AttackBuffMakerPas_setExAttackValue(lua_State* L);
int AttackBuffMakerPas_setTemplateAct(lua_State* L);

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
int DamageBuff_ctor(lua_State* L);
int TransitiveLinkBuff_ctor(lua_State* L);
int TransitiveBlinkBuff_ctor(lua_State* L);
int SplashPas_ctor(lua_State* L);
int KnockBackBuff_ctor(lua_State* L);
int AttractBuff_ctor(lua_State* L);
int ReflectBuff_ctor(lua_State* L);
int LimitedLifeBuff_ctor(lua_State* L);
int ChargeJumpBuff_ctor(lua_State* L);
int LimitedPasBuff_ctor(lua_State* L);

int luaRegCommFuncs(lua_State* L);

int g_onWorldInit(lua_State* L);
int g_onWorldTick(lua_State* L);
int g_setControlUnit(lua_State* L);
int g_getControlUnit(lua_State* L);
int g_getUnit(lua_State* L);
int g_getUnits(lua_State* L);
int g_addUnit(lua_State* L);
int g_addProjectile(lua_State* L);

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld);




///////////////////////////////// Inline //////////////////////////////////////
template <typename PTYPE>
PTYPE luaL_toobjptr(lua_State* L, int idx, PTYPE& ptr)
{
    if (lua_istable(L, idx) == false)
    {
        return nullptr;
    }

    lua_getfield(L, idx, "_p");
    ptr = (PTYPE)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ptr;
}

template <typename PTYPE>
void luaL_pushobjptr(lua_State* L, const char* name, const PTYPE _p)
{
    if (_p == nullptr)
    {
        lua_pushnil(L);
        return;
    }

    auto mo = DCAST(_p, CMultiRefObject*);
    if (mo != nullptr && mo->getScriptHandler() != 0)
    {
        luaL_getregistery(L, mo->getScriptHandler());
    }
    else
    {
        lua_newtable(L);
        int obj = lua_gettop(L);

        lua_getglobal(L, name);
        lua_setmetatable(L, obj);

        lua_pushlightuserdata(L, _p);
        lua_setfield(L, obj, "_p");
    }
}

#endif  /* __LUABINDING_H__ */

