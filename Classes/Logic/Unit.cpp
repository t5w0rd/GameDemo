/*
 * File:   Unit.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午10:55
 */

#include "CommInc.h"
#include "Unit.h"
#include "Ability.h"
#include "Action.h"
#include "Application.h"
#include "Item.h"
#include "Draw.h"
#include "LuaScriptEngine.h"
#include "LuaBinding.h"
#include "AbilityLibrary.h"


// CTypeValue
CTypeValue::CTypeValue(int type, float value)
: m_iType(type)
, m_fValue(value)
{
}

void CTypeValue::set(int type, float value)
{
    m_iType = type;
    m_fValue = value;
}

// CAttackValue
CAttackValue::CAttackValue(int type, float value)
: CTypeValue(type, value)
{
}

// CArmorValue
CArmorValue::CArmorValue(int type, float value)
: CTypeValue(type, value)
{
}

// CMultiAttackValue
const char* CMultiAttackValue::CONST_ARR_NAME[CAttackValue::CONST_MAX_ATTACK_TYPE][CAttackValue::CONST_MAX_NAME_INDEX] = {
#if 1
    { "kPhysical",  "物理" },
    { "kMagical",   "魔法" },
    { "kSiege",     "攻城" },
    { "kHoly",      "神圣" }
#else
    { "kPhysical",  "kPhysical" },
    { "kMagical",   "kMagical" },
    { "kSiege",     "kSiege" },
    { "kHoly",      "kHoly" }
#endif
};

CMultiAttackValue::CMultiAttackValue()
{
    setValueZero();
}

CMultiAttackValue::CMultiAttackValue(int iCount, CAttackValue::ATTACK_TYPE eType1, float fValue1, ...)
{
    setValueZero();
    setValue(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (CAttackValue::ATTACK_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setValue(eType1, fValue1);
    }
    va_end(argv);
}

float CMultiAttackValue::getValue(CAttackValue::ATTACK_TYPE eType) const
{
    if (eType > CAttackValue::CONST_MAX_ATTACK_TYPE)
    {
        return 0;
    }
    return m_afValues[eType];
}

void CMultiAttackValue::setValue(CAttackValue::ATTACK_TYPE eType, float fValue)
{
    if (eType > CAttackValue::CONST_MAX_ATTACK_TYPE)
    {
        return;
    }
    m_afValues[eType] = fValue;
}

void CMultiAttackValue::setAttackValue(const CMultiAttackValue& roAttackValue)
{
    memmove(m_afValues, roAttackValue.m_afValues, sizeof(m_afValues));
}

void CMultiAttackValue::setValueZero()
{
    memset(m_afValues, 0, sizeof(m_afValues));
}

const char* CMultiAttackValue::getName(CAttackValue::ATTACK_TYPE eType, int iIndex)
{
    return CONST_ARR_NAME[eType][iIndex];
}

const CMultiAttackValue& CMultiAttackValue::operator=(const CMultiAttackValue& roAttackValue)
{
    setAttackValue(roAttackValue);
    return *this;
}

const char* CMultiArmorValue::CONST_ARR_NAME[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_NAME_INDEX] = {
#if 1
    //{ "Normal",     "普通" },
    { "Heavy",      "重装" },
    { "Crystal",    "水晶" },
    { "Wall",       "城墙" },
    { "Hero",       "英雄" },
    { "Holy",       "神圣" }
#else
    //{ "Normal",     "Normal" },
    { "Heavy",      "Heavy" },
    { "Crystal",    "Crystal" },
    { "Wall",       "Wall" },
    { "Hero",       "Hero" },
    { "Holy",       "Holy" }
#endif
};

// CMultiArmorValue
CMultiArmorValue::CMultiArmorValue()
{
    setValueZero();
}

CMultiArmorValue::CMultiArmorValue(int iCount, CArmorValue::ARMOR_TYPE eType1, float fValue1, ...)
{
    setValueZero();
    setValue(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (CArmorValue::ARMOR_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setValue(eType1, fValue1);
    }
    va_end(argv);
}

float CMultiArmorValue::getValue(CArmorValue::ARMOR_TYPE eType) const
{
    if (eType > CArmorValue::CONST_MAX_ARMOR_TYPE)
    {
        return 0;
    }
    return m_afValues[eType];
}

void CMultiArmorValue::setValue(CArmorValue::ARMOR_TYPE eType, float fValue)
{
    if (eType > CArmorValue::CONST_MAX_ARMOR_TYPE)
    {
        return;
    }
    m_afValues[eType] = fValue;
}

void CMultiArmorValue::setArmorValue(const CMultiArmorValue& roArmorValue)
{
    memmove(m_afValues, roArmorValue.m_afValues, sizeof(m_afValues));
}

void CMultiArmorValue::setValueZero()
{
    memset(m_afValues, 0, sizeof(m_afValues));
}

const char* CMultiArmorValue::getName(CArmorValue::ARMOR_TYPE eType, int iIndex)
{
    return CONST_ARR_NAME[eType][iIndex];
}

const CMultiArmorValue& CMultiArmorValue::operator=(const CMultiArmorValue& roArmorValue)
{
    setArmorValue(roArmorValue);
    return *this;
}

/*
 0.00    无
 0.25    微弱
 0.50    弱
 0.75    较弱
 1.00    正常
 1.25    较强
 1.50    强
 1.75    超强
 2.00    瓦解
 */
// 防护效果表
float g_afArmorAttackTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE] = {
    //           物理攻击 魔法攻击 攻城攻击 神圣攻击
    /*重装护甲*/ { 1.50,   0.25,   1.00,   0.00 },
    /*水晶护甲*/ { 0.25,   1.50,   0.75,   0.00 },
    /*城墙护甲*/ { 2.00,   2.00,   0.25,   0.00 },
    /*英雄护甲*/ { 1.00,   1.00,   1.00,   0.00 },
    /*神圣护甲*/ { 5.00,   5.00,   5.00,   1.00 }
};

// CAttackBuff
CAttackBuff::CAttackBuff(int iTemplateBuff, int iBuffLevel)
: m_iTemplateBuff(iTemplateBuff)
, m_iBuffLevel(iBuffLevel)
{
}

// CAttackData
CAttackData::CAttackData()
{
    setDbgClassName("CAttackData");
}

CAttackData* CAttackData::copy()
{
    auto ret = new CAttackData;
    ret->m_oAtkVal = m_oAtkVal;
    ret->m_vecAtkBuffs = m_vecAtkBuffs;
    return ret;
}

void CAttackData::setAttackValue(int eAttackType, float fAttackValue)
{
    m_oAtkVal.set(eAttackType, fAttackValue);
}

void CAttackData::addAttackBuff(const CAttackBuff& rAttackBuff)
{
    m_vecAtkBuffs.push_back(rAttackBuff);
}

// CExtraCoeff
CExtraCoeff::CExtraCoeff(float fMulriple, float fAddend)
: m_fMulriple(fMulriple)
, m_fAddend(fAddend)
{
}

float CExtraCoeff::getValue(float fBase) const
{
    return fBase * m_fMulriple + m_fAddend;
}

// CUnitForce
CUnitForce::CUnitForce()
: m_dwForceFlag(0)
, m_dwAllyMaskFlag(0)
{
}

void CUnitForce::setForceByIndex(int iForceIndex)
{
    m_dwForceFlag = 1 << iForceIndex;
}

bool CUnitForce::isMyAlly(const CUnitForce* pForce) const
{
    return (m_dwForceFlag == pForce->m_dwForceFlag) || (m_dwAllyMaskFlag & pForce->m_dwForceFlag);
}

bool CUnitForce::isMyEnemy(const CUnitForce* pForce) const
{
    return !isMyAlly(pForce);
}

bool CUnitForce::canEffect(const CUnitForce* pForce, uint32_t dwEffectiveTypeFlags) const
{
    return ((this == pForce) && (dwEffectiveTypeFlags & CUnitForce::kSelf)) ||
           ((this != pForce) && (this->isMyAlly(pForce) && (dwEffectiveTypeFlags & CUnitForce::kAlly))) ||
           (this->isMyEnemy(pForce) && (dwEffectiveTypeFlags & CUnitForce::kEnemy));
}

// CActionManager 
CActionManager::CActionManager()
{
}

void CActionManager::addAction(CAction* pAction, CUnit* pTarget)
{
    m_vecActions.addObject(pAction);
    pAction->start(pTarget);
}

CAction* CActionManager::getActionByTag(int iTag)
{
    M_VEC_FOREACH(m_vecActions)
    {
        CAction* pAction = M_VEC_EACH;
        if (pAction->getTag() == iTag)
        {
            return pAction;
        }
        M_VEC_NEXT;
    }
    
    return nullptr;
}

void CActionManager::onTick(float dt)
{
    M_VEC_FOREACH(m_vecActions)
    {
        CAction* pAct = M_VEC_EACH;
        pAct->step(dt);
        if (pAct->isDone())
        {
            pAct->stop();
            pAct->release();
            
            M_VEC_DEL_CUR_NEXT(m_vecActions);
        }
        else
        {
            M_VEC_NEXT;
        }
    }
}

// CCommandTarget
CCommandTarget::CCommandTarget()
: m_eTargetType(kNoTarget)
, m_iTargetUnit(0)
{
}

CCommandTarget::CCommandTarget(int iTargetUnit)
: m_eTargetType(kUnitTarget)
, m_iTargetUnit(iTargetUnit)
{
}

CCommandTarget::CCommandTarget(const CPoint& rTargetPoint)
: m_eTargetType(kPointTarget)
, m_oTargetPoint(rTargetPoint)
, m_iTargetUnit(0)
{
}

bool CCommandTarget::operator==(const CCommandTarget& rTarget) const
{
    if (m_eTargetType != rTarget.getTargetType())
    {
        return false;
    }

    switch (m_eTargetType)
    {
    case kNoTarget:
        return true;

    case kPointTarget:
        return m_oTargetPoint == rTarget.getTargetPoint();

    case kUnitTarget:
        return m_iTargetUnit == rTarget.getTargetUnit();
    }

    return false;
}

void CCommandTarget::setTarget()
{
    m_eTargetType = kNoTarget;
    m_iTargetUnit = m_oTargetPoint.x = m_oTargetPoint.y = 0;
}

void CCommandTarget::setTarget(int iTargetUnit)
{
    m_eTargetType = kUnitTarget;
    m_iTargetUnit = iTargetUnit;
    m_oTargetPoint.x = m_oTargetPoint.y = 0;
}

void CCommandTarget::setTarget(const CPoint& rTargetPoint)
{
    m_eTargetType = kPointTarget;
    m_iTargetUnit = 0;
    m_oTargetPoint = rTargetPoint;
}

// CUnitEventAdapter

// CBaseAI
CBaseAI::CBaseAI()
{
    setDbgClassName("CBaseAI");
}

void CBaseAI::onUnitTick(CUnit* pUnit, float dt)
{
    if (pUnit->isSuspended() || pUnit->isDoingOr(CUnit::kObstinate))
    {
        // 如果正在固执做事或正在施法
        return;
    }

    if (pUnit->isDoingOr(CUnit::kCasting) && !pUnit->isDoingOr(CUnit::kMoving))
    {
        // 原地施法
        return;
    }

    CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
    if (d == nullptr)
    {
        return;
    }

    int atk = pUnit->getAttackAbilityId();
    if (atk == 0)
    {
        return;
    }

    // 追击目标仍在仇恨区内就继续追击
    if (d->getCastActiveAbilityId() == atk && d->getCastTarget().getTargetType() == CCommandTarget::kUnitTarget)
    {
        CUnit* tt = pUnit->getUnit(d->getCastTarget().getTargetUnit());
        if (tt != nullptr)
        {
            CUnitDraw2D* ttd = DCAST(tt->getDraw(), CUnitDraw2D*);
            if (ttd != nullptr && pUnit->isDoingAnd(CUnit::kCasting | CUnit::kMoving) && d->getPosition().getDistance(ttd->getPosition()) < d->getHostilityRange())
            {
                // 正在追击施法，距离在仇恨范围内
                return;
            }
        }
    }

    CUnit* t = CUnitGroup::getNearestUnitInRange(pUnit->getWorld(), d->getPosition(), d->getHostilityRange(), CUnitGroup::matchLivingEnemy(pUnit));
    if (t == nullptr || t->isDead())
    {
        //d->stop();
        // 搜不到仇恨区内的目标，有没有必要设置为doNothing？
        return;
    }

    if (d->getCastActiveAbilityId() != atk || d->getCastTarget().getTargetUnit() != t->getId())
    {
        //CCLOG("%d want to attack %d", pUnit->getId(), t->getId());
        d->cmdCastSpell(CCommandTarget(t->getId()), atk, false);
    }
}

void CBaseAI::onUnitDamagedDone(CUnit* pUnit, float fDamage, CUnit* pSource)
{
    if (pUnit->isSuspended() || pUnit->isDoingOr(CUnit::kObstinate))
    {
        return;
    }
    //CCLOG("%s", u->isDoingAnd(CUnit::kObstinate) ? "Obs" : "NOT");
    if (pSource == nullptr || pSource->isDead() || pSource->isMyAlly(pUnit))
    {
        return;
    }

    CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
    if (d == nullptr)
    {
        return;
    }

    int atk = pUnit->getAttackAbilityId();
    if (atk == 0)
    {
        return;
    }

    CActiveAbility* atking = ((atk == d->getCastActiveAbilityId()) ? pUnit->getActiveAbility(atk) : nullptr);
    CUnitDraw2D* td = nullptr;
    if (atking != nullptr && d->getCastTarget().getTargetType() == CCommandTarget::kUnitTarget)
    {
        // 正在进行攻击，而且攻击目标类型为单位目标
        if (d->getCastTarget().getTargetUnit() != pSource->getId())
        {
            // 伤害源不是当前的攻击目标
            CUnit* t = pUnit->getUnit(d->getCastTarget().getTargetUnit());
            if (t != nullptr)
            {
                td = DCAST(t->getDraw(), CUnitDraw2D*);
            }
            else
            {
                // 当前攻击目标已经不存在
                //CCLOG("setCastTarget(CCommandTarget())");
                d->setCastTarget(CCommandTarget());
                pUnit->endDoing(CUnit::kCasting);
                d->setCastActiveAbilityId(0);
                t = nullptr;
            }
        }
        else
        {
            return;
        }
    }

    // 当前目标存在！   如果能打到之前的目标 或 之前的目标在仇视范围内    (目标非建筑  或(是建筑，且源也是建筑))
    // 果伤害源
    if (td != nullptr && (d->getPosition().getDistance(td->getPosition()) < d->getHostilityRange() || d->checkCastTargetDistance(atking, d->getPosition(), d->getCastTarget(), td)))
    {
        // 如果能打到之前的目标，不改变攻击目标
        return;
    }

    if (pUnit->isDoingOr(CUnit::kSpinning))
    {
        return;
    }

    if (atk == 0)
    {
        return;
    }

    d->cmdCastSpell(CCommandTarget(pSource->getId()), atk, false);

    return;
}

// CUnitAI
CUnitAI::CUnitAI()
{
}

CUnitAI::~CUnitAI()
{
}

void CUnitAI::onUnitHpChanged(CUnit* pUnit, float fChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitHpChanged");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    lua_pushnumber(L, fChanged);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitTick(CUnit* pUnit, float dt)
{
    CBaseAI::onUnitTick(pUnit, dt);

    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitTick");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    lua_pushnumber(L, dt);
    int res = lua_pcall(L, 3, 0, 0);
    if (res != LUA_OK)
    {
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitDamagedDone(CUnit* pUnit, float fDamage, CUnit* pSource)
{
    CBaseAI::onUnitDamagedDone(pUnit, fDamage, pSource);

    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitDamagedDone");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pSource);
    if (lua_pcall(L, 4, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitDamageTargetDone(CUnit* pUnit, float fDamage, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitDamageTargetDone");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pTarget);
    if (lua_pcall(L, 4, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}
void CUnitAI::onUnitAddActiveAbility(CUnit* pUnit, CActiveAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitAddActiveAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "ActiveAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitDelActiveAbility(CUnit* pUnit, CActiveAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitDelActiveAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "ActiveAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitAddPassiveAbility(CUnit* pUnit, CPassiveAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitAddPassiveAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "PassiveAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitDelPassiveAbility(CUnit* pUnit, CPassiveAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitDelPassiveAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "PassiveAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitAddBuffAbility(CUnit* pUnit, CBuffAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitAddBuffAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "BuffAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitDelBuffAbility(CUnit* pUnit, CBuffAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitDelBuffAbility");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "BuffAbility", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

void CUnitAI::onUnitAbilityReady(CUnit* pUnit, CAbility* pAbility)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
    int ai = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, ai, "onUnitAbilityReady");
    lua_pushvalue(L, ai);
    luaL_pushobjptr(L, "Unit", pUnit);
    luaL_pushobjptr(L, "Ability", pAbility);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'ai'
}

// CUnit
CUnit::CUnit(const char* rootId, CUnitDraw* draw)
: CONST_ROOT_ID(rootId)
, m_pWorld(nullptr)
, m_fHp(1.001f)
, m_fMaxHp(1.001f)
, m_pAI(nullptr)
, m_pEventAdapter(nullptr)
, m_iKiller(0)
, m_iAttackAbilityId(0)
, m_iTriggerRefCount(0)
, m_iSuspendRef(0)
, m_bRevivable(false)
, m_iGhostOwner(0)
, m_pResource(nullptr)
, m_iRewardGold(0)
, m_iRewardExp(0)
, m_dwDoingFlags(0)
, m_iPriority(0)
, m_pDraw(nullptr)
, m_eSpecies(kUnknown)
, m_iEnergy(0)
{
    setDraw(draw);
    setDbgClassName("CUnit");
}

CUnit::~CUnit()
{
    setAI(nullptr);
    setEventAdapter(nullptr);
    setDraw(nullptr);
    setResource(nullptr);
}

CUnit* CUnit::copy()
{
    CUnit* ret = new CUnit(CONST_ROOT_ID.c_str(), getDraw()->copy());
    ret->copyData(this);
    return ret;
}

void CUnit::copyData(const CUnit* from)
{
    m_sName = from->m_sName;
    m_dwForceFlag = from->m_dwForceFlag;
    m_dwAllyMaskFlag = from->m_dwAllyMaskFlag;
    m_fMaxHp = from->m_fMaxHp;
    m_oExMaxHp = from->m_oExMaxHp;
    m_fHp = from->m_fHp;
    //m_pAI
    M_MAP_FOREACH(from->m_mapActAbilities)
    {
        CActiveAbility* a = M_MAP_EACH;
        M_MAP_NEXT;
        if (a->isTemporary())
        {
            continue;
        }

        addActiveAbility(DCAST(a->copy(), CActiveAbility*));
    }

    M_MAP_FOREACH(from->m_mapPasAbilities)
    {
        CPassiveAbility* a = M_MAP_EACH;
        M_MAP_NEXT;
        if (a->isTemporary())
        {
            continue;
        }

        addPassiveAbility(DCAST(a->copy(), CPassiveAbility*));
    }

    m_oBaseArmor = from->m_oBaseArmor;
    //m_oExArmorValue = from->m_oExArmorValue;
    m_bRevivable = from->m_bRevivable;
    m_iRewardGold = from->m_iRewardGold;
    m_iRewardExp = from->m_iRewardExp;

    m_iBaseExp = from->m_iBaseExp;
    m_iMaxExp = from->m_iMaxExp;
    m_iExp = from->m_iExp;
    m_pUpdate = from->m_pUpdate ? from->m_pUpdate->copy() : nullptr;
    m_iMaxLvl = from->m_iMaxLvl;
    //setLevel(from->getLevel());
}


const char* CUnit::getDbgTag() const
{
    return getName();
}

CUnit* CUnit::getUnit(int id)
{
    return id != 0 ? getWorld()->getUnit(id) : nullptr;
}

void CUnit::abilityCD(CAbility* pAbility)
{
    CWorld* w = getWorld();
    assert(w != nullptr);

    if (w->isAbilityCD(pAbility->getId()))
    {
        return;
    }
    else if (!pAbility->isCoolingDown())
    {
        pAbility->setCoolingDownElapsed(FLT_MAX);
        return;
    }

    w->addAbilityCD(pAbility);
    onAbilityCD(pAbility);
}

void CUnit::updateAbilityCD(int id)
{
    CWorld* w = getWorld();
    assert(w != nullptr);
    w->updateAbilityCD(id);
}

bool CUnit::revive(float fHp)
{
    if (isDead())
    {
        m_fHp = min(max(fHp, 1.0f), getRealMaxHp());
        onHpChanged(m_fHp);
        onRevive();
        setKiller(0);
        return true;
    }

    return false;
}

bool CUnit::isDead() const
{
    return m_fHp <= 0;
}

bool CUnit::setHp(float fHp)
{
    if (isDead())
    {
        return false;
    }

    float fMaxHp = getRealMaxHp();
    float fOldHp = m_fHp;
    m_fHp = min(fHp, fMaxHp);
    if (m_fHp != fOldHp)
    {
        onHpChanged(m_fHp - fOldHp);
    }

    if (m_fHp <= 0)
    {
        //onDying();
    }

    return true;
}

void CUnit::setMaxHp(float fMaxHp)
{
    float fOldMaxHp = getRealMaxHp();
    m_fMaxHp = max(fMaxHp, 1.0f);
    float fNewHp = m_fHp * getRealMaxHp() / fOldMaxHp;
    if (fNewHp < 1)
    {
        fNewHp = 1;
    }
    setHp(fNewHp);
}

void CUnit::setExMaxHp(const CExtraCoeff& var)
{
    float fOldMaxHp = getRealMaxHp();
    m_oExMaxHp = var;
    float fNewHp = m_fHp * getRealMaxHp() / fOldMaxHp;
    if (fNewHp < 1)
    {
        fNewHp = 1;
    }
    setHp(fNewHp);
}

float CUnit::getRealMaxHp() const
{
    return m_oExMaxHp.getValue(m_fMaxHp);
}

void CUnit::onLevelChanged(int iChanged)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitLevelChanged(this, iChanged);
    }
}

void CUnit::onRevive()
{
    CUnitDraw* d = getDraw();
    if (d != nullptr)
    {
        d->onUnitRevive();
    }

    triggerOnRevive();
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitRevive(this);
    }
}

void CUnit::onDying()
{
    CUnitDraw* d = getDraw();
    if (d != nullptr)
    {
        d->onUnitDying();
    }

    triggerOnDying();
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDying(this);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitDying(this);
    }
}

void CUnit::onDead()
{
    triggerOnDead();

    if (!isDead())
    {
        return;
    }

    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDead(this);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitDead(this);
    }
}

void CUnit::onHpChanged(float fChanged)
{
    triggerOnHpChanged(fChanged);
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitHpChanged(this, fChanged);
    }
}

void CUnit::step(float dt)
{
    //m_oActMgr.onTick(dt);
    
    updateBuffAbilityElapsed(dt);
    
    onTick(dt);

#ifdef DEBUG_FOR_CC
    // for cocos2dx
    CUnitDrawForCC* ccd = nullptr;
    getDraw()->dcast(ccd);

    if (ccd != nullptr)
    {
        M_MAP_FOREACH(m_mapHpChanged)
        {
            int id = M_MAP_EACH_KEY;
            int val = toInt(M_MAP_EACH);
            M_MAP_NEXT;

            if (val > 0)
            {
                char sz[64];
                if (id == 0)
                {
                    // heal
                    sprintf(sz, "%d", val);
                    ccd->addBattleTip(sz, "", 18, Color3B(40, 220, 40));
                }
                else
                {
                    // damage
                    sprintf(sz, "-%d", val);
                    ccd->addBattleTip(sz, "", 18, Color3B(220, 40, 40));
                }
            }
        }
    }
    
#endif

    m_mapHpChanged.clear();
}

void CUnit::onTick(float dt)
{
    CUnitDraw* d = getDraw();
    if (d != nullptr && !isDead())
    {
        d->onUnitTick(dt);
    }

    triggerOnTick(dt);
    
    if (m_pAI && !isDead())
    {
        m_pAI->onUnitTick(this, dt);
    }
}

void CUnit::onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kOnAttackTargetTrigger))
    {
        triggerOnAttackTarget(pAttack, pTarget);
    }
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAttackTarget(this, pAttack, pTarget);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitAttackTarget(this, pAttack, pTarget);
    }
}

bool CUnit::onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    bool res = true;
    if (!(dwTriggerMask & kOnAttackedTrigger))
    {
        res = triggerOnAttacked(pAttack, pSource);
    }
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAttacked(this, pAttack, pSource);
    }
    
    return res;
}

void CUnit::onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kOnDamagedSurfaceTrigger))
    {
        triggerOnDamagedSurface(pAttack, pSource);
    }

    if (!(dwTriggerMask & kOnDamagedInnerTrigger))
    {
        triggerOnDamagedInner(pAttack, pSource);
    }
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDamaged(this, pAttack, pSource);
    }
}

void CUnit::onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kOnDamagedDoneTrigger))
    {
        triggerOnDamagedDone(fDamage, pSource);
    }
    
    LOG("%s受到%d点伤害", getName(), toInt(fDamage));
    LOG("%s HP: %d/%d\n", getName(), toInt(getHp()), toInt(getRealMaxHp()));

    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDamagedDone(this, fDamage, pSource);
    }

#ifdef DEBUG_FOR_CC
    // for cocos2dx
    CUnitDrawForCC* ccd = nullptr;
    getDraw()->dcast(ccd);

    int dmg = toInt(fDamage);
    if (ccd != nullptr && dmg > 0)
    {
        char sz[64];
        sprintf(sz, "-%d", dmg);
        //ccd->addBattleTip(sz, "", 18, Color3B(255, 0, 0));
    }
#endif
}

void CUnit::onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kOnDamageTargetDoneTrigger))
    {
        triggerOnDamageTargetDone(fDamage, pTarget);
    }

    triggerOnCalcDamageTarget(fDamage, pTarget);
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDamageTargetDone(this, fDamage, pTarget);
    }
}

void CUnit::onProjectileEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    triggerOnProjectileEffect(pProjectile, pTarget);
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitProjectileEffect(this, pProjectile, pTarget);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitProjectileEffect(this, pProjectile, pTarget);
    }
}

bool CUnit::onProjectileArrive(CProjectile* pProjectile)
{
    if (triggerOnProjectileArrive(pProjectile) == false)
    {
        return false;
    }

    if (m_pAI != nullptr)
    {
        m_pAI->onUnitProjectileArrive(this, pProjectile);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitProjectileArrive(this, pProjectile);
    }

    return true;
}

void CUnit::onAddActiveAbility(CActiveAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAddActiveAbility(this, pAbility);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitAddActiveAbility(this, pAbility);
    }
}

void CUnit::onDelActiveAbility(CActiveAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDelActiveAbility(this, pAbility);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitDelActiveAbility(this, pAbility);
    }
}

void CUnit::onAddPassiveAbility(CPassiveAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAddPassiveAbility(this, pAbility);
    }
}

void CUnit::onDelPassiveAbility(CPassiveAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDelPassiveAbility(this, pAbility);
    }
}

void CUnit::onAddBuffAbility(CBuffAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAddBuffAbility(this, pAbility);
    }
}

void CUnit::onDelBuffAbility(CBuffAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDelBuffAbility(this, pAbility);
    }
}

void CUnit::onAbilityCD(CAbility* pAbility)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAbilityCD(this, pAbility);
    }

    if (m_pEventAdapter != nullptr)
    {
        m_pEventAdapter->onUnitAbilityCD(this, pAbility);
    }
}

void CUnit::onAbilityReady(CAbility* pAbility)
{
    pAbility->onUnitAbilityReady();
    
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAbilityReady(this, pAbility);
    }
}

void CUnit::onAddItem(int iIndex)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitAddItem(this, iIndex);
    }
}

void CUnit::onDelItem(int iIndex)
{
    if (m_pAI != nullptr)
    {
        m_pAI->onUnitDelItem(this, iIndex);
    }
}

void CUnit::setAI(CBaseAI* pAI)
{
    if (pAI == m_pAI)
    {
        return;
    }

    M_SAFE_RELEASE(m_pAI);
    M_SAFE_RETAIN(pAI);

    m_pAI = pAI;
}

void CUnit::setEventAdapter(CUnitEventAdapter* pEventAdapter)
{
    if (pEventAdapter == m_pEventAdapter)
    {
        return;
    }

    M_SAFE_RELEASE(m_pEventAdapter);
    M_SAFE_RETAIN(pEventAdapter);

    m_pEventAdapter = pEventAdapter;
}

bool CUnit::attack(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    onAttackTarget(pAttack, pTarget, dwTriggerMask);
    
    if (pTarget == nullptr || pTarget->onAttacked(pAttack, this, dwTriggerMask) == false)
    {
        return false;
    }

    attackLow(pAttack, pTarget, dwTriggerMask);

    return true;
}

void CUnit::attackLow(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
}

void CUnit::damaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (isDead())
    {
        return;
    }

    while (pSource != nullptr && pSource->isGhost())
    {
        auto go = pSource->getUnit(pSource->getGhostOwner());
        if (go == pSource)
        {
            break;
        }
        pSource = go;
    }
    
    onDamaged(pAttack, pSource, dwTriggerMask);

    if (pSource != nullptr)
    {
        M_VEC_FOREACH(pAttack->getAttackBuffs())
        {
            const CAttackBuff& ab = M_VEC_EACH;
            // TODO: copy BUFF from TemplateAbilities
            addBuffAbility(ab.getTemplateBuff(), pSource->getId(), ab.getBuffLevel());
            M_MAP_NEXT;
        }
    }

    //transformDamageByAttribute(pAttack);
    float fDamage = calcDamage((CAttackValue::ATTACK_TYPE)pAttack->getAttackValue().getType(),
                               pAttack->getAttackValue().getValue(),
                               (CArmorValue::ARMOR_TYPE)getBaseArmor().getType(),
                               getRealArmorValue());
    
    damagedLow(fDamage, pSource, dwTriggerMask);

    if (pSource != nullptr)
    {
        auto& hp = m_mapHpChanged[pSource->getId()];
        hp += fDamage;
    }
}

void CUnit::damagedLow(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (isDead())
    {
        return;
    }

    while (pSource != nullptr && pSource->isGhost())
    {
        auto go = pSource->getUnit(pSource->getGhostOwner());
        if (go == pSource)
        {
            break;
        }
        pSource = go;
    }

    if (fDamage >= m_fHp)
    {
        setKiller(pSource->getId());
        setHp(0);
    }
    else
    {
        setHp(m_fHp - fDamage);
    }
    
    onDamagedDone(fDamage, pSource, dwTriggerMask);
    
    if (pSource != nullptr)
    {
        pSource->onDamageTargetDone(fDamage, this, dwTriggerMask);
    }
}

float CUnit::calcDamage(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue)
{
    float aa = g_afArmorAttackTable[eArmorType][eAttackType];
    float fRet;
    if (fArmorValue > 0)
    {
        fRet = fArmorValue * aa * 0.06;
        fRet = 1 - fRet / (fRet + 1);
    }
    else
    {
        fRet = 2 - pow(0.94f, -fArmorValue * (aa < 1 ? (2 - aa) : (1 / aa)));
    }
    fRet *= fAttackValue;
    
    return fRet;
}

void CUnit::healLow(float heal, CUnit* pSource, uint32_t dwTriggerMask/* = kNoMasked*/)
{
    float newHp = min(getRealMaxHp(), getHp() + heal);
    auto& hp = m_mapHpChanged[0];
    hp += newHp - getHp();

    setHp(newHp);
}

void CUnit::addActiveAbility(CActiveAbility* pAbility, bool bNotify)
{
    m_mapActAbilities.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddActiveAbility(pAbility);
    }
}

void CUnit::addActiveAbility(int id, int iLevel, bool bNotify)
{
    CWorld* w = getWorld();
    CActiveAbility* pAbility = nullptr;
    w->copyAbility(id)->dcast(pAbility);
    if (pAbility == nullptr)
    {
        CCLOG("ERR | addActiveAbility(%d, %d, %d), ability is not found", id, iLevel, (int)bNotify);
        return;
    }
    pAbility->setLevel(iLevel);
    addActiveAbility(pAbility, bNotify);
}

void CUnit::delActiveAbility(int id, bool bNotify)
{
    auto it = m_mapActAbilities.find(id);
    if (it == m_mapActAbilities.end())
    {
        return;
    }
    
    CActiveAbility* pAbility = it->second;
    
    if (bNotify)
    {
        onDelActiveAbility(pAbility);
    }
    
    if (pAbility->isCoolingDown())
    {
        getWorld()->delAbilityCD(id);
    }
    
    pAbility->onDelFromUnit();
    delAbilityFromTriggers(pAbility);
    
    m_mapActAbilities.erase(it);
    pAbility->release();
}

CActiveAbility* CUnit::getActiveAbility(int id)
{
    return id != 0 ? m_mapActAbilities.getObject(id) : nullptr;
}

CActiveAbility* CUnit::getActiveAbility(const char* name)
{
    M_MAP_FOREACH(m_mapActAbilities)
    {
        CActiveAbility* a = M_MAP_EACH;
        if (strcmp(a->getName(), name) == 0)
        {
            return a;
        }

        M_MAP_NEXT;
    }

    return nullptr;
}

void CUnit::addPassiveAbility(CPassiveAbility* pAbility, bool bNotify)
{
    m_mapPasAbilities.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddPassiveAbility(pAbility);
    }
}

void CUnit::addPassiveAbility(int id, int iLevel, bool bNotify)
{
    CWorld* w = getWorld();
    CPassiveAbility* pAbility = nullptr;
    w->copyAbility(id)->dcast(pAbility);
    if (pAbility == nullptr)
    {
        CCLOG("ERR | addPassiveAbility(%d, %d, %d), ability is not found", id, iLevel, (int)bNotify);
        return;
    }
    pAbility->setLevel(iLevel);
    addPassiveAbility(pAbility, bNotify);
}

void CUnit::delPassiveAbility(int id, bool bNotify)
{
    auto it = m_mapPasAbilities.find(id);
    if (it == m_mapPasAbilities.end())
    {
        return;
    }
    
    CPassiveAbility* pAbility = it->second;
    
    if (bNotify)
    {
        onDelPassiveAbility(pAbility);
    }
    
    if (pAbility->isCoolingDown())
    {
        getWorld()->delAbilityCD(id);
    }
    
    pAbility->onDelFromUnit();
    delAbilityFromTriggers(pAbility);
    
    m_mapPasAbilities.erase(it);
    pAbility->release();
}

CPassiveAbility* CUnit::getPassiveAbility(int id)
{
    return id != 0 ? m_mapPasAbilities.getObject(id) : nullptr;
}

void CUnit::addBuffAbility(CBuffAbility* pAbility, bool bNotify)
{
    if (pAbility->isStackable() == false)
    {
        M_MAP_FOREACH(m_mapBuffAbilities)
        {
            CBuffAbility* pBuff = M_MAP_EACH;
            if (strcmp(pBuff->getRootId(), pAbility->getRootId()) == 0)
#if 0
            {
                if (pBuff->isCoolingDown())
                {
                    getWorld()->delAbilityCD(pBuff->getId());
                }

                pBuff->onDelFromUnit();
                delAbilityFromTriggers(pBuff);
                pBuff->release();
                
                m_mapBuffAbilities.addObject(pAbility);
                pAbility->onAddToUnit(this);
                addAbilityToTriggers(pAbility);
                
                M_MAP_DEL_CUR_NEXT(m_mapBuffAbilities);
                return;
            }
            else
            {
                M_MAP_NEXT;
            }
#else
            {
                pBuff->setSrcUnit(pAbility->getSrcUnit());
                pBuff->setLevel(pAbility->getLevel());
                pBuff->setName(pAbility->getName());
                pBuff->setDuration(pAbility->getDuration());
                pBuff->setElapsed(0.0f);
                pBuff->onUnitDisplaceAbility(pAbility);

                if (pAbility->getAppendBuff() != 0)
                {
                    addBuffAbility(pAbility->getAppendBuff(), pAbility->getSrcUnit(), pAbility->getLevel());
                }

                return;
            }
            M_MAP_NEXT;
#endif
        }
    }
    
    m_mapBuffAbilities.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddBuffAbility(pAbility);
    }

    if (pAbility->getAppendBuff() != 0)
    {
        addBuffAbility(pAbility->getAppendBuff(), pAbility->getSrcUnit(), pAbility->getLevel());
    }
}

void CUnit::addBuffAbility(int id, int iSrcUnit, int iLevel, bool bNotify)
{
    CWorld* w = getWorld();
    CBuffAbility* pAbility = nullptr;
    w->copyAbility(id)->dcast(pAbility);
    if (pAbility == nullptr)
    {
        CCLOG("ERR | addBuffAbility(%d, %d, %d, %d), ability is not found", id, iSrcUnit, iLevel, (int)bNotify);
        return;
    }

    pAbility->setSrcUnit(iSrcUnit);
    pAbility->setLevel(iLevel);
    addBuffAbility(pAbility, bNotify);
}

void CUnit::delBuffAbility(int id, bool bNotify)
{
    auto it = m_mapBuffAbilities.find(id);
    if (it == m_mapBuffAbilities.end())
    {
        return;
    }
    
    CBuffAbility* pAbility = it->second;
    
    if (bNotify)
    {
        onDelBuffAbility(pAbility);
    }
    
    if (pAbility->isCoolingDown())
    {
        getWorld()->delAbilityCD(id);
    }
    
    pAbility->onDelFromUnit();
    delAbilityFromTriggers(pAbility);
    
    m_mapBuffAbilities.erase(it);
    pAbility->release();
}

CBuffAbility* CUnit::getBuffAbility(int id)
{
    return id != 0 ? m_mapBuffAbilities.getObject(id) : nullptr;
}

CBuffAbility* CUnit::getBuffAbility(const char* name)
{
    M_MAP_FOREACH(m_mapBuffAbilities)
    {
        CBuffAbility* a = M_MAP_EACH;
        if (strcmp(a->getName(), name) == 0)
        {
            return a;
        }

        M_MAP_NEXT;
    }

    return nullptr;
}

void CUnit::addSystemAbility(CPassiveAbility* pAbility)
{
    m_mapSysAbilities.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
}

void CUnit::delSystemAbility(const char *name)
{
    M_MAP_FOREACH(m_mapSysAbilities)
    {
        CPassiveAbility* a = M_MAP_EACH;
        if (strcmp(a->getName(), name) == 0)
        {
            if (a->isCoolingDown())
            {
                getWorld()->delAbilityCD(a->getId());
            }

            a->onDelFromUnit();
            delAbilityFromTriggers(a);

            m_mapSysAbilities.erase(M_MAP_IT);
            a->release();
            return;
        }

        M_MAP_NEXT;
    }
}

void CUnit::updateBuffAbilityElapsed(float dt)
{
    M_MAP_FOREACH(m_mapBuffAbilities)
    {
        CBuffAbility* pBuff = M_MAP_EACH;
        pBuff->setElapsed(pBuff->getElapsed() + dt);
        if (pBuff->isDone())
        {
            if (pBuff->isCoolingDown())
            {
                getWorld()->delAbilityCD(pBuff->getId());
            }

            pBuff->onDelFromUnit();
            delAbilityFromTriggers(pBuff);
            pBuff->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapBuffAbilities);
        }
        else
        {
            M_MAP_NEXT;
        }
    }
}

void CUnit::addAbilityToTriggers(CAbility* pAbility)
{
    assert(pAbility != nullptr);
    uint32_t dwTriggerFlags = pAbility->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerAbilitiesToAdd.addObject(pAbility);
        return;
    }
    
    if (dwTriggerFlags & kOnReviveTrigger)
    {
        m_mapOnReviveTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnDyingTrigger)
    {
        m_mapOnDyingTriggerAbilities.addObject(pAbility);
    }

    if (dwTriggerFlags & kOnDeadTrigger)
    {
        m_mapOnDeadTriggerAbilities.addObject(pAbility);
    }

    if (dwTriggerFlags & kOnHpChangedTrigger)
    {
        m_mapOnHpChangedTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnTickTrigger)
    {
        m_mapOnTickTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnAttackedTrigger)
    {
        m_mapOnAttackedTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerAbilities.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kOnProjectileEffectTrigger)
    {
        m_mapOnProjectileEffectTriggerAbilities.addObject(pAbility);
    }

    if (dwTriggerFlags & kOnProjectileArriveTrigger)
    {
        m_mapOnProjectileArriveTriggerAbilities.addObject(pAbility);
    }

    if (dwTriggerFlags & kOnCalcDamageTargetTrigger)
    {
        m_mapOnCalcDamageTargetTriggerAbilities.addObject(pAbility);
    }
}

void CUnit::delAbilityFromTriggers(CAbility* pAbility)
{
    assert(pAbility != nullptr);
    uint32_t dwTriggerFlags = pAbility->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerAbilitiesToDel.addObject(pAbility);
        return;
    }
    
    int id = pAbility->getId();
    
    if (dwTriggerFlags & kOnReviveTrigger)
    {
        m_mapOnReviveTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnDyingTrigger)
    {
        m_mapOnDyingTriggerAbilities.delObject(id);
    }

    if (dwTriggerFlags & kOnDeadTrigger)
    {
        m_mapOnDeadTriggerAbilities.delObject(id);
    }

    if (dwTriggerFlags & kOnHpChangedTrigger)
    {
        m_mapOnHpChangedTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnTickTrigger)
    {
        m_mapOnTickTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnAttackedTrigger)
    {
        m_mapOnAttackedTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerAbilities.delObject(id);
    }
    
    if (dwTriggerFlags & kOnProjectileEffectTrigger)
    {
        m_mapOnProjectileEffectTriggerAbilities.delObject(id);
    }

    if (dwTriggerFlags & kOnProjectileArriveTrigger)
    {
        m_mapOnProjectileArriveTriggerAbilities.delObject(id);
    }

    if (dwTriggerFlags & kOnCalcDamageTargetTrigger)
    {
        m_mapOnCalcDamageTargetTriggerAbilities.delObject(id);
    }
}

void CUnit::updateTriggerAbilitiesWhenTriggerFree()
{
    assert(isTriggerFree());
    
    M_MAP_FOREACH(m_mapTriggerAbilitiesToAdd)
    {
        CAbility* pAbility = M_MAP_EACH;
        addAbilityToTriggers(pAbility);
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(m_mapTriggerAbilitiesToDel)
    {
        CAbility* pAbility = M_MAP_EACH;
        delAbilityFromTriggers(pAbility);
        M_MAP_NEXT;
    }
    
    m_mapTriggerAbilitiesToAdd.delAllObjects();
    m_mapTriggerAbilitiesToDel.delAllObjects();
}

void CUnit::beginTrigger()
{
    ++m_iTriggerRefCount;
}

void CUnit::endTrigger()
{
    assert(m_iTriggerRefCount > 0);
    --m_iTriggerRefCount;
    if (m_iTriggerRefCount == 0)
    {
        updateTriggerAbilitiesWhenTriggerFree();
    }
}

bool CUnit::isTriggerFree() const
{
    return m_iTriggerRefCount == 0;
}

void CUnit::triggerOnRevive()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnReviveTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitRevive();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDying()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDyingTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDying();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDead()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDeadTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDead();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnHpChanged(float fChanged)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnHpChangedTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitHpChanged(fChanged);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnTick(float dt)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnTickTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        //pAbility->onUnitTick(dt);
        if (pAbility->getInterval() > 0.0f)
        {
            pAbility->setIntervalElapsed(pAbility->getIntervalElapsed() + dt);

            while (pAbility->getIntervalElapsed() >= pAbility->getInterval())
            {
                pAbility->onUnitInterval();
                if (pAbility->getInterval() > 0.0f)
                {
                    pAbility->setIntervalElapsed(pAbility->getIntervalElapsed() - pAbility->getInterval());
                }
                else
                {
                    pAbility->setIntervalElapsed(0.0f);
                    break;
                }
            }
        }
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnAttackTargetTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitAttackTarget(pAttack, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

bool CUnit::triggerOnAttacked(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    bool res = true;
    M_MAP_FOREACH(m_mapOnAttackedTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        if (pAbility->onUnitAttacked(pAttack, pSource) == false)
        {
            res = false;
            break;
        }
        M_MAP_NEXT;
    }
    endTrigger();
    return res;
}

void CUnit::triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedSurfaceTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDamaged(pAttack, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamagedInner(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedInnerTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDamaged(pAttack, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamagedDone(float fDamage, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedDoneTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDamagedDone(fDamage, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamageTargetDone(float fDamage, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamageTargetDoneTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDamageTargetDone(fDamage, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnProjectileEffect(CProjectile* pProjectile, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnProjectileEffectTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitProjectileEffect(pProjectile, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

bool CUnit::triggerOnProjectileArrive(CProjectile* pProjectile)
{
    beginTrigger();
    bool res = true;
    M_MAP_FOREACH(m_mapOnProjectileArriveTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        if (pAbility->onUnitProjectileArrive(pProjectile) == false)
        {
            res = false;
            break;
        }
        M_MAP_NEXT;
    }
    endTrigger();
    return res;
}

void CUnit::triggerOnCalcDamageTarget(float fDamage, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnCalcDamageTargetTriggerAbilities)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitCalcDamageTarget(fDamage, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

bool CUnit::isSuspended() const
{
    return m_iSuspendRef > 0;
}

void CUnit::suspend()
{
    endDoing(kObstinate);
    ++m_iSuspendRef;
    // TODO: stop actions
    if (m_iSuspendRef == 1)
    {
        LOG("%s不能动了", getName());
    }
}

void CUnit::resume()
{
    if (m_iSuspendRef == 0)
    {
        return;
    }
    
    --m_iSuspendRef;
    if (m_iSuspendRef == 0)
    {
        // TODO: resume
        LOG("%s能动了", getName());
    }
}

float CUnit::getRealArmorValue() const
{
    return m_oExArmorValue.getValue(m_oBaseArmor.getValue());
}

void CUnit::setGhost(int iGhostOwner)
{
    bool bGhost = m_iGhostOwner != 0;
    m_iGhostOwner = iGhostOwner;

    if ((m_iGhostOwner != 0) == bGhost)
    {
        return;
    }

    CWorld* w = getWorld();
    if (w == nullptr)
    {
        return;
    }

    if (m_iGhostOwner != 0)
    {
        w->onDelNormalAttributes(this);
    }
    else
    {
        w->onAddNormalAttributes(this);
    }
}

bool CUnit::isGhost() const
{
    return m_iGhostOwner != 0;
}

int CUnit::getGhostOwner() const
{
    return m_iGhostOwner;
}

CUnit* CUnit::getRootGhostOwner()
{
    CUnit* res = this;
    while (res != nullptr && res->isGhost())
    {
        auto go = res->getUnit(res->getGhostOwner());
        if (go == res)
        {
            break;
        }
        res = go;
    }
    return res;
}

void CUnit::setResource(CForceResource* var)
{
    if (m_pResource == var)
    {
        return;
    }

    M_SAFE_RETAIN(var);
    M_SAFE_RELEASE(m_pResource);
    m_pResource = var;
}

void CUnit::setPackageSize(int iSize)
{
    m_vecItems.resize(iSize);
}

bool CUnit::addItem(CItem* pItem)
{
    if (pItem->getMaxStackSize() == 0)
    {
        // 直接使用
        assert(pItem->getItemType() == CItem::kConsumable);
        pItem->onAddToNewSlot(this);
        assert(!pItem->getActiveAbilities().empty());
        
        pItem->use();
        pItem->onDelFromSlot();
        return true;
    }
    
    int iFirstEmpty = -1;
    for (int i = 0; i < (int)m_vecItems.size(); ++i)
    {
        CItem* pSlot = m_vecItems[i];
        if (pSlot == nullptr)
        {
            // 找到第一个空位
            if (iFirstEmpty == -1)
            {
                iFirstEmpty = i;
            }
            continue;
        }
        
        if (strcmp(pSlot->getRootId(), pItem->getRootId()) == 0)
        {
            // 找到堆叠组
            int iFree = pSlot->getFreeStackSize();
            if (iFree > 0)
            {
                // 可以继续堆叠
                unsigned int uInc = pSlot->incStackCount(pItem->getStackCount());
                pItem->decStatckCount(uInc);
                return true;
            }
            else
            {
                // 已到达堆叠上限
                return false;
            }
        }
    }
    
    if (iFirstEmpty == -1)
    {
        // 没找到空位
        return false;
    }
    
    m_vecItems.setObject(iFirstEmpty, pItem);
    pItem->onAddToNewSlot(this);  // 消息传递
    
    onAddItem(iFirstEmpty);
    
    return true;
}

void CUnit::delItem(int iIndex)
{
    if (iIndex < 0 || iIndex >= (int)m_vecItems.size())
    {
        return;
    }
    
    if (m_vecItems[iIndex] == nullptr)
    {
        return;
    }
    
    onDelItem(iIndex);
    
    m_vecItems[iIndex]->onDelFromSlot();
    m_vecItems.delObject(iIndex);
}

CItem* CUnit::getItem(int iIndex)
{
    if (iIndex < 0 || iIndex >= (int)m_vecItems.size())
    {
        return nullptr;
    }
    
    return m_vecItems[iIndex];
}

int CUnit::useItem(int iIndex)
{
    if (isSuspended() || isDead() || iIndex < 0 || iIndex >= (int)m_vecItems.size())
    {
        return -1;
    }
    
    CItem* pItem = m_vecItems[iIndex];
    if (pItem == nullptr)
    {
        return -1;
    }
    
    if (pItem->use() == false)
    {
        return -1;
    }
    //LOG("%s使用了%s", getName(), pItem->getName());
    
    if (pItem->getStackCount() == 0)
    {
        delItem(iIndex);
    }
    
    return 0;
}

void CUnit::startDoing(uint32_t dwDoingFlags)
{
    m_dwDoingFlags |= dwDoingFlags;
}

void CUnit::endDoing(uint32_t dwDoingFlags)
{
    m_dwDoingFlags &= (~dwDoingFlags);
}

bool CUnit::isDoingOr(uint32_t dwDoingFlags) const
{
    return (m_dwDoingFlags & dwDoingFlags) != 0;
}

bool CUnit::isDoingAnd(uint32_t dwDoingFlags) const
{
    return (m_dwDoingFlags & dwDoingFlags) == dwDoingFlags;
}

bool CUnit::isDoingNothing() const
{
    return m_dwDoingFlags == 0;
}

bool CUnit::tryDoing(int priority)
{
    if (priority > m_iPriority)
    {
        m_iPriority = priority;
        return true;
    }

    return false;
}

void CUnit::setDraw(CUnitDraw* pDraw)
{
    if (pDraw == m_pDraw)
    {
        return;
    }
    
    if (pDraw != nullptr)
    {
        pDraw->retain();
        pDraw->setUnit(this);
    }
    M_SAFE_RELEASE(m_pDraw);
    m_pDraw = pDraw;
}

void CUnit::runAction(CAction* pAction)
{
    m_oActMgr.addAction(pAction, this);
}

CAction* CUnit::getActionByTag(int iTag)
{
    return m_oActMgr.getActionByTag(iTag);
}

// CAIRetainer
CAIRetainer::CAIRetainer()
: m_w(nullptr)
, m_unitId(0)
, m_aiOld(nullptr)
, m_aiCur(nullptr)
{
}

CAIRetainer::~CAIRetainer()
{
    M_SAFE_RELEASE(m_aiOld);
    M_SAFE_RELEASE(m_aiCur);
}

void CAIRetainer::init(CWorld* w)
{
    m_w = w;
}

void CAIRetainer::retain(CUnit* u)
{
    if (u != nullptr && (u->getAI() == nullptr || (u->getId() == m_unitId && m_aiCur == u->getAI())))
    {
        return;
    }

    if (m_aiOld != nullptr)
    {
        auto old = m_w->getUnit(m_unitId);
        if (old != nullptr && m_aiCur == old->getAI())
        {
            // 之前的单位存在，且AI没有更换
            old->setAI(m_aiOld);
        }
        m_aiOld->release();
        m_aiCur->release();
    }

    if (u == nullptr)
    {
        m_unitId = 0;
        m_aiOld = nullptr;
        m_aiCur = nullptr;
    }
    else
    {
        m_unitId = u->getId();
        m_aiOld = u->getAI();
        m_aiOld->retain();
        m_aiCur = new CBaseAI();
        m_aiCur->retain();
        u->setAI(m_aiCur);
    }
}

// CWorld
CWorld::CWorld()
: m_iScriptHandler(0)
, m_iCtrlUnit(0)
, m_bShutdown(false)
{
    setDbgClassName("CWorld");
}

CWorld::~CWorld()
{
}

bool CWorld::onInit()
{
    return true;
}

void CWorld::onTick(float dt)
{
}

void CWorld::onAddUnit(CUnit* pUnit)
{
}

void CWorld::onDelUnit(CUnit* pUnit)
{
}

void CWorld::onAddProjectile(CProjectile* pProjectile)
{
}

void CWorld::onDelProjectile(CProjectile* pProjectile)
{
}

bool CWorld::init()
{
    m_aiRetainer.init(this);
    m_mapMRObjs.delAllObjects();
    return onInit();
}

void CWorld::setCtrlUnit(CUnit* u)
{
    if (u == nullptr)
    {
        m_iCtrlUnit = 0;
    }
    else
    {
        m_iCtrlUnit = u->getId();
    }

    m_aiRetainer.retain(u);
}

void CWorld::addUnit(CUnit* pUnit)
{
    onAddUnit(pUnit);

    pUnit->setWorld(this);
    pUnit->setEventAdapter(this);
    m_mapUnits.addObject(pUnit);
}

void CWorld::delUnit(int id, bool bRevivable /*= false*/)
{
    auto it = m_mapUnits.find(id);
    if (it == m_mapUnits.end())
    {
        return;
    }

    CUnit* pUnit = it->second;

    onDelUnit(pUnit);

    if (bRevivable)
    {
        // 如果单位可以复活，拖进灵魂域
        m_mapUnitsToRevive.addObject(pUnit);
    }
    else
    {
        // 如果不可以复活，该单位将不再拥有世界，清除该单位的所有CD中的技能
        pUnit->setWorld(nullptr);
        if (pUnit->getEventAdapter() == this)
        {
            pUnit->setEventAdapter(nullptr);
        }
        cleanAbilitiesCD(pUnit);
    }

    pUnit->release();
    m_mapUnits.erase(it);
}

CUnit* CWorld::getUnit(int id) const
{
    return m_mapUnits.getObject(id);
}

void CWorld::reviveUnit(int id, float fHp)
{
    auto it = m_mapUnitsToRevive.find(id);
    if (it == m_mapUnitsToRevive.end())
    {
        return;
    }
    
    CUnit* pUnit = it->second;
    if (pUnit == nullptr)
    {
        return;
    }
    
    addUnit(pUnit);
    pUnit->revive(fHp);
    
    m_mapUnitsToRevive.delObject(it);
}

CUnit* CWorld::getUnitToRevive(int id)
{
    return m_mapUnitsToRevive.getObject(id);
}

void CWorld::addAbilityCD(CAbility* pAbility)
{
    assert(pAbility->isCoolingDown());
    m_mapAbilitiesCD.addObject(pAbility);
    LOG("%s的%s技能开始冷却(%.1fs)", pAbility->getOwner()->getName(), pAbility->getName(), pAbility->getRealCoolDown());
}

void CWorld::delAbilityCD(int id)
{
    m_mapAbilitiesCD.delObject(id);
}

bool CWorld::isAbilityCD(int id) const
{
    return m_mapAbilitiesCD.find(id) != m_mapAbilitiesCD.end();
}

CAbility* CWorld::getAbilityCD(int id) const
{
    return m_mapAbilitiesCD.getObject(id);
}

void CWorld::updateAbilityCD(int id)
{
    auto it = m_mapAbilitiesCD.find(id);
    if (it == m_mapAbilitiesCD.end())
    {
        return;
    }
    
    CAbility* pAbility = it->second;
    if (pAbility->isCoolingDown())
    {
        return;
    }
    
    m_mapAbilitiesCD.erase(it);
    abilityReady(pAbility);
    pAbility->release();
}

void CWorld::onAddNormalAttributes(CUnit* pUnit)
{
}

void CWorld::onDelNormalAttributes(CUnit* pUnit)
{
}

void CWorld::cleanAbilitiesCD(CUnit* pUnit)
{
    M_MAP_FOREACH(pUnit->getActiveAbilities())
    {
        CActiveAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitiesCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getPassiveAbilities())
    {
        CPassiveAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitiesCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getBuffAbilities())
    {
        CBuffAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitiesCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
}

void CWorld::abilityReady(CAbility* pAbility)
{
    // 由于技能的所有者可能在等待重生，所以主世界可能不存在该单位，但是单位仍未被释放
    CUnit* o = getUnit(pAbility->getOwner()->getId());
    if (o != nullptr)
    {
        // 存在于主世界中，则触发事件
        o->onAbilityReady(pAbility);
    }

    // 防止BUFF更改CD而导技能在致脱离CD管理器后CD大于Elapsed
    pAbility->setCoolingDownElapsed(FLT_MAX);
}

void CWorld::step(float dt)
{
    if (m_bShutdown)
    {
        return;
    }

    // 单位死亡后技能CD独立计算，所以放在此处独立计算，不整合到单位onTick中
    M_MAP_FOREACH(m_mapAbilitiesCD)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->setCoolingDownElapsed(pAbility->getCoolingDownElapsed() + dt);
        if (!pAbility->isCoolingDown())
        {
            // 如果技能已经就绪，从中删除
            abilityReady(pAbility);
            pAbility->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapAbilitiesCD);
        }
        else
        {
            M_MAP_NEXT;
        }
    }
    
    M_MAP_FOREACH(m_mapUnits)
    {
        CUnit* pUnit = M_MAP_EACH;
        pUnit->step(dt);
        
        if (pUnit->isDead() && !pUnit->isDoingOr(CUnit::kDying))  // terrible code
        {
            // 刚死，计划最后移除该单位
            pUnit->onDying();
        }

        M_MAP_NEXT;
    }

    M_MAP_FOREACH(m_mapProjectiles)
    {
        CProjectile* pProjectiles = M_MAP_EACH;
        pProjectiles->step(dt);

        M_MAP_NEXT;
    }
    
    onTick(dt);
}

int CWorld::addTemplateAbility(CAbility* pAbility)
{
    static CAbilityLibrary* al = CAbilityLibrary::instance();
    return al->addTemplateAbility(pAbility);
}

int CWorld::addTemplateAbility(int id, CAbility* pAbility)
{
    static CAbilityLibrary* al =  CAbilityLibrary::instance();
    return al->addTemplateAbility(id, pAbility);
}

CAbility* CWorld::copyAbility(int id) const
{
    static CAbilityLibrary* al = CAbilityLibrary::instance();
    return al->copyAbility(id);
}

void CWorld::addProjectile(CProjectile* pProjectile)
{
    onAddProjectile(pProjectile);

    pProjectile->setWorld(this);
    m_mapProjectiles.addObject(pProjectile);
}


void CWorld::delProjectile(int id)
{
    auto it = m_mapProjectiles.find(id);
    if (it == m_mapProjectiles.end())
    {
        return;
    }

    CProjectile* pProjectile = it->second;
    onDelProjectile(pProjectile);

    pProjectile->setWorld(nullptr);
    pProjectile->release();
    m_mapProjectiles.erase(it);
}

void CWorld::shutdown()
{
    onShutDown();
    m_bShutdown = true;
    m_aiRetainer.retain(nullptr);
    m_mapUnits.delAllObjects();
    m_mapUnitsToRevive.delAllObjects();
    m_mapAbilitiesCD.delAllObjects();
    m_mapProjectiles.delAllObjects();
}

void CWorld::onShutDown()
{
}

// CForceResource
CForceResource::CForceResource(const FUNC_ONGOLDCHANGED& onGoldChanged)
: m_iGold(0)
{
    setDbgClassName("CForceResource");
    m_onGoldChanged = onGoldChanged;
}

void CForceResource::changeGold(int iChange)
{
    m_iGold += iChange;
    if (iChange)
    {
        onGoldChanged(iChange);
    }
}

void CForceResource::onGoldChanged(int iChange)
{
    if (m_onGoldChanged)
    {
        m_onGoldChanged(iChange);
    }
}
