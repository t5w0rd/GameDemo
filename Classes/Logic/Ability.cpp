/* 
 * File:   Ability.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午11:45
 */

#include "CommInc.h"
#include "Unit.h"
#include "Ability.h"
#include "MultiRefObject.h"
#include "Application.h"
#include "Draw.h"
#include "LuaBinding.h"
#include "LuaScriptEngine.h"
#include "GameControl.h"
#include "AbilityLibrary.h"


// CAbility
CAbility::CAbility(const char* pRootId, const char* pName, float fCoolDown)
: CONST_ROOT_ID(pRootId)
, m_iScriptHandler(0)
, m_sName(pName)
, m_pOwner(nullptr)
, m_fCoolDown(fCoolDown)
, m_fCoolingDownElapsed(FLT_MAX)
, m_fInterval(0.0f)
, m_fIntervalElapsed(0.0f)
, m_dwTriggerFlags(0)

, m_iGrade(kNormal)
, m_iCost(0)
, m_bTemporary(false)
{
    setDbgClassName("CAbility");
}

CAbility::~CAbility()
{
    if (getScriptHandler() != 0)
    {
        lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
        luaL_unref(L, LUA_REGISTRYINDEX, getScriptHandler());
    }
}

const char* CAbility::getDbgTag() const
{
    return getName();
}

CAbility* CAbility::copy()
{
    CAbility* ret = new CAbility(getRootId(), getName(), getCoolDown());
    ret->copyData(this);
    ret->onCopy(this);
    return ret;
}

void CAbility::copyData(CAbility* from)
{
    // TODO: copy some members which are not in the constuctor params
    copyScriptHandler(from->getScriptHandler());
    m_dwTriggerFlags = from->m_dwTriggerFlags;
    setInterval(from->getInterval());
    m_vecCastAnis = from->m_vecCastAnis;
    m_vecEffectSounds = from->m_vecEffectSounds;
    m_sImageName = from->m_sImageName;
    m_iCost = from->m_iCost;
    m_iGrade = from->m_iGrade;
    m_sDescribe = from->m_sDescribe;
    m_vecLevelDescribe = from->m_vecLevelDescribe;
    m_vecLevelType = from->m_vecLevelType;
    m_bTemporary = from->m_bTemporary;

    m_iBaseExp = from->m_iBaseExp;
    m_iMaxExp = from->m_iMaxExp;
    m_iExp = from->m_iExp;
    m_pUpdate = from->m_pUpdate ? from->m_pUpdate->copy() : nullptr;
    m_iMaxLvl = from->m_iMaxLvl;
    //setLevel(from->getLevel());
}

void CAbility::copyScriptHandler(int iScriptHandler)
{
    if (iScriptHandler == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();

    // copy source lua obj
    lua_getglobal(L, "table");
    lua_getfield(L, -1, "copy");
    luaL_getregistery(L, iScriptHandler);
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
        lua_pushlightuserdata(L, this);
        lua_setfield(L, -2, "_p");
        setScriptHandler(luaL_ref(L, LUA_REGISTRYINDEX));
    }

    lua_pop(L, 1);  // pop "table"
}

const char* CAbility::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

float CAbility::getRealCoolDown() const
{
    return getCoolDown();
}

bool CAbility::isCoolingDown() const
{
    return m_fCoolingDownElapsed < getRealCoolDown();
}

void CAbility::resetCD()
{
    m_fCoolingDownElapsed = FLT_MAX;
    getOwner()->updateAbilityCD(getId());
}

void CAbility::coolDown()
{
    setCoolingDownElapsed(0.0f);
    getOwner()->abilityCD(this);
}

void CAbility::setInterval(float fInterval)
{
    if (fInterval <= FLT_EPSILON)
    {
        m_fInterval = 0.0f;
        return;
    }
    
    setTriggerFlags(CUnit::kOnTickTrigger);
    m_fInterval = fInterval;
}

void CAbility::addCastAnimation(int id)
{
    m_vecCastAnis.push_back(id);
}

int CAbility::getCastRandomAnimation() const
{
    if (m_vecCastAnis.empty())
    {
        return -1;
    }

    return m_vecCastAnis[rand() % m_vecCastAnis.size()];
}

void CAbility::onChangeLevel(int iChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onChangeLevel");
    lua_pushvalue(L, a);
    lua_pushinteger(L, iChanged);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onCopy(CAbility* from)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onCopy");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "Ability", from);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitAddAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());
    
    lua_getfield(L, a, "onUnitAddAbility");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDelAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());
    
    lua_getfield(L, a, "onUnitDelAbility");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitAbilityReady()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAbilityReady");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitRevive()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitRevive");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDying()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDying");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDead()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDead");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitChangeHp(float fChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitChangeHp");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fChanged);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitTick(float dt)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitTick");
    lua_pushvalue(L, a);
    lua_pushnumber(L, dt);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitInterval()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitInterval");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttackTarget");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pTarget);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

bool CAbility::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return true;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttacked");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pSource);

    bool res = true;
    if (lua_pcall(L, 3, 1, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    else
    {
        res = (lua_toboolean(L, -1) != 0);
    }

    lua_pop(L, 2);  // pop 'a' and ret

    return res;
}

void CAbility::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamaged");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pSource);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDamagedDone(float fDamage, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamagedDone");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pSource);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamageTargetDone");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pTarget);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitProjectileEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitProjectileEffect");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "Projectile", pProjectile);
    luaL_pushobjptr(L, "Unit", pTarget);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1);  // pop 'a'
}

bool CAbility::onUnitProjectileArrive(CProjectile* pProjectile)
{
    if (getScriptHandler() == 0)
    {
        return true;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitProjectileArrive");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "Projectile", pProjectile);

    bool res = true;
    if (lua_pcall(L, 2, 1, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    else
    {
        res = (lua_toboolean(L, -1) != 0);
    }

    lua_pop(L, 2);  // pop 'a' and ret

    return res;
}

void CAbility::onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAbilityEffect");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "Projectile", pProjectile);
    luaL_pushobjptr(L, "Unit", pTarget);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onAddToUnit(CUnit* pOwner)
{
    setOwner(pOwner);
    onUnitAddAbility();
}

void CAbility::onDelFromUnit()
{
    onUnitDelAbility();
    setOwner(nullptr);
}

void CAbility::setTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags |= dwTriggerFlags;
}

void CAbility::unsetTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags &= ~dwTriggerFlags;
}

void CAbility::addEffectSound(const char* pSounds)
{
    m_vecEffectSounds.push_back(pSounds);
}

void CAbility::playEffectSound()
{
    if (m_vecEffectSounds.empty())
    {
        return;
    }
    // !!!!!
    M_DEF_GC(gc);
    gc->playSound(m_vecEffectSounds[rand() % m_vecEffectSounds.size()].c_str());
}

const char* CAbility::getLevelDescribe()
{
    return getLevelDescribe(getLevel());
}

const char* CAbility::getLevelDescribe(int level)
{
    if (level < 1 || level > (int)m_vecLevelDescribe.size())
    {
        return nullptr;
    }

    return m_vecLevelDescribe[level - 1].c_str();
}

int CAbility::getLevelType()
{
    return getLevelType(getLevel());
}

int CAbility::getLevelType(int level)
{
    if (level < 1 || level > (int)m_vecLevelType.size())
    {
        return -1;
    }

    return m_vecLevelType[level - 1];
}

void CAbility::setLevelInfo(int level, int type, const char* desc)
{
    if (level < 1)
    {
        return;
    }

    for (auto i = (int)m_vecLevelDescribe.size(); i < level; ++i)
    {
        m_vecLevelDescribe.push_back(string());
        m_vecLevelType.push_back(int());
    }

    m_vecLevelDescribe[level - 1] = desc;
    m_vecLevelType[level - 1] = type;
}

// CActiveAbility
CActiveAbility::CActiveAbility(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags)
: CAbility(pRootId, pName, fCoolDown)
, m_eCastTargetType(eCastType)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
, m_fCastMinRange(0.0f)
, m_fCastRange(0.0f)
, m_fCastTargetRadius(0.0f)
, m_iTemplateProjectile(0)
, m_bCastHorizontal(false)
{
    setDbgClassName("CActiveAbility");
}

CActiveAbility::~CActiveAbility()
{
}

CActiveAbility* CActiveAbility::copy()
{
    CActiveAbility* ret = new CActiveAbility(getRootId(), getName(), m_fCoolDown, m_eCastTargetType, m_dwEffectiveTypeFlags);
    ret->copyData(this);
    return ret;
}

void CActiveAbility::copyData(CAbility* from)
{
    CAbility::copyData(from);
    CActiveAbility* a = DCAST(from, CActiveAbility*);
    setCastTargetType(a->getCastTargetType());
    setEffectiveTypeFlags(a->getEffectiveTypeFlags());
    setCastMinRange(a->getCastMinRange());
    setCastRange(a->getCastRange());
    setCastTargetRadius(a->getCastTargetRadius());
    setTemplateProjectile(a->getTemplateProjectile());
    setCastHorizontal(a->isCastHorizontal());
}


const float CActiveAbility::CONST_MAX_CAST_BUFFER_RANGE = 50.0f;
const float CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE = 5.0f;

bool CActiveAbility::cast()
{
    if (isCoolingDown() == true)
    {
        return false;
    }

    CUnit* o = getOwner();
    LOG("%s%s%s..", o->getName(), o->getAttackAbilityId() == getId() ? "的" : "施放了", getName());
    coolDown();
    onUnitCastAbility();  // onCastAbility在cd变化下面，所以可以添加重置cd的逻辑
    
    return true;
}

bool CActiveAbility::checkConditions(const CCommandTarget& rTarget)
{
    if (getScriptHandler() == 0)
    {
        return true;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "checkConditions");
    lua_pushvalue(L, a);
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

    bool res = false;
    if (lua_pcall(L, 2, 1, 0) != LUA_OK)
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

    lua_pop(L, 2);

    return res;
}

void CActiveAbility::onUnitCastAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitCastAbility");
    lua_pushvalue(L, a);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void CActiveAbility::effect()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    CWorld* w = o->getWorld();
    // Ability
    
    coolDown();

    onUnitCastAbility();

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        playEffectSound();
        onUnitAbilityEffect(nullptr, nullptr);
        break;

    case CCommandTarget::kUnitTarget:
        {
            CUnit* t = o->getUnit(d->getCastTarget().getTargetUnit());

            if (t == nullptr || t->isDead())
            {
                return;
            }

            if (getTemplateProjectile() != 0)
            {
                
                CProjectile* p = w->copyProjectile(getTemplateProjectile());
                w->addProjectile(p);
                p->setSrcUnit(o->getId());
                p->setSrcAbility(this);
                p->setEffectiveTypeFlags(getEffectiveTypeFlags());

                CUnit* t = w->getUnit(d->getCastTarget().getTargetUnit());
                CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
                assert(td != nullptr);

                p->setFromToType(CProjectile::kUnitToUnit);
                p->setFromUnit(o->getId());
                p->setToUnit(t->getId());

                p->fire();
            }
            else
            {
                playEffectSound();
                onUnitAbilityEffect(nullptr, t);
            }
        }
        break;

    case CCommandTarget::kPointTarget:
        if (getTemplateProjectile() != 0)
        {
            CProjectile* p = w->copyProjectile(getTemplateProjectile());
            w->addProjectile(p);
            p->setSrcUnit(o->getId());
            p->setSrcAbility(this);
            p->setEffectiveTypeFlags(getEffectiveTypeFlags());

            p->setFromToType(CProjectile::kUnitToPoint);
            p->setFromUnit(o->getId());
            p->setToPoint(d->getPosition().getForwardPoint(d->getCastTarget().getTargetPoint(), getCastRange()));

            p->fire();
        }
        else
        {
            playEffectSound();
            onUnitAbilityEffect(nullptr, nullptr);
        }

        break;
    }

    
#if 1
    // onCastAbility在cd变化下面，所以可以添加重置cd的逻辑
#else
    ad->retain();
    TEST_ATTACK_INFO* pAi = new TEST_ATTACK_INFO;
    pAi->iTarget = t->getId();
    pAi->pAttackData = ad;
    o->runAction(new CCallFunc(this, (FUNC_CALLFUNC_ND)&CAttackAct::onTestAttackEffect, pAi));
#endif
}

const CPoint& CActiveAbility::getAbilityEffectPoint(CProjectile* pProjectile, CUnit* pTarget)
{
    CUnitDraw2D* od = DCAST(getOwner()->getDraw(), CUnitDraw2D*);
    const CPoint& ret = ((pProjectile != nullptr) ? pProjectile->getPosition() : (getCastTargetType() == CCommandTarget::kNoTarget ? od->getPosition() : od->getCastTarget().getTargetPoint()));
    return ret;
}

// CPassiveAbility
CPassiveAbility::CPassiveAbility(const char* pRootId, const char* pName, float fCoolDown)
: CAbility(pRootId, pName, fCoolDown)
{
    setDbgClassName("CPassiveAbility");
}

CPassiveAbility::~CPassiveAbility()
{
}

CPassiveAbility* CPassiveAbility::copy()
{
    CPassiveAbility* ret = new CPassiveAbility(getRootId(), getName(), m_fCoolDown);
    ret->copyData(this);
    return ret;
}

// CBuffAbility
CBuffAbility::CBuffAbility(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CAbility(pRootId, pName, 0.0f)
, m_fDuration(fDuration)
, m_fElapsed(0.0f)
, m_bStackable(bStackable)
, m_iSrcUnit(0)
, m_iAppendBuff(0)
{
    setDbgClassName("CBuffAbility");
}

CBuffAbility* CBuffAbility::copy()
{
    CBuffAbility* ret = new CBuffAbility(getRootId(), getName(), m_fDuration, m_bStackable);
    ret->copyData(this);
    return ret;
}

void CBuffAbility::copyData(CAbility* from)
{
    CAbility::copyData(from);
    CBuffAbility* a = DCAST(from, CBuffAbility*);
    setSrcUnit(a->getSrcUnit());
    setAppendBuff(a->getAppendBuff());
}

void CBuffAbility::onUnitDisplaceAbility(CBuffAbility* newBuff)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDisplaceAbility");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "BuffAbility", newBuff);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'a'
}

bool CBuffAbility::isDone() const
{
    return m_fElapsed >= m_fDuration;
}

// CAttackAct
const float CAttackAct::CONST_MIN_ATTACK_SPEED_INTERVAL = 0.1f; // 0.1s
const float CAttackAct::CONST_MIN_ATTACK_SPEED_MULRIPLE = 0.2f; // 20%
const float CAttackAct::CONST_MAX_ATTACK_SPEED_MULRIPLE = 5.0f; // 500%

CAttackAct::CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange)
: CActiveAbility(pRootId, pName, fCoolDown, CCommandTarget::kUnitTarget, CUnitForce::kEnemy)
, m_oAttackValue(rAttackValue)
, m_fAttackValueRandomRange(fAttackValueRandomRange)
{
    setDbgClassName("CAttackAct");
}

CAttackAct* CAttackAct::copy()
{
    CAttackAct* pRet = new CAttackAct(getRootId(), getName(), m_fCoolDown, m_oAttackValue, m_fAttackValueRandomRange);
    pRet->copyData(this);
    return pRet;
}

void CAttackAct::onUnitAddAbility()
{
    getOwner()->setAttackAbilityId(getId());
}

void CAttackAct::onUnitDelAbility()
{
    getOwner()->setAttackAbilityId(0);
}

bool CAttackAct::checkConditions(const CCommandTarget& rTarget)
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != nullptr);

    CUnit* t = o->getUnit(rTarget.getTargetUnit());
    if (t == nullptr || t->isDead())
    {
        return false;
    }
    
    return true;
}

void CAttackAct::onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    CUnit* o = getOwner();
    if (o == nullptr)
    {
        return;
    }

    if (pProjectile != nullptr)
    {
        pProjectile->decContactLeft();
    }

    CAttackData* ad = new CAttackData();
    ad->setAttackValue(getBaseAttack().getType(), getRealAttackValue());

    o->attack(ad, pTarget);

    if (pTarget != nullptr)
    {
        pTarget->damaged(ad, o);
    }
}

float CAttackAct::getBaseAttackValue() const
{
    return m_oAttackValue.getValue();
}

float CAttackAct::getRealAttackValue(bool bUseRandomRange) const
{
    if (bUseRandomRange)
    {
        float fAttackValueRandomRange = m_oExAttackValueRandomRange.getValue(m_fAttackValueRandomRange);
        if (fAttackValueRandomRange > 0.001)
        {
            //return m_aoExAttackValue.getValue(m_oAttackValue.getValue()) * (1 - fAttackValueRandomRange * 0.5 + (rand() % (int)(fAttackValueRandomRange * 1000)) * 0.001);
            return randValue(m_aoExAttackValue.getValue(m_oAttackValue.getValue()), fAttackValueRandomRange);
        }
    }
    return m_aoExAttackValue.getValue(m_oAttackValue.getValue());
}

float CAttackAct::getRealCoolDown() const
{
    return getRealAttackInterval();
}

void CAttackAct::setBaseAttackInterval(float fAttackInterval)
{
    m_fCoolDown = max(fAttackInterval, CONST_MIN_ATTACK_SPEED_INTERVAL);
}

float CAttackAct::getBaseAttackInterval() const
{
    return getCoolDown();
}

float CAttackAct::getBaseAttackSpeed() const
{
    return 1 / max(getBaseAttackInterval(), FLT_EPSILON);
}

float CAttackAct::getRealAttackInterval() const
{
    float fAttackInterval = getBaseAttackInterval();
    // 取攻击速度系数，不得小于最小值
    float fMulriple = m_oExAttackSpeed.getMulriple();
    fMulriple = max(fMulriple, CONST_MIN_ATTACK_SPEED_MULRIPLE);
    // 计算两种最短攻击间隔中的最大值作为最短攻击间隔
    float fMinAttackSpeedInterval = fAttackInterval / CONST_MAX_ATTACK_SPEED_MULRIPLE;
    fMinAttackSpeedInterval = max(CONST_MIN_ATTACK_SPEED_INTERVAL, fMinAttackSpeedInterval);
    // 计算实际攻击间隔，不得小于上述最短攻击间隔
    float fRealAttackInterval = fAttackInterval / fMulriple;
    return max(fRealAttackInterval, fMinAttackSpeedInterval);
}

float CAttackAct::getRealAttackSpeed() const
{
    return 1 / getRealAttackInterval();
}

void CAttackAct::setExAttackSpeed(const CExtraCoeff& roExAttackSpeed)
{
    m_oExAttackSpeed = roExAttackSpeed;  // 必须保留原值，尽管有可能超出范围，否则技能删除后无法恢复
    updateAttackSpeed();
}

const CExtraCoeff& CAttackAct::getExAttackSpeed() const
{
    return m_oExAttackSpeed;
}

void CAttackAct::updateAttackSpeed()
{
    CUnit* o = getOwner();
    assert(o != nullptr);
    o->updateAbilityCD(getId());
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    if (d->getCastActiveAbilityId() == o->getAttackAbilityId())
    {
        float spd = getBaseAttackInterval() / max(FLT_EPSILON, getRealAttackInterval());
        LOG("ATK SPD: %.1f", spd);
        d->setActionSpeed(d->getCastActionId(), spd);
    }
    
}

void CAttackAct::onTestAttackEffect(CMultiRefObject* pObj, void* pData)
{
    TEST_ATTACK_INFO* pAi = (TEST_ATTACK_INFO*)pData;
    
    CUnit* o = getOwner();
    CUnit* t = o->getUnit(pAi->iTarget);
    CAttackData* ad = pAi->pAttackData;
    
    if (t != nullptr && t->isDead() == false)
    {
        t->damaged(ad, o);
    }
    
    ad->release();
    
    delete pAi;
}

// CBuffMakerAct
CBuffMakerAct::CBuffMakerAct(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags, float fChance, int iTemplateBuff)
: CActiveAbility(pRootId, pName, fCoolDown, eCastType, dwEffectiveTypeFlags)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
, m_bEffectFixed(false)
, m_bEffectDead(false)
, m_pTarget(nullptr)
{
    setDbgClassName("CBuffMakerAct");
}

CBuffMakerAct* CBuffMakerAct::copy()
{
    CBuffMakerAct* pRet = new CBuffMakerAct(getRootId(), getName(), getCoolDown(), getCastTargetType(), getEffectiveTypeFlags(), getChance(), getTemplateBuff());
    pRet->copyData(this);
    return pRet;
}

bool CBuffMakerAct::checkConditions(const CCommandTarget& rTarget)
{
    m_pTarget = nullptr;
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != nullptr);

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        m_pTarget = o;
        break;
        
    case CCommandTarget::kUnitTarget:
        m_pTarget = o->getUnit(rTarget.getTargetUnit());
        if (m_pTarget != nullptr && (m_pTarget->isDead()))
        {
            m_pTarget = nullptr;
        }
        break;
        
    default:
        ;
    }
    
    if (m_pTarget != nullptr &&
        !o->canEffect(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags()))
    {
        // 如果有待选目标(自身或命令目标)，但是无法作用
        if (getCastTargetRadius() <= FLT_EPSILON ||
            getEffectiveTypeFlags() == CUnitForce::kSelf)
        {
            // 如果不能扩展周围单位
            return false;
        }
    }
    
    return true;
}

void CBuffMakerAct::onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    if (pTarget != nullptr)
    {
        // 抛射物被反弹后，m_pTarget需要更新
        m_pTarget = pTarget;
    }
    
    CUnit* o = getOwner();

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
    case CCommandTarget::kUnitTarget:

        if (M_RAND_HIT(m_fChance) && (pProjectile != nullptr || o->canEffect(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags())))
        {
            m_pTarget->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }
        break;

    default:
        ;
    }

    CWorld* w = o->getWorld();
    if (pProjectile != nullptr && pProjectile->hasPenaltyType(CProjectile::kOnContact) && pTarget != nullptr)
    {
        // 接触型抛射物，接触单位
        if (M_RAND_HIT(m_fChance) && o->canEffect(DCAST(pTarget, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            CBuffAbility* pBuff = DCAST(w->copyAbility(getTemplateBuff()), CBuffAbility*);
            pBuff->setSrcUnit(o->getId());
            pBuff->setLevel(getLevel());
            pTarget->addBuffAbility(pBuff);
            pProjectile->decContactLeft();
        }
        return;
    }

    if (getCastTargetRadius() <= FLT_EPSILON)
    {
        return;
    }

    CUnitDraw2D* od  = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != nullptr);
    CBuffAbility* pBuff = nullptr;
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;

        if (M_RAND_HIT(m_fChance) == false || u == nullptr || u->isDead() || u == m_pTarget)
        {
            continue;
        }

        if (!o->canEffect(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud  = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != nullptr);
        
        const CPoint& p = getAbilityEffectPoint(pProjectile, pTarget);
        if (ud->getPosition().getDistance(p) - ud->getHalfOfWidth() > getCastTargetRadius())
        {
            continue;
        }

        if (pBuff == nullptr)
        {
            w->copyAbility(getTemplateBuff())->dcast(pBuff);
            pBuff->setSrcUnit(o->getId());
            pBuff->setLevel(getLevel());
        }
        else
        {
            pBuff->copy()->dcast(pBuff);
            // !!! copy is not work well
            pBuff->setSrcUnit(o->getId());
            pBuff->setLevel(getLevel());
        }

        u->addBuffAbility(pBuff);
    }
}

// CAuraPas
CAuraPas::CAuraPas(const char* pRootId, const char* pName, float fInterval, int iTemplateBuff, float fRange, uint32_t dwEffectiveTypeFlags, bool bEffectFixed)
: CPassiveAbility(pRootId, pName)
, m_iTemplateBuff(iTemplateBuff)
, m_fRange(fRange)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
, m_fEffectCD(0.0f)
, m_bEffectFixed(bEffectFixed)
{
    setDbgClassName("CAuraPas");
    setInterval(fInterval);
}

CAuraPas::~CAuraPas()
{
}

CAuraPas* CAuraPas::copy()
{
    return new CAuraPas(getRootId(), getName(), m_fInterval, m_iTemplateBuff, m_fRange, m_dwEffectiveTypeFlags, m_bEffectFixed);
}

void CAuraPas::onUnitInterval()
{
    CUnit* o = getOwner();
    if (o->isDead())
    {
        return;
    }

    CWorld* w = o->getWorld();
    CBuffAbility* pBuff = nullptr;
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != nullptr);
    
#if 0
    // DemoTemp
    m_fEffectCD += getInterval();
    if (m_fEffectCD > 2.0f)
    {
        m_fEffectCD = 0.0f;
        Sprite* sp = Sprite::createWithSpriteFrameName("UI/cmd/Target2.png");
        Node* sn = DCAST(o->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow();
        sn->addChild(sp);
        sp->setColor(Color3B(236, 156, 85));
        sp->setPosition(Point(sn->getContentSize().width * sn->getAnchorPoint().x, sp->getContentSize().height * 0.5));
        sp->runAction(Sequence::create(Spawn::create(ScaleTo::create(1.0f, 3.5f, 3.5f), FadeOut::create(1.0f), nullptr), RemoveSelf::create(true), nullptr));
        
        sp = Sprite::createWithSpriteFrameName("UI/cmd/Target.png");
        sn->addChild(sp);
        sp->setColor(Color3B(236, 156, 85));
        sp->setPosition(Point(sn->getContentSize().width * sn->getAnchorPoint().x, sp->getContentSize().height * 0.5));
        sp->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(ScaleTo::create(1.0f, 3.5f, 3.5f), FadeOut::create(1.0f), nullptr), RemoveSelf::create(true), nullptr));
    }
#endif
    
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;
        
        if (u == nullptr || u->isDead())
        {
            continue;
        }
        
        if (!o->canEffect(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != nullptr);
        if (ud->getPosition().getDistance(od->getPosition()) > m_fRange || (!isEffectFixed() && ud->isFixed()))
        {
            continue;
        }
        
        if (pBuff == nullptr)
        {
            w->copyAbility(m_iTemplateBuff)->dcast(pBuff);
        }
        else
        {
            pBuff = pBuff->copy();
        }
        
        pBuff->setSrcUnit(o->getId());
        pBuff->setLevel(getLevel());
        u->addBuffAbility(pBuff);
    }
}

// CAttackBuffMakerPas
CAttackBuffMakerPas::CAttackBuffMakerPas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff, bool bToSelf, const CExtraCoeff& roExAttackValue, int iTemplateAct)
: CPassiveAbility(pRootId, pName)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
, m_bToSelf(bToSelf)
, m_oExAttackValue(roExAttackValue)
, m_iTemplateAct(iTemplateAct)
, m_pInnerAct(nullptr)
{
    setDbgClassName("CAttackBuffMakerPas");
    setTriggerFlags(CUnit::kOnAttackTargetTrigger);
}

CAttackBuffMakerPas::~CAttackBuffMakerPas()
{
    //M_SAFE_RELEASE(m_pInnerAct);
}

CAttackBuffMakerPas* CAttackBuffMakerPas::copy()
{
    CAttackBuffMakerPas* ret = new CAttackBuffMakerPas(getRootId(), getName(), m_fChance, m_iTemplateBuff, m_bToSelf, m_oExAttackValue);
    ret->setCoolDown(getCoolDown());
    ret->setTemplateAct(getTemplateAct());
    return ret;
}

void CAttackBuffMakerPas::onUnitAddAbility()
{
    if (m_iTemplateAct == 0)
    {
        return;
    }

    m_pInnerAct = DCAST(CAbilityLibrary::instance()->copyAbility(m_iTemplateAct), CActiveAbility*);
    if (m_pInnerAct == nullptr)
    {
        return;
    }

    m_pInnerAct->setTemporary();
    m_pInnerAct->setLevel(getLevel());
    auto o = getOwner();
    o->addActiveAbility(m_pInnerAct, false);
}

void CAttackBuffMakerPas::onUnitDelAbility()
{
    if (m_pInnerAct == nullptr)
    {
        return;
    }

    auto o = getOwner();
    o->delActiveAbility(m_pInnerAct->getId(), false);
    
    //M_SAFE_RELEASE(m_pInnerAct);
    m_pInnerAct = nullptr;
}

void CAttackBuffMakerPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    if (M_RAND_HIT(m_fChance) == false)
    {
        return;
    }
    
    pAttack->getAttackValue().setValue(m_oExAttackValue.getValue(pAttack->getAttackValue().getValue()));
    
    if (m_iTemplateBuff != 0)
    {
        if (isToSelf())
        {
            CUnit* o = getOwner();
            o->addBuffAbility(m_iTemplateBuff, o->getId(), getLevel());
        }
        else
        {
            pAttack->addAttackBuff(CAttackBuff(m_iTemplateBuff, getLevel()));
        }
    }

    if (m_pInnerAct != nullptr)
    {
        castInnerSpell(pTarget);
    }
}

bool CAttackBuffMakerPas::castInnerSpell(CUnit* pTarget)
{
    assert(m_pInnerAct != nullptr);

    if (m_pInnerAct->isCoolingDown())
    {
        return false;
    }
    
    auto o = getOwner();
    auto od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != nullptr);

    CUnitDraw2D* td = pTarget ? DCAST(pTarget->getDraw(), CUnitDraw2D*) : nullptr;
    CCommandTarget cmdTarget;
    switch (m_pInnerAct->getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        cmdTarget.setTarget();

        break;

    case CCommandTarget::kUnitTarget:
        if (pTarget == nullptr)
        {
            // 目标不存在
            return false;
        }

        cmdTarget.setTarget(pTarget->getId());

        break;

    case CCommandTarget::kPointTarget:
        if (pTarget == nullptr)
        {
            // 目标不存在
            return false;
        }

        assert(td != nullptr);
        cmdTarget.setTarget(td->getPosition());

        break;
    }

    if (m_pInnerAct->checkConditions(cmdTarget) == false)
    {
        return false;
    }

    // 施法距离合法性
    bool disOk = od->checkCastTargetDistance(m_pInnerAct, od->getPosition(), cmdTarget, td);
    if (disOk == false)
    {
        return false;
    }

    m_pInnerAct->effect();

    return true;
}

// CDamageBuff
CDamageBuff::CDamageBuff(const char* pName, const CAttackValue& rDamage, float fChance, bool bToSelf /*= false*/, const CExtraCoeff& roExAttackValue /*= CExtraCoeff()*/, uint32_t dwTriggerMask /*= CUnit::kMaskActiveTrigger*/, bool bAttack /*= false*/)
: CBuffAbility("DBM", pName, 0.0f, true)
, m_oDamage(rDamage)
, m_fChance(fChance)
, m_bToSelf(bToSelf)
, m_oExAttackValue(roExAttackValue)
, m_dwTriggerMask(dwTriggerMask)
, m_bAttack(bAttack)
{
    setDbgClassName("CDamageBuff");
}

CDamageBuff* CDamageBuff::copy()
{
    CDamageBuff* ret = new CDamageBuff(getName(), getDamage(), getChance(), isToSelf(), getExAttackValue(), m_dwTriggerMask, m_bAttack);
    ret->copyData(this);
    return ret;
}

void CDamageBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnit* s = o->getUnit(getSrcUnit());
    //CUnit* sd = DCAST(s->getDraw(), CUnitDraw2D*);
    
    
    CAttackData* ad = new CAttackData;
    if (getDamage().getValue() == 0.0f)
    {
        CAttackAct* atk = nullptr;
        s->getActiveAbility(s->getAttackAbilityId())->dcast(atk);
        if (atk != nullptr)
        {
            ad->setAttackValue(atk->getBaseAttack().getType(), getExAttackValue().getValue(atk->getRealAttackValue()));
        }
    }
    else
    {
        ad->setAttackValue(getDamage());
    }
    
    if (M_RAND_HIT(getChance()))
    {
        if (isToSelf())
        {
            if (s != nullptr && !s->isDead())
            {
                s->addBuffAbility(getAppendBuff(), getSrcUnit(), getLevel());
            }
        }
        else
        {
            //ad->addAttackBuff(CAttackBuff(getTemplateBuff(), getLevel()));
        }
        
    }

    if (m_bAttack && s != nullptr)
    {
        s->attack(ad, o, m_dwTriggerMask);
    }
    o->damaged(ad, s, m_dwTriggerMask);
}

// CVampirePas
CVampirePas::CVampirePas(const char* pRootId, const char* pName, float fPercentConversion)
: CPassiveAbility(pRootId, pName)
, m_fPercentConversion(fPercentConversion)
{
    setDbgClassName("CVampirePas");
    setTriggerFlags(CUnit::kOnDamageTargetDoneTrigger);
}

CVampirePas* CVampirePas::copy()
{
    return new CVampirePas(getRootId(), getName(), getPercentConversion());
}

void CVampirePas::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    float fDtHp = fDamage * getPercentConversion();
    o->setHp(o->getHp() + fDtHp);
    LOG("%s恢复%d点HP", o->getName(), toInt(fDtHp));

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnit* u = getOwner();
    CUnitDrawForCC* d = nullptr;
    u->getDraw()->dcast(d);

    if (d != nullptr)
    {
        char sz[64];
        sprintf(sz, "+%d", toInt(fDtHp));
        d->addBattleTip(sz, "", 18, Color3B(113, 205, 44));
    }
#endif
}

// CStunBuff
CStunBuff::CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
{
    setDbgClassName("CStunBuff");
}

CStunBuff* CStunBuff::copy()
{
    CStunBuff* ret = new CStunBuff(getRootId(), getName(), m_fDuration, m_bStackable);
    ret->copyData(this);
    return ret;
}

void CStunBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != nullptr);
    if (d->isFixed())
    {
        setDuration(0.0f);
        return;
    }
    d->cmdStop();
    o->suspend();
    
    LOG("%s%s中", o->getName(), getName());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = nullptr;
    o->getDraw()->dcast(ccd);

    if (ccd != nullptr)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "", 18, Color3B(250, 104, 16));

        M_DEF_GC(gc);

        Node* sn = DCAST(o->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow();
        Sprite* sp = DCAST(sn->getChildByTag(4242), Sprite*);
        if (sp == nullptr)
        {
            sp = Sprite::createWithSpriteFrameName("Effects/Stun/Big/00.png");
            sn->addChild(sp, M_BASE_Z - sn->getPosition().y, 4242);
        }
        else
        {
            sp->stopAllActions();
        }

        Animation* pAni = gc->getAnimation("Effects/Stun/Big");
        pAni->setLoops(2);
        
        sp->setPosition(Point(sn->getContentSize().width * sn->getAnchorPoint().x, sp->getContentSize().height * 0.5));
        sp->runAction(Sequence::create(Animate::create(pAni), RemoveSelf::create(), nullptr));
    }
#endif
}

void CStunBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != nullptr);
    if (d->isFixed())
    {
        return;
    }

    o->resume();
    
    if (!o->isSuspended())
    {
        LOG("%s不在%s", o->getName(), getName());

        Node* sn = DCAST(o->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow();
        Sprite* sp = DCAST(sn->getChildByTag(4242), Sprite*);
        if (sp != nullptr)
        {
            sp->stopAllActions();
            sp->removeFromParentAndCleanup(true);
        }
    }
}

// CDoubleAttackPas
CDoubleAttackPas::CDoubleAttackPas(const char* pRootId, const char* pName, float fChange, const CExtraCoeff& roExAttackValue)
: CAttackBuffMakerPas(pRootId, pName, fChange, 0, true, roExAttackValue)
{
    setDbgClassName("CDoubleAttackPas");
}

CDoubleAttackPas* CDoubleAttackPas::copy()
{
    return new CDoubleAttackPas(getRootId(), getName(), m_fChance, m_oExAttackValue);
}

void CDoubleAttackPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    CUnit* o = getOwner();
    if (o->getAttackAbilityId() == 0)
    {
        return;
    }

    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    if (M_RAND_HIT(m_fChance) == false)
    {
        return;
    }

    pAttack->getAttackValue().setValue(m_oExAttackValue.getValue(pAttack->getAttackValue().getValue()));

    CAttackAct* pAtk = nullptr;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtk);
    
    pAtk->resetCD();
    
    LOG("%s将进行%s", o->getName(), getName());
}

// CSpeedBuff
CSpeedBuff::CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oExMoveSpeedDelta(roExMoveSpeedDelta)
, m_oExAttackSpeedDelta(roExAttackSpeedDelta)
{
    setDbgClassName("CSpeedBuff");
}

CSpeedBuff* CSpeedBuff::copy()
{
    CSpeedBuff* ret = new CSpeedBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_oExMoveSpeedDelta, m_oExAttackSpeedDelta);
    ret->copyData(this);
    return ret;
}

void CSpeedBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != nullptr);
    const CExtraCoeff& rExMs = od->getExMoveSpeed();
    od->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() + m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() + m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = nullptr;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == nullptr)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() + m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() + m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度变慢(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = nullptr;
    o->getDraw()->dcast(ccd);

    if (ccd != nullptr)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "", 18, Color3B(72, 130, 200));
    }
#endif
}

void CSpeedBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    const CExtraCoeff& rExMs = d->getExMoveSpeed();
    d->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() - m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() - m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = nullptr;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == nullptr)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() - m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() - m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度恢复(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());
}

// CChangeHpPas
CChangeHpPas::CChangeHpPas(const char* pRootId, const char* pName, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp)
: CPassiveAbility(pRootId, pName)
, m_oChangeHp(roChangeHp)
, m_oMinHp(roMinHp)
{
    setDbgClassName("CChangeHpPas");
    setInterval(fInterval);
}

CChangeHpPas* CChangeHpPas::copy()
{
    return new CChangeHpPas(getRootId(), getName(), m_fInterval, m_oChangeHp, m_oMinHp);
}

void CChangeHpPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    LOG("%s获得%s状态(%.1f/s)\n", o->getName(), getName(), getChangeHp().getValue(o->getRealMaxHp()));
}

void CChangeHpPas::onUnitDelAbility()
{
    CUnit* o = getOwner();
    LOG("%s失去%s状态\n", o->getName(), getName());
}

void CChangeHpPas::onUnitInterval()
{
    CUnit* o = getOwner();
    float fNewHp = o->getHp();
    float fChangeHp = getChangeHp().getValue(o->getRealMaxHp());
    fNewHp += fChangeHp;

    float fMinHp = getMinHp().getValue(o->getRealMaxHp());
    if (fNewHp < fMinHp)
    {
        fNewHp = fMinHp;
    }

    o->setHp(fNewHp);
}

// CChangeHpBuff
CChangeHpBuff::CChangeHpBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oChangeHp(roChangeHp)
, m_oMinHp(roMinHp)
{
    setDbgClassName("CChangeHpBuff");
    setInterval(fInterval);
}

CChangeHpBuff* CChangeHpBuff::copy()
{
    CChangeHpBuff* ret = new CChangeHpBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fInterval, m_oChangeHp, m_oMinHp);
    ret->copyData(this);
    return ret;
}

void CChangeHpBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    float change = getChangeHp().getValue(o->getRealMaxHp());
    LOG("%s获得%s状态(%.1f/s)\n", o->getName(), getName(), change);
}

void CChangeHpBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    LOG("%s失去%s状态\n", o->getName(), getName());
}

void CChangeHpBuff::onUnitInterval()
{
    CUnit* o = getOwner();
    float fNewHp = o->getHp();
    float fChangeHp = getChangeHp().getValue(o->getRealMaxHp());
    fNewHp += fChangeHp;

    float fMinHp = getMinHp().getValue(o->getRealMaxHp());
    if (fNewHp < fMinHp)
    {
        fNewHp = fMinHp;
    }

    o->setHp(fNewHp);
}

// CRebirthPas
CRebirthPas::CRebirthPas(const char* pRootId, const char* pName, float fCoolDown, const CExtraCoeff& rExMaxHp)
: CPassiveAbility(pRootId, pName, fCoolDown)
, m_oExMaxHp(rExMaxHp)
, m_bRevivableBefore(false)
{
    setDbgClassName("CRebirthPas");
    setTriggerFlags(CUnit::kOnDeadTrigger);
}

CRebirthPas* CRebirthPas::copy()
{
    return new CRebirthPas(getRootId(), getName(), getCoolDown(), m_oExMaxHp);
}

void CRebirthPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    setRevivableBefore(o->isRevivable());
    o->setRevivable();
}

void CRebirthPas::onUnitDelAbility()
{
    CUnit* o = getOwner();
    o->setRevivable(isRevivableBefore());
}

void CRebirthPas::onUnitDead()
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    CWorld* w = o->getWorld();

    CUnit* oo = w->getUnit(o->getId());
    float fHp = getExMaxHp().getValue(o->getRealMaxHp());
    if (oo != nullptr)
    {
        oo->revive(fHp);
    }
    else
    {
        w->reviveUnit(o->getId(), fHp);
    }

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = nullptr;
    o->getDraw()->dcast(ccd);

    if (ccd != nullptr)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "", 18, Color3B(217, 47, 111));
    }
#endif

    LOG("Doing: %u", o->getDoingFlags());
}

// CEvadePas
CEvadePas::CEvadePas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff)
: CPassiveAbility(pRootId, pName)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
{
    setDbgClassName("CEvadePas");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CEvadePas* CEvadePas::copy()
{
    return new CEvadePas(getRootId(), getName(), m_fChance, m_iTemplateBuff);
}

bool CEvadePas::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (M_RAND_HIT(getChance()))
    {
        CUnit* o = getOwner();
        if (getTemplateBuff() != 0)
        {
            o->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }

        if (pSource != nullptr)
        {
            LOG("%s%s了%s的攻击", getOwner()->getName(), getName(), pSource->getName());
        }

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnitDrawForCC* ccd = nullptr;
        o->getDraw()->dcast(ccd);

        if (ccd != nullptr)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "", 18, Color3B(250, 104, 16));
            
            auto sp = Sprite::createWithSpriteFrame(ccd->getSprite()->getSpriteFrame());
            sp->setAnchorPoint(ccd->getSprite()->getAnchorPoint());
            sp->setFlippedX(ccd->getSprite()->isFlippedX());
            sp->setOpacity(160);
            ccd->getSprite()->getShadow()->addChild(sp);
            sp->setPosition(ccd->getSprite()->getShadow()->getAnchorPointInPoints() + Point(-10.0f, 0.0f));
            sp->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.03f, Point(20.0f, 0.0f)), MoveBy::create(0.03f, Point(-20.0f, 0.0f)), nullptr), 2), RemoveSelf::create(), nullptr));
        }
#endif
        return false;
    }

    return true;
}

// CEvadeBuff
CEvadeBuff::CEvadeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_fChance(fChance)
{
    setDbgClassName("CEvadeBuff");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CEvadeBuff* CEvadeBuff::copy()
{
    CEvadeBuff* ret = new CEvadeBuff(getRootId(), getName(), getDuration(), isStackable(), m_fChance);
    ret->copyData(this);
    return ret;
}

bool CEvadeBuff::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (M_RAND_HIT(getChance()))
    {
        LOG("%s%s了%s的攻击", getOwner()->getName(), getName(), pSource->getName());

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnit* o = getOwner();
        CUnitDrawForCC* ccd = nullptr;
        o->getDraw()->dcast(ccd);

        if (ccd != nullptr)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "", 18, Color3B(250, 104, 16));
            
            auto sp = Sprite::createWithSpriteFrame(ccd->getSprite()->getSpriteFrame());
            sp->setAnchorPoint(ccd->getSprite()->getAnchorPoint());
            sp->setFlippedX(ccd->getSprite()->isFlippedX());
            sp->setOpacity(160);
            ccd->getSprite()->getShadow()->addChild(sp);
            sp->setPosition(ccd->getSprite()->getShadow()->getAnchorPointInPoints() + Point(-10.0f, 0.0f));
            sp->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.03f, Point(20.0f, 0.0f)), MoveBy::create(0.03f, Point(-20.0f, 0.0f)), nullptr), 2), RemoveSelf::create(), nullptr));
        }
        return false;
#endif
    }
    
    return true;
}

// CTransitiveLinkBuff
CTransitiveLinkBuff::CTransitiveLinkBuff(const char* pName, float fDuration, float fRange, int iMaxTimes, int iMinIntervalTimes, uint32_t dwEffectiveTypeFlags)
: CBuffAbility("LinkBuff", pName, fDuration, true)
, m_fRange(fRange)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
, m_iMaxTimes(iMaxTimes)
, m_iTimesLeft(m_iMaxTimes)
, m_iFromUnit(0)
, m_iToUnit(0)
, m_iTemplateProjectile(0)
, m_iMinIntervalTimes(iMinIntervalTimes)
, m_bTransmited(false)
{
    setDbgClassName("CTransitiveLinkBuff");
    setTriggerFlags(CUnit::kOnDeadTrigger);
}

CTransitiveLinkBuff* CTransitiveLinkBuff::copy()
{
    CTransitiveLinkBuff* ret = new CTransitiveLinkBuff(getName(), getDuration(), m_fRange, m_iMaxTimes, m_iMinIntervalTimes, m_dwEffectiveTypeFlags);
    ret->copyData(this);
    return ret;
}

void CTransitiveLinkBuff::copyData(CAbility* from)
{
    CBuffAbility::copyData(from);
    CTransitiveLinkBuff* a = DCAST(from, CTransitiveLinkBuff*);
    setTimesLeft(a->getTimesLeft());
    setTemplateProjectile(a->getTemplateProjectile());
    m_mapTransmited = a->m_mapTransmited;
}

void CTransitiveLinkBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    m_iFromUnit = o->getId();
    setTimesLeft(getTimesLeft() - 1);
    m_mapTransmited[m_iFromUnit] = 0;
}

void CTransitiveLinkBuff::onUnitDelAbility()
{
    TransmitNext();
}

void CTransitiveLinkBuff::onUnitDead()
{
    TransmitNext();
}

void CTransitiveLinkBuff::TransmitNext()
{
    if (m_bTransmited == true)
    {
        // 保证只传递一次
        return;
    }
    else
    {
        m_bTransmited = true;
    }

    if (getTimesLeft() <= 0)
    {
        return;
    }

    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    if (o->isDead())
    {
        //return;
    }
    
    CWorld* w = o->getWorld();

    M_MAP_FOREACH(m_mapTransmited)
    {
        auto& times = M_MAP_EACH;
        ++times;
        if (times > m_iMinIntervalTimes)
        {
            M_MAP_DEL_CUR_NEXT(m_mapTransmited);
        }
        else
        {
            M_MAP_NEXT;
        }
    }
    
    CUnit* t = nullptr;
    if (m_iMinIntervalTimes > m_iMaxTimes)
    {
        t = CUnitGroup::getNearestUnitInRange(w, d->getPosition(), m_fRange, bind(&CTransitiveLinkBuff::checkConditions, this, placeholders::_1));
    }
    else
    {
        t = (new CUnitGroup(w, d->getPosition(), m_fRange, -1, bind(&CTransitiveLinkBuff::checkConditions, this, placeholders::_1)))->getRandomUnit();
    }
    
    if (!t)
    {
        return;
    }

    setFromUnit(o->getId());
    setToUnit(t->getId());

    CUnit* s = w->getUnit(getSrcUnit());

    CAttackData* pAtk = new CAttackData();

    int buff = w->addTemplateAbility(this);
    pAtk->addAttackBuff(CAttackBuff(buff, getLevel()));

    CProjectile* p = w->copyProjectile(getTemplateProjectile());
    w->addProjectile(p);
    p->setSrcUnit(getSrcUnit());
    p->setFromToType(CProjectile::kUnitToUnit);
    p->setFromUnit(getFromUnit());
    p->setToUnit(getToUnit());
    p->setAttackData(pAtk);
    p->setEffectiveTypeFlags(getEffectiveTypeFlags());
    p->setTriggerMask(CUnit::kMaskActiveTrigger);
    p->fire();
}

bool CTransitiveLinkBuff::checkConditions(CUnit* pUnit)
{
    CUnit* o = getOwner();
    CUnitForce* s = DCAST(o->getUnit(getSrcUnit()), CUnitForce*);
    if (s == nullptr)
    {
        return false;
    }

    if (pUnit->isDead() ||
        !s->canEffect(DCAST(pUnit, CUnitForce*), m_dwEffectiveTypeFlags) ||
        m_mapTransmited.find(pUnit->getId()) != m_mapTransmited.end())
    {
        return false;
    }

    return true;
}

// CTransitiveBlinkBuff
CTransitiveBlinkBuff::CTransitiveBlinkBuff(const char* pName, float fRange, int iMaxTimes, int iMinIntervalTimes, int iCastAnimation)
: CBuffAbility("LinkBuff", pName, 0.5f, true)
, m_fRange(fRange)
, m_dwEffectiveTypeFlags(CUnitForce::kEnemy)
, m_iMaxTimes(iMaxTimes)
, m_iTimesLeft(m_iMaxTimes)
, m_iFromUnit(0)
, m_iToUnit(0)
, m_iMinIntervalTimes(iMinIntervalTimes)
, m_bTransmited(false)
{
    setDbgClassName("CTransitiveBlinkBuff");
    setTriggerFlags(CUnit::kOnDeadTrigger);
    addCastAnimation(iCastAnimation);
}

CTransitiveBlinkBuff* CTransitiveBlinkBuff::copy()
{
    CTransitiveBlinkBuff* ret = new CTransitiveBlinkBuff(getName(), m_fRange, m_iMaxTimes, m_iMinIntervalTimes, 0);
    ret->copyData(this);
    return ret;
}

void CTransitiveBlinkBuff::copyData(CAbility* from)
{
    CBuffAbility::copyData(from);
    CTransitiveBlinkBuff* a = DCAST(from, CTransitiveBlinkBuff*);
    setTimesLeft(a->getTimesLeft());
    m_mapTransmited = a->m_mapTransmited;
}

void CTransitiveBlinkBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    m_iFromUnit = o->getId();
    m_mapTransmited[m_iFromUnit] = 0;

    souceUnitRunActions(m_iTimesLeft == m_iMaxTimes);

    setTimesLeft(getTimesLeft() - 1);
}

void CTransitiveBlinkBuff::onUnitDelAbility()
{
    TransmitNext();
}

void CTransitiveBlinkBuff::onUnitDead()
{
    TransmitNext();
}

void CTransitiveBlinkBuff::TransmitNext()
{
    if (m_bTransmited == true)
    {
        // 保证只传递一次
        return;
    }
    else
    {
        m_bTransmited = true;
    }

    if (getTimesLeft() <= 0)
    {
        // 终止继续传递
        resumeSourceUnit();
        return;
    }

    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    if (o->isDead())
    {
        //return;
    }

    CWorld* w = o->getWorld();

    M_MAP_FOREACH(m_mapTransmited)
    {
        auto& times = M_MAP_EACH;
        ++times;
        if (times > m_iMinIntervalTimes)
        {
            M_MAP_DEL_CUR_NEXT(m_mapTransmited);
        }
        else
        {
            M_MAP_NEXT;
        }
    }

    CUnit* t = nullptr;
    auto s = getOwner()->getUnit(getSrcUnit());

    if (m_iMinIntervalTimes > m_iMaxTimes)
    {
        t = CUnitGroup::getNearestUnitInRange(w, d->getPosition(), m_fRange, bind(&CTransitiveBlinkBuff::checkConditions, this, placeholders::_1));
    }
    else
    {
        t = (new CUnitGroup(w, d->getPosition(), m_fRange, -1, bind(&CTransitiveBlinkBuff::checkConditions, this, placeholders::_1)))->getRandomUnit();
    }

    if (!t || !s || s->isDead())
    {
        // 终止继续传递
        resumeSourceUnit();
        return;
    }

    setFromUnit(o->getId());
    setToUnit(t->getId());

    auto atk = DCAST(s->getActiveAbility(s->getAttackAbilityId()), CAttackAct*);
    
    CAttackData* pAtk = new CAttackData();
    pAtk->setAttackValue(atk->getBaseAttack().getType(), atk->getRealAttackValue());
    s->attack(pAtk, t);
    
    int buff = w->addTemplateAbility(this);
    pAtk->addAttackBuff(CAttackBuff(buff, getLevel()));

    if (t->damaged(pAtk, s, CUnit::kOnAttackedTrigger) == false)
    {
        resumeSourceUnit();
    }
}

bool CTransitiveBlinkBuff::checkConditions(CUnit* pUnit)
{
    CUnit* o = getOwner();
    CUnitForce* s = DCAST(o->getUnit(getSrcUnit()), CUnitForce*);
    if (s == nullptr)
    {
        return false;
    }

    if (pUnit->isDead() ||
        !s->canEffect(DCAST(pUnit, CUnit*), m_dwEffectiveTypeFlags) ||
        m_mapTransmited.find(pUnit->getId()) != m_mapTransmited.end())
    {
        return false;
    }

    return true;
}

void CTransitiveBlinkBuff::souceUnitRunActions(bool bFirstTime)
{
    auto o = getOwner();
    auto s = o->getUnit(getSrcUnit());
    if (s == nullptr || s->isDead())
    {
        return;
    }

    auto sd = DCAST(s->getDraw(), CUnitDrawForCC*);
    auto od = DCAST(o->getDraw(), CUnitDraw2D*);

    auto eff = Effect::createWithSpriteFrame(sd->getCurrentFrame());
    eff->setAnchorPoint(sd->getAnchorPoint());
    sd->getSprite()->getParent()->addChild(eff);
    eff->setLogicPosition(sd->getPosition());

    CPoint from = od->getPosition().getDirectionPoint(rand(), od->getHalfOfWidth());
    CPoint to = from.getForwardPoint(od->getPosition(), od->getHalfOfWidth() * 2);
    eff->setOpacity(200);
    eff->setFlippedX(from.x < eff->getPosition().x);
    eff->runAction(Sequence::create(MoveTo::create(eff->getLogicPosition().getDistance(from) / 1000.0, Point(from.x, from.y)), FadeTo::create(0.8f, 0), RemoveSelf::create(), nullptr));

    sd->cmdStop();
    //sd->stopAllActions();
    if (bFirstTime)
    {
        s->suspend();
        s->setGhost(s->getId());
    }

    sd->setPosition(from);
    sd->setFlippedX(to.x < from.x);
    float spd = 2.5f;
    sd->doMoveTo(to, 0.35f, nullptr, spd);
    // unsafe
    sd->doAnimation(getCastRandomAnimation(), nullptr, 1, bind(&CTransitiveBlinkBuff::TransmitNext, this), spd);
}

void CTransitiveBlinkBuff::resumeSourceUnit()
{
    auto s = getOwner()->getUnit(getSrcUnit());
    if (s == nullptr)
    {
        return;
    }

    s->resume();
    s->setGhost(0);
    auto sd = DCAST(s->getDraw(), CUnitDraw2D*);
    sd->cmdStop();
}

// CSplashPas
CSplashPas::CSplashPas(const char* pName, float fNearRange, const CExtraCoeff& roExNearDamage, float fFarRange, const CExtraCoeff& roExFarDamage)
: CPassiveAbility("SplashPas", pName)
, m_fNearRange(fNearRange)
, m_oExNearDamage(roExNearDamage)
, m_fFarRange(fFarRange)
, m_oExFarDamage(roExFarDamage)
{
    setTriggerFlags(CUnit::kOnDamageTargetDoneTrigger);
    setDbgClassName("CSplashPas");
}

CSplashPas* CSplashPas::copy()
{
    CSplashPas* ret = new CSplashPas(getName(), getNearRange(), getExNearDamage(), getFarRange(), getExFarDamage());

    return ret;
}

void CSplashPas::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    CUnit* o = getOwner();
    if (!pTarget || !o)
    {
        return;
    }

    CUnitDraw2D* td = DCAST(pTarget->getDraw(), CUnitDraw2D*);
    uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;
    float fDis;
    CWorld* w = o->getWorld();
    CWorld::MAP_UNITS& units = w->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* pUnit = M_MAP_EACH;
        CUnitDraw2D* pDraw = DCAST(pUnit->getDraw(), CUnitDraw2D*);
        if (!pUnit || pUnit->isGhost())
        {
            M_MAP_NEXT;
            continue;
        }

        fDis = max(0.0f, pDraw->getPosition().getDistance(td->getPosition()) - pDraw->getHalfOfWidth());
        if (fDis <= m_fFarRange && CUnitGroup::matchLivingEnemy(o)(pUnit) && pUnit != pTarget)
        {
            if (fDis <= m_fNearRange)
            {
                pUnit->damagedLow(m_oExNearDamage.getValue(fDamage), o, dwTriggerMask);
            }
            else
            {
                pUnit->damagedLow(m_oExFarDamage.getValue(fDamage), o, dwTriggerMask);
            }
        }
        M_MAP_NEXT;
    }
}

// CKnockBackBuff
const int CKnockBackBuff::CONST_ACT_TAG = CKeyGen::nextKey();

CKnockBackBuff::CKnockBackBuff(const char* pRootId, const char* pName, float fDuration, float fDistance)
: CStunBuff(pRootId, pName, fDuration, false)
, m_fDistance(fDistance)
{
    setDbgClassName("CKnockBackBuff");
}

CKnockBackBuff* CKnockBackBuff::copy()
{
    CKnockBackBuff* ret = new CKnockBackBuff(getRootId(), getName(), getDuration(), getDistance());
    ret->copyData(this);
    return ret;
}

void CKnockBackBuff::onUnitAddAbility()
{
    knockBack();
    CStunBuff::onUnitAddAbility();
}

void CKnockBackBuff::onUnitDelAbility()
{
    CStunBuff::onUnitDelAbility();
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    d->stopAction(CONST_ACT_TAG);
}

void CKnockBackBuff::onUnitDisplaceAbility(CBuffAbility* newBuff)
{
    knockBack();
}

void CKnockBackBuff::knockBack()
{
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    if (d->isFixed())
    {
        return;
    }

    CUnit* s = o->getUnit(getSrcUnit());
    CUnitDraw2D* sd = DCAST(s->getDraw(), CUnitDraw2D*);

    const CPoint& p1 = sd->getPosition();
    const CPoint& p2 = d->getPosition();

    CPoint tp = p2.getForwardPoint(p1, -m_fDistance);
    Point cctp(tp.x, tp.y);

    d->stopAction(CONST_ACT_TAG);
    Action* pAct = EaseExponentialOut::create(MoveTo::create(getDuration(), cctp));
    pAct->setTag(CONST_ACT_TAG);
    d->getSprite()->runAction(pAct);
}

// CAttractBuff
const int CAttractBuff::CONST_ACT_TAG = CKeyGen::nextKey();

CAttractBuff::CAttractBuff(const char* pRootId, const char* pName, float fDuration, float fDistance)
: CStunBuff(pRootId, pName, fDuration, false)
, m_fDistance(fDistance)
{
    setDbgClassName("CAttractBuff");
}

CAttractBuff* CAttractBuff::copy()
{
    CAttractBuff* ret = new CAttractBuff(getRootId(), getName(), getDuration(), getDistance());
    ret->copyData(this);
    return ret;
}

void CAttractBuff::onUnitAddAbility()
{
    attract();
    CStunBuff::onUnitAddAbility();
}

void CAttractBuff::onUnitDelAbility()
{
    CStunBuff::onUnitDelAbility();
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    d->stopAction(CONST_ACT_TAG);
}

void CAttractBuff::onUnitDisplaceAbility(CBuffAbility* newBuff)
{
    attract();
}

void CAttractBuff::attract()
{
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    if (d->isFixed())
    {
        return;
    }

    CUnit* s = o->getUnit(getSrcUnit());
    CUnitDraw2D* sd = DCAST(s->getDraw(), CUnitDraw2D*);

    const CPoint& p1 = sd->getPosition();
    const CPoint& p2 = d->getPosition();
    float dis = min(p2.getDistance(p1) - d->getHalfOfWidth() - sd->getHalfOfWidth(), getDistance());

    CPoint tp = p2.getForwardPoint(p1, dis);
    Point cctp(tp.x, tp.y);

    if (!d->isDoingAction(CONST_ACT_TAG))
    {
        //d->stopAction(CONST_ACT_TAG);
        Action* pAct = EaseIn::create(MoveTo::create(getDuration(), cctp), 2.0f);
        pAct->setTag(CONST_ACT_TAG);
        d->getSprite()->runAction(pAct);
    }
}

// CReflectBuff
CReflectBuff::CReflectBuff(const char* pRootId, const char* pName, float fDuration)
: CBuffAbility(pRootId, pName, fDuration, false)
{
    setTriggerFlags(CUnit::kOnProjectileArriveTrigger);
    setDbgClassName("CReflectBuff");
}

CReflectBuff* CReflectBuff::copy()
{
    CReflectBuff* ret = new CReflectBuff(getRootId(), getName(), m_fDuration);
    ret->copyData(this);
    return ret;
}

bool CReflectBuff::onUnitProjectileArrive(CProjectile* pProjectile)
{
    CUnit* o = getOwner();
    //pProjectile->setSrcUnit(o->getId());
    //pProjectile->setSrcAbility(this);

    int swp = pProjectile->getSrcUnit();
    CUnit* s = o->getUnit(swp);
    if (s == nullptr)
    {
        return true;
    }

    if (pProjectile->getFireType() == CProjectile::kFireFollow && pProjectile->getFromToType() == CProjectile::kUnitToUnit)
    {
        //pProjectile->setSrcUnit(o->getId());  如果反射后，源发射源死亡，当反射的抛射物到达目标点后，调用getSrcAbility()->onUnitAbilityEffect()的时候，技能内部的getOwner()将指向无效指针，故不能重设源单位
        pProjectile->setToUnit(swp);

        pProjectile->setMoveSpeed(pProjectile->getMoveSpeed() * 3.0f);
        pProjectile->setMaxHeightDelta(0.0f);
        pProjectile->redirect();

        return false;
    }
    else if (pProjectile->getFireType() == CProjectile::kFireStraight && (pProjectile->getFromToType() == CProjectile::kUnitToPoint || pProjectile->getFromToType() == CProjectile::kPointToPoint))
    {
        return true;
        //pProjectile->setSrcUnit(o->getId());
        if (pProjectile->getFromToType() == CProjectile::kUnitToPoint)
        {
            CUnit* u = o->getUnit(pProjectile->getFromUnit());
            CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
            pProjectile->setToPoint(d->getPosition());
        }
        else
        {
            pProjectile->setToPoint(pProjectile->getFromPoint());
        }

        pProjectile->setMoveSpeed(pProjectile->getMoveSpeed() * 5.0f);
        pProjectile->setMaxHeightDelta(0.0f);
        pProjectile->redirect();

        return false;
    }

    return true;
}

// CLimitedLife
CLimitedLifeBuff::CLimitedLifeBuff(const char* pRootId, const char* pName, float fDuration)
: CBuffAbility(pRootId, pName, fDuration, false)
{
}

CLimitedLifeBuff* CLimitedLifeBuff::copy()
{
    CLimitedLifeBuff* ret = new CLimitedLifeBuff(getRootId(), getName(), m_fDuration);
    ret->copyData(this);
    return ret;
}

void CLimitedLifeBuff::onUnitDelAbility()
{
    CUnit* u = getOwner();
    u->setHp(0.0f);
}

// CChargeJumpBuff
CChargeJumpBuff::CChargeJumpBuff(const char* pName, float fDuration, const CExtraCoeff& rExTotalSpeedDelta, float fDamageCoeff, int iTemplateBuff)
: CBuffAbility("ChargeJumpBuff", pName, fDuration + 1.0f, false)
, m_oExTotalSpeedDelta(0.0f, 0.0f)
, m_fDamageCoeff(fDamageCoeff)
, m_iTemplateBuff(iTemplateBuff)
, m_fLastAngle(0.0f)
{
    setDbgClassName("CChargeJumpBuff");
    setInterval(0.1f);
    m_iTimes = toInt(fDuration / getInterval());
    m_oExSpeedDelta.setMulriple(rExTotalSpeedDelta.getMulriple() / m_iTimes);
    m_oExSpeedDelta.setAddend(rExTotalSpeedDelta.getAddend() / m_iTimes);
}

CChargeJumpBuff* CChargeJumpBuff::copy()
{
    float times = getDuration() / getInterval();
    auto ret = new CChargeJumpBuff(getName(), getDuration() - 1.0f, CExtraCoeff(m_oExSpeedDelta.getMulriple() * times, m_oExSpeedDelta.getAddend() * times), m_fDamageCoeff, m_iTemplateBuff);
    ret->copyData(this);
    return ret;
}
#include "ActionForCC.h"
void CChargeJumpBuff::onUnitInterval()
{
    auto o = getOwner();
    auto od = DCAST(o->getDraw(), CUnitDrawForCC*);

    auto& ex = od->getExMoveSpeed();
    od->setExMoveSpeed(CExtraCoeff(ex.getMulriple() + m_oExSpeedDelta.getMulriple(), ex.getAddend() + m_oExSpeedDelta.getAddend()));
    m_oExTotalSpeedDelta.setMulriple(m_oExTotalSpeedDelta.getMulriple() + m_oExSpeedDelta.getMulriple());
    m_oExTotalSpeedDelta.setAddend(m_oExTotalSpeedDelta.getAddend() + m_oExSpeedDelta.getAddend());
    --m_iTimes;

    auto& pos = od->getPosition();

    if (m_iTimes <= 0)
    {
        setInterval(0.0f);
        if (!o->isDead())
        {
            float spd = od->getRealMoveSpeed();

            const float dur = 0.5;  // base duration
            //auto to = pos.getForwardPoint(m_oLastPosition, -dur * od->getRealMoveSpeed());
            auto to = pos.getDirectionPoint(m_fLastAngle, max(pos.getDistance(m_oStartPosition), 50.0f));

            od->cmdStop();
            o->suspend();
            auto aniInfo = od->getSpriteInfo()->getAnimationInfo(kAniAct1);
            float aniDur = aniInfo->iNotifyFrameIndex * aniInfo->pAni->getDelayPerUnit();
            od->doAnimation(kAniAct1, nullptr, 1, nullptr, aniDur / dur);  // calc notifyframe
            od->getSprite()->runAction(SequenceEx::create(EaseInOut::create(LogicJumpTo::create(dur, Point(to.x, to.y), 200), 0.4f), CallFunc::create(CC_CALLBACK_0(CChargeJumpBuff::onJumpDone, this)), nullptr));
        }

        auto& ex = od->getExMoveSpeed();
        od->setExMoveSpeed(CExtraCoeff(ex.getMulriple() - m_oExTotalSpeedDelta.getMulriple(), ex.getAddend() - m_oExTotalSpeedDelta.getAddend()));
    }

    if (pos != m_oLastPosition)
    {
        m_fLastAngle = (pos - m_oLastPosition).getAngle();
        m_oLastPosition = pos;
    }
}

void CChargeJumpBuff::onUnitAddAbility()
{
    auto o = getOwner();
    auto od = DCAST(o->getDraw(), CUnitDrawForCC*);
    auto& pos = od->getPosition();
    m_oStartPosition = pos;
    m_oLastPosition = pos;
    m_fLastAngle = od->isFlippedX() ? M_PI : 0.0f;
}

void CChargeJumpBuff::onUnitDelAbility()
{
    auto o = getOwner();
    auto od = DCAST(o->getDraw(), CUnitDrawForCC*);
    o->resume();
    od->cmdStop();
}

void CChargeJumpBuff::onJumpDone()
{
    auto o = getOwner();
    auto od = DCAST(o->getDraw(), CUnitDrawForCC*);
    auto ug = new CUnitGroup(o->getWorld(), od->getPosition(), 100, -1, CUnitGroup::matchLivingEnemy(o));

    auto eff = Effect::createWithSpriteFrameName("Effects/Kidnap/Bang.png");
    od->getSprite()->getParent()->addChild(eff);
    eff->setLogicPosition(od->getPosition());
    eff->setScale(3.0f);
    eff->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(1.0f), RemoveSelf::create(), nullptr));

    eff = Effect::create("Effects/Bang", 0.1f);
    eff->setAnchorPoint(Point(0.5f, 0.6f));
    od->getSprite()->getParent()->addChild(eff);
    eff->setScale(0.6f);
    eff->setLogicPosition(od->getPosition());
    eff->playRelease();

    M_DEF_GC(gc);
    gc->playSound("sounds/Effects/ThunderCap.mp3");
    
    auto a = DCAST(o->getActiveAbility(o->getAttackAbilityId()), CAttackAct*);
    auto& atk = a->getBaseAttack();
    auto ad = new CAttackData;
    ad->setAttackValue(atk.getType(), a->getRealAttackValue() * (1.0f + m_fDamageCoeff * m_oStartPosition.getDistance(od->getPosition()) / 100));

    ug->damaged(ad, o);
}

// 
CLimitedPasBuff::CLimitedPasBuff(const char* pName, float fDuration, int iTemplatePas)
: CBuffAbility("LimitedPasBuf", pName, fDuration, false)
, m_iTemplatePas(iTemplatePas)
, m_pInnerPas(nullptr)
{
    setDbgClassName("CLimitedPasBuff");
}

CLimitedPasBuff* CLimitedPasBuff::copy()
{
    CLimitedPasBuff* ret = new CLimitedPasBuff(getName(), m_fDuration, m_iTemplatePas);
    ret->copyData(this);
    return ret;
}

void CLimitedPasBuff::onUnitAddAbility()
{
    if (m_iTemplatePas == 0)
    {
        return;
    }

    m_pInnerPas = DCAST(CAbilityLibrary::instance()->copyAbility(m_iTemplatePas), CPassiveAbility*);
    if (m_pInnerPas == nullptr)
    {
        return;
    }

    m_pInnerPas->setTemporary();
    m_pInnerPas->setLevel(getLevel());
    auto o = getOwner();
    o->addPassiveAbility(m_pInnerPas, false);
}

void CLimitedPasBuff::onUnitDelAbility()
{
    if (m_pInnerPas == nullptr)
    {
        return;
    }

    auto o = getOwner();
    o->delPassiveAbility(m_pInnerPas->getId(), false);

    //M_SAFE_RELEASE(m_pInnerPas);
    m_pInnerPas = nullptr;
}
