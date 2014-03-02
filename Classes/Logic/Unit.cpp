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
#include "LuaBinding.h"


#ifdef DEBUG_FOR_CC
// for cocos2d
#include "../CommHeader.h"
#include "../DrawForCC.h"
#endif


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
CAttackValue::CAttackValue(ATTACK_TYPE type, float value)
: CTypeValue(type, value)
{
}

// CArmorValue
CArmorValue::CArmorValue(ARMOR_TYPE type, float value)
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
    { "Normal",     "普通" },
    { "Heavy",      "重装" },
    { "Crystal",    "水晶" },
    { "Wall",       "城墙" },
    { "Hero",       "英雄" },
    { "Holy",       "神圣" }
#else
    { "Normal",     "Normal" },
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

float g_afAttackArmorTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE] = {
    //           物理攻击 魔法攻击 攻城攻击 神圣攻击
    /*普通护甲*/ { 1.00,   1.00,   1.00,   1.00 },
    /*重装护甲*/ { 0.50,   1.50,   0.75,   1.00 },
    /*水晶护甲*/ { 1.25,   0.50,   1.50,   1.00 },
    /*城墙护甲*/ { 0.50,   0.50,   1.50,   1.00 },
    /*英雄护甲*/ { 0.75,   0.75,   0.75,   1.00 },
    /*神圣护甲*/ { 0.25,   0.25,   0.25,   1.00 }
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

void CAttackData::setAttackValue(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue)
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

bool CUnitForce::isAllyOf(const CUnitForce* pForce) const
{
    return (m_dwForceFlag == pForce->m_dwForceFlag) || (m_dwAllyMaskFlag & pForce->m_dwForceFlag);
}

bool CUnitForce::isEnemyOf(const CUnitForce* pForce) const
{
    return !isAllyOf(pForce);
}

bool CUnitForce::isEffective(const CUnitForce* pForce, uint32_t dwEffectiveTypeFlags) const
{
    return ((this == pForce) && (dwEffectiveTypeFlags & CUnitForce::kSelf)) ||
           ((this != pForce) && (this->isAllyOf(pForce) && (dwEffectiveTypeFlags & CUnitForce::kAlly))) ||
           (this->isEnemyOf(pForce) && (dwEffectiveTypeFlags & CUnitForce::kEnemy));
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
    
    return NULL;
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

CCommandTarget::CCommandTarget(const CPoint& rTargetPoint)
: m_eTargetType(kPointTarget)
, m_oTargetPoint(rTargetPoint)
, m_iTargetUnit(0)
{
}

CCommandTarget::CCommandTarget(int iTargetUnit)
: m_eTargetType(kUnitTarget)
, m_iTargetUnit(iTargetUnit)
{
}

// CUnitEventAdapter
CUnitEventAdapter::CUnitEventAdapter()
: m_pNotifyUnit(NULL)
{
}

CUnitEventAdapter::~CUnitEventAdapter()
{
}

// CDefaultAI
void CDefaultAI::onUnitTick( float dt )
{
    CUnit* u = getNotifyUnit();
    if (u->isDoingNothing() == false)
    {
        return;
    }

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    if (d == NULL)
    {
        return;
    }

    int atk = u->getAttackAbilityId();
    if (atk == 0)
    {
        return;
    }

    CUnit* t = CUnitGroup::getNearestUnitInRange(u->getWorld(), d->getPosition(), 200.0, (FUNC_UNIT_CONDITION)&CUnitGroup::isLivingEnemyOf, DCAST(u, CUnitForce*));
    if (t == NULL || t->isDead())
    {
        d->stop();
        return;
    }

    d->setCastTarget(CCommandTarget(t->getId()));
    d->cmdCastSpell(atk);
}

CAttackData* CDefaultAI::onUnitAttacked( CAttackData* pAttack, CUnit* pSource )
{
    if (pSource->isDead())
    {
        return pAttack;
    }

    CUnit* u = getNotifyUnit();
    if (u->isDoingOr(CUnit::kCasting | CUnit::kObstinate))
    {
        // 正在施法，或者处于固执状态
        return pAttack;
    }

    int atk = u->getAttackAbilityId();
    if (atk == 0)
    {
        return pAttack;
    }

    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    if (d == NULL)
    {
        return pAttack;
    }

    d->setCastTarget(CCommandTarget(pSource->getId()));
    d->cmdCastSpell(atk, false);

    return pAttack;
}

// CUnit
CUnit::CUnit(const char* pRootId)
: CONST_ROOT_ID(pRootId)
, m_pWorld(NULL)
, m_fHp(1.001f)
, m_fMaxHp(1.001f)
, m_pAI(NULL)
, m_iAttackAbilityId(0)
, m_iTriggerRefCount(0)
, m_iSuspendRef(0)
, m_bRevivable(false)
, m_dwDoingFlags(0)
, m_pDraw(NULL)
, m_eSpecies(kUnknown)
{
    setDbgClassName("CUnit");
}

CUnit::~CUnit()
{
    delAI();
    setDraw(NULL);
}

const char* CUnit::getDbgTag() const
{
    return getName();
}

CUnit* CUnit::getUnit(int id)
{
    return id != 0 ? getWorld()->getUnit(id) : NULL;
}

void CUnit::abilityCD(CAbility* pAbility)
{
    CWorld* w = getWorld();
    assert(w != NULL);
    w->addAbilityCD(pAbility);
}

void CUnit::updateAbilityCD(int id)
{
    CWorld* w = getWorld();
    assert(w != NULL);
    w->updateAbilityCD(id);
}

bool CUnit::revive(float fHp)
{
    if (isDead())
    {
        m_fHp = min(max(fHp, 1.0f), m_fMaxHp);
        onChangeHp(m_fHp);
        onRevive();
        return true;
    }

    return false;
}

bool CUnit::setHp(float fHp)
{
    if (isDead())
    {
        return false;
    }

    float fOldHp = m_fHp;
    m_fHp = min(fHp, m_fMaxHp);
    if (m_fHp != fOldHp)
    {
        onChangeHp(m_fHp - fOldHp);
    }

    if (m_fHp <= 0)
    {
        //onDying();
    }

    return true;
}

void CUnit::setMaxHp(float fMaxHp)
{
    float fOldMaxHp = m_fMaxHp;
    m_fMaxHp = max(fMaxHp, 1.0f);
    float fNewHp = m_fHp * m_fMaxHp / fOldMaxHp;
    if (fNewHp < 1)
    {
        fNewHp = 1;
    }
    setHp(fNewHp);
}

bool CUnit::isDead() const
{
    return m_fHp <= 0;
}

void CUnit::onChangeLevel(int iChanged)
{
    if (m_pAI)
    {
        m_pAI->onUnitChangeLevel(iChanged);
    }
}

void CUnit::onRevive()
{
    triggerOnRevive();
    
    if (m_pAI)
    {
        m_pAI->onUnitRevive();
    }
}

void CUnit::onDying()
{
    CUnitDraw* d = getDraw();
    if (d != NULL)
    {
        d->onUnitDying();
    }

    triggerOnDying();
    
    if (m_pAI)
    {
        m_pAI->onUnitDying();
    }
}

void CUnit::onDead()
{
    triggerOnDead();

    if (m_pAI)
    {
        m_pAI->onUnitDead();
    }
}

void CUnit::onChangeHp(float fChanged)
{
    triggerOnChangeHp(fChanged);
    
    if (m_pAI)
    {
        m_pAI->onUnitChangeHp(fChanged);
    }
}

void CUnit::step(float dt)
{
    //m_oActMgr.onTick(dt);
    
    updateBuffAbilityElapsed(dt);
    
    onTick(dt);
}

void CUnit::onTick(float dt)
{
    CUnitDraw* d = getDraw();
    if (d != NULL)
    {
        d->onUnitTick(dt);
    }

    triggerOnTick(dt);
    
    if (m_pAI)
    {
        m_pAI->onUnitTick(dt);
    }
}

CAttackData* CUnit::onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kAttackTargetTrigger))
    {
        pAttack = triggerOnAttackTarget(pAttack, pTarget);
    }
    
    if (m_pAI)
    {
        m_pAI->onUnitAttackTarget(pAttack, pTarget);
    }
    
    return pAttack;
}

CAttackData* CUnit::onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kAttackedTrigger))
    {
        pAttack = triggerOnAttacked(pAttack, pSource);;
    }
    
    if (m_pAI)
    {
        m_pAI->onUnitAttacked(pAttack, pSource);
    }
    
    return pAttack;
}

void CUnit::onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kDamagedSurfaceTrigger))
    {
        triggerOnDamagedSurface(pAttack, pSource);
    }

    if (!(dwTriggerMask & kDamagedInnerTrigger))
    {
        triggerOnDamagedInner(pAttack, pSource);
    }
    
    if (m_pAI)
    {
        m_pAI->onUnitDamaged(pAttack, pSource);
    }
}

void CUnit::onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kDamagedDoneTrigger))
    {
        triggerOnDamagedDone(fDamage, pSource);
    }
    
    LOG("%s受到%d点伤害", getName(), toInt(fDamage));
    LOG("%s HP: %d/%d\n", getName(), toInt(getHp()), toInt(getMaxHp()));

    if (m_pAI)
    {
        m_pAI->onUnitDamagedDone(fDamage, pSource);
    }

#ifdef DEBUG_FOR_CC
    // for cocos2dx
    CUnitDrawForCC* ccd = NULL;
    getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "-%d", toInt(fDamage));
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(255, 0, 0));
    }
#endif
}

void CUnit::onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (!(dwTriggerMask & kDamageTargetDoneTrigger))
    {
        triggerOnDamageTargetDone(fDamage, pTarget);
    }
    
    if (m_pAI)
    {
        m_pAI->onUnitDamageTargetDone(fDamage, pTarget);
    }
}

void CUnit::onProjectileEffect(CProjectile* pProjectile)
{
    triggerOnProjectileEffect(pProjectile);
    
    if (m_pAI)
    {
        m_pAI->onUnitProjectileEffect(pProjectile);
    }
}

void CUnit::onAddActiveAbility(CActiveAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddActiveAbility(pAbility);
    }
}

void CUnit::onDelActiveAbility(CActiveAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelActiveAbility(pAbility);
    }
}

void CUnit::onAddPassiveAbility(CPassiveAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddPassiveAbility(pAbility);
    }
}

void CUnit::onDelPassiveAbility(CPassiveAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelPassiveAbility(pAbility);
    }
}

void CUnit::onAddBuffAbility(CBuffAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddBuffAbility(pAbility);
    }
}

void CUnit::onDelBuffAbility(CBuffAbility* pAbility)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelBuffAbility(pAbility);
    }
}

void CUnit::onAbilityReady(CAbility* pAbility)
{
    pAbility->onUnitAbilityReady();
    
    if (m_pAI)
    {
        m_pAI->onUnitAbilityReady(pAbility);
    }
}

void CUnit::onAddItem(int iIndex)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddItem(iIndex);
    }
}

void CUnit::onDelItem(int iIndex)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelItem(iIndex);
    }
}

void CUnit::delAI()
{
    if (m_pAI != NULL)
    {
        delete m_pAI;
        m_pAI = NULL;
    }
}

CAttackData* CUnit::attackAdv(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    return attackMid(pAttack, pTarget, dwTriggerMask);
}

CAttackData* CUnit::attackMid(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    pAttack = onAttackTarget(pAttack, pTarget, dwTriggerMask);
    return attackBot(pAttack, pTarget, dwTriggerMask);
}

CAttackData* CUnit::attackBot(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    return pAttack;
}

bool CUnit::damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    pAttack = onAttacked(pAttack, pSource, dwTriggerMask);
    if (!pAttack)
    {
        return false;
    }
    damagedMid(pAttack, pSource, dwTriggerMask);
    return true;
}

void CUnit::damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    onDamaged(pAttack, pSource, dwTriggerMask);

    //transformDamageByAttribute(pAttack);
    float fDamage = calcDamage((CAttackValue::ATTACK_TYPE)pAttack->getAttackValue().getType(),
                               pAttack->getAttackValue().getValue(),
                               (CArmorValue::ARMOR_TYPE)getBaseArmor().getType(),
                               getRealArmorValue());
    
    damagedBot(fDamage, pSource, dwTriggerMask);
    
    M_VEC_FOREACH(pAttack->getAttackBuffs())
    {
        const CAttackBuff* pAb = &M_VEC_EACH;
        // TODO: copy BUFF from TemplateAbilitys
        addBuffAbility(pAb->getTemplateBuff(), pSource->getId(), pAb->getBuffLevel());
        M_MAP_NEXT;
    }
}

void CUnit::damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (fDamage > m_fHp)
    {
        setHp(0);
    }
    else
    {
        setHp(m_fHp - fDamage);
    }
    
    onDamagedDone(fDamage, pSource, dwTriggerMask);
    
    if (pSource)
    {
        pSource->onDamageTargetDone(fDamage, this, dwTriggerMask);
    }
}

float CUnit::calcDamage(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue)
{
    float fRet;
    if (fArmorValue > 0)
    {
        fRet = fArmorValue * 0.06;
        fRet = 1 - fRet / (fRet + 1);
    }
    else
    {
        fRet = 2 - pow(0.94f, -fArmorValue);
    }
    fRet *= fAttackValue * g_afAttackArmorTable[eArmorType][eAttackType];
    return fRet;
}

void CUnit::addActiveAbility(CActiveAbility* pAbility, bool bNotify)
{
    m_mapActAbilitys.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddActiveAbility(pAbility);
    }
}

void CUnit::addActiveAbility(int id, int iLevel)
{
    CWorld* w = getWorld();
    CActiveAbility* pAbility = NULL;
    w->copyAbility(id)->dcast(pAbility);
    pAbility->setLevel(iLevel);
    addActiveAbility(pAbility);
}

void CUnit::delActiveAbility(int id, bool bNotify)
{
    auto it = m_mapActAbilitys.find(id);
    if (it == m_mapActAbilitys.end())
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
    
    m_mapActAbilitys.erase(it);
    pAbility->release();
}

CActiveAbility* CUnit::getActiveAbility(int id)
{
    return id != 0 ? m_mapActAbilitys.getObject(id) : NULL;
}

void CUnit::addPassiveAbility(CPassiveAbility* pAbility, bool bNotify)
{
    m_mapPasAbilitys.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddPassiveAbility(pAbility);
    }
}

void CUnit::addPassiveAbility(int id, int iLevel)
{
    CWorld* w = getWorld();
    CPassiveAbility* pAbility = NULL;
    w->copyAbility(id)->dcast(pAbility);
    pAbility->setLevel(iLevel);
    addPassiveAbility(pAbility);
}

void CUnit::delPassiveAbility(int id, bool bNotify)
{
    auto it = m_mapPasAbilitys.find(id);
    if (it == m_mapPasAbilitys.end())
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
    
    m_mapPasAbilitys.erase(it);
    pAbility->release();
}

CPassiveAbility* CUnit::getPassiveAbility(int id)
{
    return id != 0 ? m_mapPasAbilitys.getObject(id) : NULL;
}

void CUnit::addBuffAbility(CBuffAbility* pAbility, bool bNotify)
{
    if (pAbility->isStackable() == false)
    {
        M_MAP_FOREACH(m_mapBuffAbilitys)
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
                
                m_mapBuffAbilitys.addObject(pAbility);
                pAbility->onAddToUnit(this);
                addAbilityToTriggers(pAbility);
                
                M_MAP_DEL_CUR_NEXT(m_mapBuffAbilitys);
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
                pBuff->setElapsed(0.0f);
                return;
            }
            M_MAP_NEXT;
#endif
        }
    }
    
    m_mapBuffAbilitys.addObject(pAbility);
    pAbility->onAddToUnit(this);  // 消息传递
    addAbilityToTriggers(pAbility);
    
    if (bNotify)
    {
        onAddBuffAbility(pAbility);
    }
}

void CUnit::addBuffAbility(int id, int iSrcUnit, int iLevel)
{
    CWorld* w = getWorld();
    CBuffAbility* pAbility = NULL;
    w->copyAbility(id)->dcast(pAbility);
    pAbility->setSrcUnit(iSrcUnit);
    pAbility->setLevel(iLevel);
    addBuffAbility(pAbility);
}

void CUnit::delBuffAbility(int id, bool bNotify)
{
    auto it = m_mapBuffAbilitys.find(id);
    if (it == m_mapBuffAbilitys.end())
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
    
    m_mapBuffAbilitys.erase(it);
    pAbility->release();
}

CBuffAbility* CUnit::getBuffAbility(int id)
{
    return id != 0 ? m_mapBuffAbilitys.getObject(id) : NULL;
}

void CUnit::updateBuffAbilityElapsed(float dt)
{
    M_MAP_FOREACH(m_mapBuffAbilitys)
    {
        CBuffAbility* pBuff = M_MAP_EACH;
        pBuff->setElapsed(pBuff->getElapsed() + dt);
        if (pBuff->isDone())
        {
            pBuff->onDelFromUnit();
            delAbilityFromTriggers(pBuff);
            pBuff->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapBuffAbilitys);
        }
        else
        {
            M_MAP_NEXT;
        }
    }
}

void CUnit::addAbilityToTriggers(CAbility* pAbility)
{
    assert(pAbility != NULL);
    uint32_t dwTriggerFlags = pAbility->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerAbilitysToAdd.addObject(pAbility);
        return;
    }
    
    if (dwTriggerFlags & kReviveTrigger)
    {
        m_mapOnReviveTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDyingTrigger)
    {
        m_mapOnDyingTriggerAbilitys.addObject(pAbility);
    }

    if (dwTriggerFlags & kDeadTrigger)
    {
        m_mapOnDeadTriggerAbilitys.addObject(pAbility);
    }

    if (dwTriggerFlags & kChangeHpTrigger)
    {
        m_mapOnChangeHpTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kTickTrigger)
    {
        m_mapOnTickTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kAttackedTrigger)
    {
        m_mapOnAttackedTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kProjectileEffectTrigger)
    {
        m_mapOnProjectileEffectTriggerAbilitys.addObject(pAbility);
    }
}

void CUnit::delAbilityFromTriggers(CAbility* pAbility)
{
    assert(pAbility != NULL);
    uint32_t dwTriggerFlags = pAbility->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerAbilitysToDel.addObject(pAbility);
        return;
    }
    
    int id = pAbility->getId();
    
    if (dwTriggerFlags & kReviveTrigger)
    {
        m_mapOnReviveTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kDyingTrigger)
    {
        m_mapOnDyingTriggerAbilitys.delObject(id);
    }

    if (dwTriggerFlags & kDeadTrigger)
    {
        m_mapOnDeadTriggerAbilitys.delObject(id);
    }

    if (dwTriggerFlags & kChangeHpTrigger)
    {
        m_mapOnChangeHpTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kTickTrigger)
    {
        m_mapOnTickTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kAttackedTrigger)
    {
        m_mapOnAttackedTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerAbilitys.addObject(pAbility);
    }
    
    if (dwTriggerFlags & kDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerAbilitys.delObject(id);
    }
    
    if (dwTriggerFlags & kProjectileEffectTrigger)
    {
        m_mapOnProjectileEffectTriggerAbilitys.delObject(id);
    }
}

void CUnit::updateTriggerAbilitysWhenTriggerFree()
{
    assert(isTriggerFree());
    
    M_MAP_FOREACH(m_mapTriggerAbilitysToAdd)
    {
        CAbility* pAbility = M_MAP_EACH;
        addAbilityToTriggers(pAbility);
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(m_mapTriggerAbilitysToDel)
    {
        CAbility* pAbility = M_MAP_EACH;
        delAbilityFromTriggers(pAbility);
        M_MAP_NEXT;
    }
    
    m_mapTriggerAbilitysToAdd.delAllObjects();
    m_mapTriggerAbilitysToDel.delAllObjects();
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
        updateTriggerAbilitysWhenTriggerFree();
    }
}

bool CUnit::isTriggerFree() const
{
    return m_iTriggerRefCount == 0;
}

void CUnit::triggerOnRevive()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnReviveTriggerAbilitys)
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
    M_MAP_FOREACH(m_mapOnDyingTriggerAbilitys)
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
    M_MAP_FOREACH(m_mapOnDeadTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDead();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnChangeHp(float fChanged)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnChangeHpTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitChangeHp(fChanged);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnTick(float dt)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnTickTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitTick(dt);
        if (pAbility->getInterval() > FLT_EPSILON)
        {
            pAbility->setIntervalElapsed(pAbility->getIntervalElapsed() + dt);

            while (pAbility->getIntervalElapsed() >= pAbility->getInterval())
            {
                pAbility->onUnitInterval();
                pAbility->setIntervalElapsed(pAbility->getIntervalElapsed() - pAbility->getInterval());
            }
        }
        M_MAP_NEXT;
    }
    endTrigger();
}

CAttackData* CUnit::triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnAttackTargetTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAttack = pAbility->onUnitAttackTarget(pAttack, pTarget);
        if (pAttack == NULL)
        {
            break;
        }
        M_MAP_NEXT;
    }
    endTrigger();
    return pAttack;
}

CAttackData* CUnit::triggerOnAttacked(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnAttackedTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAttack = pAbility->onUnitAttacked(pAttack, pSource);
        if (pAttack == NULL)
        {
            break;
        }
        M_MAP_NEXT;
    }
    endTrigger();
    return pAttack;
}

void CUnit::triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedSurfaceTriggerAbilitys)
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
    M_MAP_FOREACH(m_mapOnDamagedInnerTriggerAbilitys)
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
    M_MAP_FOREACH(m_mapOnDamagedDoneTriggerAbilitys)
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
    M_MAP_FOREACH(m_mapOnDamageTargetDoneTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitDamageTargetDone(fDamage, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnProjectileEffect(CProjectile* pProjectile)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnProjectileEffectTriggerAbilitys)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->onUnitProjectileEffect(pProjectile);
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
        assert(!pItem->getActiveAbilitys().empty());
        
        pItem->use();
        pItem->onDelFromSlot();
        return true;
    }
    
    int iFirstEmpty = -1;
    for (int i = 0; i < (int)m_vecItems.size(); ++i)
    {
        CItem* pSlot = m_vecItems[i];
        if (pSlot == NULL)
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
    
    if (m_vecItems[iIndex] == NULL)
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
        return NULL;
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
    if (pItem == NULL)
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

void CUnit::setDraw( CUnitDraw* pDraw )
{
    if (pDraw != m_pDraw)
    {
        if (m_pDraw != NULL)
        {
            m_pDraw->release();
        }
        else
        {
            pDraw->retain();
            pDraw->setUnit(this);
        }

        m_pDraw = pDraw;
    }
}

void CUnit::runAction(CAction* pAction)
{
    m_oActMgr.addAction(pAction, this);
}

CAction* CUnit::getActionByTag(int iTag)
{
    return m_oActMgr.getActionByTag(iTag);
}

// CWorld
CWorld::CWorld()
: m_pLua(NULL)
, m_iControlUnit(0)
{
    setDbgClassName("CWorld");
    
    m_pLua = luaL_newstate();
    luaL_openlibs(m_pLua);
}

CWorld::~CWorld()
{
    lua_close(m_pLua);
}

bool CWorld::loadScript(const char* pName)
{
    std::string code("require \"");
    code.append(pName);
    code.append("\"");
    int nRet = luaL_dostring(m_pLua, code.c_str());
    if (nRet != 0)
    {
        LOG(lua_tostring(m_pLua, -1));
        lua_pop(m_pLua, 1);

        return false;
    }
    
    return true;
}

void CWorld::addScriptSearchPath( const char* pPath )
{
    lua_getglobal(m_pLua, "package");
    lua_getfield(m_pLua, -1, "path");
    const char* cur_path =  lua_tostring(m_pLua, -1);
    lua_pushfstring(m_pLua, "%s;%s/?.lua", cur_path, pPath);
    lua_setfield(m_pLua, -3, "path");
    lua_pop(m_pLua, 2);
}

bool CWorld::onInit()
{
    return true;
}

void CWorld::onTick(float dt)
{
}

void CWorld::onAddUnit( CUnit* pUnit )
{
}

void CWorld::onDelUnit( CUnit* pUnit )
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
    return onInit();
}

void CWorld::addUnit(CUnit* pUnit)
{
    onAddUnit(pUnit);

    pUnit->setWorld(this);
    m_mapUnits.addObject(pUnit);
}

void CWorld::delUnit( int id, bool bRevivable /*= false*/ )
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
        pUnit->setWorld(NULL);
        cleanAbilitysCD(pUnit);
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
    if (pUnit == NULL)
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
    if (!pAbility->isCoolingDown())
    {
        return;
    }
    m_mapAbilitysCD.addObject(pAbility);
    LOG("%s的%s技能开始冷却(%.1fs)", pAbility->getOwner()->getName(), pAbility->getName(), pAbility->getCoolDown());
}

void CWorld::delAbilityCD(int id)
{
    m_mapAbilitysCD.delObject(id);
}

bool CWorld::isAbilityCD(int id) const
{
    return m_mapAbilitysCD.find(id) != m_mapAbilitysCD.end();
}

CAbility* CWorld::getAbilityCD(int id) const
{
    return m_mapAbilitysCD.getObject(id);
}

void CWorld::updateAbilityCD(int id)
{
    auto it = m_mapAbilitysCD.find(id);
    if (it == m_mapAbilitysCD.end())
    {
        return;
    }
    
    CAbility* pAbility = it->second;
    if (pAbility->isCoolingDown())
    {
        return;
    }
    
    m_mapAbilitysCD.erase(it);
    abilityReady(pAbility);
    pAbility->release();
}

void CWorld::cleanAbilitysCD(CUnit* pUnit)
{
    M_MAP_FOREACH(pUnit->getActiveAbilitys())
    {
        CActiveAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitysCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getPassiveAbilitys())
    {
        CPassiveAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitysCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getBuffAbilitys())
    {
        CBuffAbility* pAbility = M_MAP_EACH;
        if (pAbility->isCoolingDown())
        {
            m_mapAbilitysCD.delObject(pAbility->getId());
        }
        M_MAP_NEXT;
    }
}

void CWorld::abilityReady(CAbility* pAbility)
{
    // 由于技能的所有者可能在等待重生，所以主世界可能不存在该单位，但是单位仍未被释放
    CUnit* o = getUnit(pAbility->getOwner()->getId());
    if (o != NULL)
    {
        // 存在于主世界中，则触发事件
        o->onAbilityReady(pAbility);
        // 防止BUFF更改CD而导技能在致脱离CD管理器后CD大于Elapsed
        pAbility->setCoolingDownElapsed(FLT_MAX);
    }
}

void CWorld::step(float dt)
{
    // 单位死亡后技能CD独立计算，所以放在此处独立计算，不整合到单位onTick中
    M_MAP_FOREACH(m_mapAbilitysCD)
    {
        CAbility* pAbility = M_MAP_EACH;
        pAbility->setCoolingDownElapsed(pAbility->getCoolingDownElapsed() + dt);
        if (!pAbility->isCoolingDown())
        {
            // 如果技能已经就绪，从中删除
            abilityReady(pAbility);
            pAbility->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapAbilitysCD);
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
    m_mapTemplateAbilitys.addObject(pAbility);
    return pAbility->getId();
}

void CWorld::loadTemplateAbilitys()
{
}

CAbility* CWorld::copyAbility(int id) const
{
    CAbility* pAbility = m_mapTemplateAbilitys.getObject(id);
    if (pAbility == NULL)
    {
        return NULL;
    }
    
    return pAbility->copy()->dcast(pAbility);  // 即时转换失败也不需要释放，因为有CAutoReleasePool
}

int CWorld::addTemplateProjectile( CProjectile* pProjectile )
{
    m_mapTemplateProjectiles.addObject(pProjectile);
    return pProjectile->getId();
}

CProjectile* CWorld::copyProjectile(int id) const
{
    CProjectile* pProjectile = m_mapTemplateProjectiles.getObject(id);
    if (pProjectile == NULL)
    {
        return NULL;
    }

    return pProjectile->copy()->dcast(pProjectile);  // 即时转换失败也不需要释放，因为有CAutoReleasePool
}

void CWorld::addProjectile( CProjectile* pProjectile )
{
    onAddProjectile(pProjectile);

    pProjectile->setWorld(this);
    m_mapProjectiles.addObject(pProjectile);
}


void CWorld::delProjectile( int id )
{
    auto it = m_mapProjectiles.find(id);
    if (it == m_mapProjectiles.end())
    {
        return;
    }

    CProjectile* pProjectile = it->second;
    onDelProjectile(pProjectile);

    pProjectile->setWorld(NULL);
    pProjectile->release();
    m_mapProjectiles.erase(it);
}
