/* 
 * File:   Unit.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午10:55
 */

#include "CommInc.h"
#include "Unit.h"
#include "Skill.h"
#include "Action.h"
#include "Application.h"
#include "Item.h"
#include "Draw.h"


// CAttackValue
const char* CAttackValue::CONST_ARR_NAME[CONST_MAX_ATTACK_TYPE][CONST_MAX_NAME_INDEX] = {
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

CAttackValue::CAttackValue()
{
    setValueZero();
}

CAttackValue::CAttackValue(int iCount, ATTACK_TYPE eType1, float fValue1, ...)
{
    setValueZero();
    setValue(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (ATTACK_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setValue(eType1, fValue1);
    }
    va_end(argv);
}

float CAttackValue::getValue(ATTACK_TYPE eType) const
{
    if (eType > CONST_MAX_ATTACK_TYPE)
    {
        return 0;
    }
    return m_afValues[eType];
}

void CAttackValue::setValue(ATTACK_TYPE eType, float fValue)
{
    if (eType > CONST_MAX_ATTACK_TYPE)
    {
        return;
    }
    m_afValues[eType] = fValue;
}

void CAttackValue::setAttackValue(const CAttackValue& roAttackValue)
{
    memmove(m_afValues, roAttackValue.m_afValues, sizeof(m_afValues));
}

void CAttackValue::setValueZero()
{
    memset(m_afValues, 0, sizeof(m_afValues));
}

const char* CAttackValue::getName(ATTACK_TYPE eType, int iIndex)
{
    return CONST_ARR_NAME[eType][iIndex];
}

const CAttackValue& CAttackValue::operator=(const CAttackValue& roAttackValue)
{
    setAttackValue(roAttackValue);
    return *this;
}

const char* CArmorValue::CONST_ARR_NAME[CONST_MAX_ARMOR_TYPE][CONST_MAX_NAME_INDEX] = {
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

// CArmorValue
CArmorValue::CArmorValue()
{
    setValueZero();
}

CArmorValue::CArmorValue(int iCount, ARMOR_TYPE eType1, float fValue1, ...)
{
    setValueZero();
    setValue(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (ARMOR_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setValue(eType1, fValue1);
    }
    va_end(argv);
}

float CArmorValue::getValue(ARMOR_TYPE eType) const
{
    if (eType > CONST_MAX_ARMOR_TYPE)
    {
        return 0;
    }
    return m_afValues[eType];
}

void CArmorValue::setValue(ARMOR_TYPE eType, float fValue)
{
    if (eType > CONST_MAX_ARMOR_TYPE)
    {
        return;
    }
    m_afValues[eType] = fValue;
}

void CArmorValue::setArmorValue(const CArmorValue& roArmorValue)
{
    memmove(m_afValues, roArmorValue.m_afValues, sizeof(m_afValues));
}

void CArmorValue::setValueZero()
{
    memset(m_afValues, 0, sizeof(m_afValues));
}

const char* CArmorValue::getName(ARMOR_TYPE eType, int iIndex)
{
    return CONST_ARR_NAME[eType][iIndex];
}

const CArmorValue& CArmorValue::operator=(const CArmorValue& roArmorValue)
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
    m_oAtkVal.setValue(eAttackType, fAttackValue);
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

// CUnit
CUnit::CUnit(const char* pRootId)
: CONST_ROOT_ID(pRootId)
, m_pWorld(NULL)
, m_fHp(1.001f)
, m_fMaxHp(1.001f)
, m_pAI(NULL)
, m_iAttackSkillId(0)
, m_iTriggerRefCount(0)
, m_iSuspendRef(0)
, m_eArmorType(CArmorValue::kNormal)
, m_fBaseArmorValue(0.0f)
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

void CUnit::skillCD(CSkill* pSkill)
{
    LOG("%s的%s技能开始冷却(%.1fs)", getName(), pSkill->getName(), pSkill->getCoolDown());
    getWorld()->addSkillCD(pSkill);
}

void CUnit::updateSkillCD(int id)
{
    getWorld()->updateSkillCD(id);
}

bool CUnit::revive(float fHp)
{
    if (isDead())
    {
        m_fHp = min(max(fHp, 1.0f), m_fMaxHp);
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
        onDie();
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

void CUnit::onDie()
{
    triggerOnDie();
    
    if (m_pAI)
    {
        m_pAI->onUnitDie();
    }
}

void CUnit::onChangeHp(float fChanged)
{
    triggerOnHpChange(fChanged);
    
    if (m_pAI)
    {
        m_pAI->onUnitChangeHp(fChanged);
    }
}

void CUnit::step(float dt)
{
    m_oActMgr.onTick(dt);
    
    updateBuffSkillElapsed(dt);
    
    onTick(dt);
}

void CUnit::onTick(float dt)
{
    triggerOnTick(dt);
    
    if (m_pAI)
    {
        m_pAI->onUnitTick(dt);
    }
}

CAttackData* CUnit::onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (dwTriggerMask & kAttackTargetTrigger)
    {
        return pAttack;
    }
    pAttack = triggerOnAttackTarget(pAttack, pTarget);
    
    if (m_pAI)
    {
        m_pAI->onUnitAttackTarget(pAttack, pTarget);
    }
    
    return pAttack;
}

CAttackData* CUnit::onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (dwTriggerMask & kAttackedTrigger)
    {
        return pAttack;
    }
    pAttack = triggerOnAttacked(pAttack, pSource);
    
    if (m_pAI)
    {
        m_pAI->onUnitAttacked(pAttack, pSource);
    }
    
    return pAttack;
}

void CUnit::onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (dwTriggerMask & kDamagedSurfaceTrigger)
    {
        return;
    }
    triggerOnDamagedSurface(pAttack, pSource);

    if (dwTriggerMask & kDamagedInnerTrigger)
    {
        return;
    }
    triggerOnDamagedInner(pAttack, pSource);
    
    if (m_pAI)
    {
        m_pAI->onUnitDamaged(pAttack, pSource);
    }
}

void CUnit::onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (dwTriggerMask & kDamagedDoneTrigger)
    {
        return;
    }
    triggerOnDamagedDone(fDamage, pSource);
    
    LOG("%s受到%d点伤害", getName(), toInt(fDamage));
    LOG("%s HP: %d/%d\n", getName(), toInt(getHp()), toInt(getMaxHp()));
    
    if (m_pAI)
    {
        m_pAI->onUnitDamagedDone(fDamage, pSource);
    }
}

void CUnit::onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (dwTriggerMask & kDamageTargetDoneTrigger)
    {
        return;
    }
    triggerOnDamageTargetDone(fDamage, pTarget);
    
    if (m_pAI)
    {
        m_pAI->onUnitDamageTargetDone(fDamage, pTarget);
    }
}

void CUnit::onDestroyProjectile(CProjectile* pProjectile)
{
    triggerOnDestroyProjectile(pProjectile);
    
    if (m_pAI)
    {
        m_pAI->onUnitDestroyProjectile(pProjectile);
    }
}

void CUnit::onAddActiveSkill(CActiveSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddActiveSkill(pSkill);
    }
}

void CUnit::onDelActiveSkill(CActiveSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelActiveSkill(pSkill);
    }
}

void CUnit::onAddPassiveSkill(CPassiveSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddPassiveSkill(pSkill);
    }
}

void CUnit::onDelPassiveSkill(CPassiveSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelPassiveSkill(pSkill);
    }
}

void CUnit::onAddBuffSkill(CBuffSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitAddBuffSkill(pSkill);
    }
}

void CUnit::onDelBuffSkill(CBuffSkill* pSkill)
{
    if (m_pAI)
    {
        m_pAI->onUnitDelBuffSkill(pSkill);
    }
}

void CUnit::onSkillReady(CSkill* pSkill)
{
    pSkill->onUnitSkillReady();
    
    if (m_pAI)
    {
        m_pAI->onUnitSkillReady(pSkill);
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
    float fDamage = 0;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; i++)
    {
        fDamage += calcDamage((CAttackValue::ATTACK_TYPE)i, pAttack->getAttackValue().getValue((CAttackValue::ATTACK_TYPE)i), m_eArmorType, getRealArmorValue());
    }

    damagedBot(fDamage, pSource, dwTriggerMask);
    
    M_VEC_FOREACH(pAttack->getAttackBuffs())
    {
        const CAttackBuff* pAb = &M_VEC_EACH;
        // TODO: copy BUFF from TemplateSkills
        addBuffSkill(pAb->getTemplateBuff(), pSource->getId(), pAb->getBuffLevel());
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

void CUnit::addActiveSkill(CActiveSkill* pSkill, bool bNotify)
{
    m_mapActSkills.addObject(pSkill);
    pSkill->onAddToUnit(this);  // 消息传递
    addSkillToTriggers(pSkill);
    
    if (bNotify)
    {
        onAddActiveSkill(pSkill);
    }
}

void CUnit::addActiveSkill(int id, int iLevel)
{
    CWorld* w = getWorld();
    CActiveSkill* pSkill = NULL;
    w->copySkill(id)->dcast(pSkill);
    pSkill->setLevel(iLevel);
    addActiveSkill(pSkill);
}

void CUnit::delActiveSkill(int id, bool bNotify)
{
    auto it = m_mapActSkills.find(id);
    if (it == m_mapActSkills.end())
    {
        return;
    }
    
    CActiveSkill* pSkill = it->second;
    
    if (bNotify)
    {
        onDelActiveSkill(pSkill);
    }
    
    if (pSkill->isCoolingDown())
    {
        getWorld()->delSkillCD(id);
    }
    
    pSkill->onDelFromUnit();
    delSkillFromTriggers(pSkill);
    
    m_mapActSkills.erase(it);
    pSkill->release();
}

CActiveSkill* CUnit::getActiveSkill(int id)
{
    return id != 0 ? m_mapActSkills.getObject(id) : NULL;
}

void CUnit::addPassiveSkill(CPassiveSkill* pSkill, bool bNotify)
{
    m_mapPasSkills.addObject(pSkill);
    pSkill->onAddToUnit(this);  // 消息传递
    addSkillToTriggers(pSkill);
    
    if (bNotify)
    {
        onAddPassiveSkill(pSkill);
    }
}

void CUnit::addPassiveSkill(int id, int iLevel)
{
    CWorld* w = getWorld();
    CPassiveSkill* pSkill = NULL;
    w->copySkill(id)->dcast(pSkill);
    pSkill->setLevel(iLevel);
    addPassiveSkill(pSkill);
}

void CUnit::delPassiveSkill(int id, bool bNotify)
{
    auto it = m_mapPasSkills.find(id);
    if (it == m_mapPasSkills.end())
    {
        return;
    }
    
    CPassiveSkill* pSkill = it->second;
    
    if (bNotify)
    {
        onDelPassiveSkill(pSkill);
    }
    
    if (pSkill->isCoolingDown())
    {
        getWorld()->delSkillCD(id);
    }
    
    pSkill->onDelFromUnit();
    delSkillFromTriggers(pSkill);
    
    m_mapPasSkills.erase(it);
    pSkill->release();
}

CPassiveSkill* CUnit::getPassiveSkill(int id)
{
    return id != 0 ? m_mapPasSkills.getObject(id) : NULL;
}

void CUnit::addBuffSkill(CBuffSkill* pSkill, bool bNotify)
{
    if (pSkill->isStackable() == false)
    {
        M_MAP_FOREACH(m_mapBuffSkills)
        {
            CBuffSkill* pBuff = M_MAP_EACH;
            if (strcmp(pBuff->getRootId(), pSkill->getRootId()) == 0)
#if 0
            {
                if (pBuff->isCoolingDown())
                {
                    getWorld()->delSkillCD(pBuff->getId());
                }

                pBuff->onDelFromUnit();
                delSkillFromTriggers(pBuff);
                pBuff->release();
                
                m_mapBuffSkills.addObject(pSkill);
                pSkill->onAddToUnit(this);
                addSkillToTriggers(pSkill);
                
                M_MAP_DEL_CUR_NEXT(m_mapBuffSkills);
                return;
            }
            else
            {
                M_MAP_NEXT;
            }
#else
            {
                pBuff->setSrcUnit(pSkill->getSrcUnit());
                pBuff->setLevel(pSkill->getLevel());
                pBuff->setName(pSkill->getName());
                pBuff->setElapsed(0.0f);
                return;
            }
            M_MAP_NEXT;
#endif
        }
    }
    
    m_mapBuffSkills.addObject(pSkill);
    pSkill->onAddToUnit(this);  // 消息传递
    addSkillToTriggers(pSkill);
    
    if (bNotify)
    {
        onAddBuffSkill(pSkill);
    }
}

void CUnit::addBuffSkill(int id, int iSrcUnit, int iLevel)
{
    CWorld* w = getWorld();
    CBuffSkill* pSkill = NULL;
    w->copySkill(id)->dcast(pSkill);
    pSkill->setSrcUnit(iSrcUnit);
    pSkill->setLevel(iLevel);
    addBuffSkill(pSkill);
}

void CUnit::delBuffSkill(int id, bool bNotify)
{
    auto it = m_mapBuffSkills.find(id);
    if (it == m_mapBuffSkills.end())
    {
        return;
    }
    
    CBuffSkill* pSkill = it->second;
    
    if (bNotify)
    {
        onDelBuffSkill(pSkill);
    }
    
    if (pSkill->isCoolingDown())
    {
        getWorld()->delSkillCD(id);
    }
    
    pSkill->onDelFromUnit();
    delSkillFromTriggers(pSkill);
    
    m_mapBuffSkills.erase(it);
    pSkill->release();
}

CBuffSkill* CUnit::getBuffSkill(int id)
{
    return id != 0 ? m_mapBuffSkills.getObject(id) : NULL;
}

void CUnit::updateBuffSkillElapsed(float dt)
{
    M_MAP_FOREACH(m_mapBuffSkills)
    {
        CBuffSkill* pBuff = M_MAP_EACH;
        pBuff->setElapsed(pBuff->getElapsed() + dt);
        if (pBuff->isDone())
        {
            pBuff->onDelFromUnit();
            delSkillFromTriggers(pBuff);
            pBuff->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapBuffSkills);
        }
        else
        {
            M_MAP_NEXT;
        }
    }
}

void CUnit::addSkillToTriggers(CSkill* pSkill)
{
    assert(pSkill != NULL);
    uint32_t dwTriggerFlags = pSkill->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerSkillsToAdd.addObject(pSkill);
        return;
    }
    
    if (dwTriggerFlags & kReviveTrigger)
    {
        m_mapOnReviveTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDieTrigger)
    {
        m_mapOnDieTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kHpChangeTrigger)
    {
        m_mapOnHpChangeTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kTickTrigger)
    {
        m_mapOnTickTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kAttackedTrigger)
    {
        m_mapOnAttackedTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDestroyProjectileTrigger)
    {
        m_mapOnDestroyProjectileTriggerSkills.addObject(pSkill);
    }
}

void CUnit::delSkillFromTriggers(CSkill* pSkill)
{
    assert(pSkill != NULL);
    uint32_t dwTriggerFlags = pSkill->getTriggerFlags();
    if (dwTriggerFlags == 0)
    {
        return;
    }
    
    if (isTriggerFree() == false)
    {
        m_mapTriggerSkillsToDel.addObject(pSkill);
        return;
    }
    
    int id = pSkill->getId();
    
    if (dwTriggerFlags & kReviveTrigger)
    {
        m_mapOnReviveTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kDieTrigger)
    {
        m_mapOnDieTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kHpChangeTrigger)
    {
        m_mapOnHpChangeTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kTickTrigger)
    {
        m_mapOnTickTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kAttackTargetTrigger)
    {
        m_mapOnAttackTargetTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kAttackedTrigger)
    {
        m_mapOnAttackedTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedSurfaceTrigger)
    {
        m_mapOnDamagedSurfaceTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedInnerTrigger)
    {
        m_mapOnDamagedInnerTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kDamagedDoneTrigger)
    {
        m_mapOnDamagedDoneTriggerSkills.addObject(pSkill);
    }
    
    if (dwTriggerFlags & kDamageTargetDoneTrigger)
    {
        m_mapOnDamageTargetDoneTriggerSkills.delObject(id);
    }
    
    if (dwTriggerFlags & kDestroyProjectileTrigger)
    {
        m_mapOnDestroyProjectileTriggerSkills.delObject(id);
    }
}

void CUnit::updateTriggerSkillsWhenTriggerFree()
{
    assert(isTriggerFree());
    
    M_MAP_FOREACH(m_mapTriggerSkillsToAdd)
    {
        CSkill* pSkill = M_MAP_EACH;
        addSkillToTriggers(pSkill);
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(m_mapTriggerSkillsToDel)
    {
        CSkill* pSkill = M_MAP_EACH;
        delSkillFromTriggers(pSkill);
        M_MAP_NEXT;
    }
    
    m_mapTriggerSkillsToAdd.delAllObjects();
    m_mapTriggerSkillsToDel.delAllObjects();
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
        updateTriggerSkillsWhenTriggerFree();
    }
}

bool CUnit::isTriggerFree() const
{
    return m_iTriggerRefCount == 0;
}

void CUnit::triggerOnRevive()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnReviveTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitRevive();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDie()
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDieTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDie();
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnHpChange(float fChanged)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnHpChangeTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitChangeHp(fChanged);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnTick(float dt)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnTickTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitTick(dt);
        if (pSkill->getInterval() > FLT_EPSILON)
        {
            pSkill->setIntervalElapsed(pSkill->getIntervalElapsed() + dt);

            while (pSkill->getIntervalElapsed() >= pSkill->getInterval())
            {
                pSkill->onUnitInterval();
                pSkill->setIntervalElapsed(pSkill->getIntervalElapsed() - pSkill->getInterval());
            }
        }
        M_MAP_NEXT;
    }
    endTrigger();
}

CAttackData* CUnit::triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnAttackTargetTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pAttack = pSkill->onUnitAttackTarget(pAttack, pTarget);
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
    M_MAP_FOREACH(m_mapOnAttackedTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pAttack = pSkill->onUnitAttacked(pAttack, pSource);
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
    M_MAP_FOREACH(m_mapOnDamagedSurfaceTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDamaged(pAttack, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamagedInner(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedInnerTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDamaged(pAttack, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamagedDone(float fDamage, CUnit* pSource)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamagedDoneTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDamagedDone(fDamage, pSource);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDamageTargetDone(float fDamage, CUnit* pTarget)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDamageTargetDoneTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDamageTargetDone(fDamage, pTarget);
        M_MAP_NEXT;
    }
    endTrigger();
}

void CUnit::triggerOnDestroyProjectile(CProjectile* pProjectile)
{
    beginTrigger();
    M_MAP_FOREACH(m_mapOnDestroyProjectileTriggerSkills)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->onUnitDestroyProjectile(pProjectile);
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
    return m_oExArmorValue.getValue(m_fBaseArmorValue);
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
        assert(!pItem->getActiveSkills().empty());
        
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

// CProjectile 
CProjectile::CProjectile(const char* pRootId)
: CUnit(pRootId)
, m_iSourceUnit(0)
, m_iStartUnit(0)
, m_iTargetUnit(0)
{
    setDbgClassName("CProjectile");
}

CProjectile::~CProjectile()
{
}

bool CProjectile::hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const
{
    return (m_dwPenaltyFlags & (uint32_t)ePenaltyType) != 0;
}

// CWorld
CWorld::CWorld()
{
    setDbgClassName("CWorld");
}

CWorld::~CWorld()
{
}

void CWorld::onInit()
{
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

void CWorld::init()
{
    onInit();
}

void CWorld::addUnit(CUnit* pUnit)
{
    onAddUnit(pUnit);

    pUnit->setWorld(this);
    m_mapUnits.addObject(pUnit);
}

void CWorld::delUnit(int id)
{
    auto it = m_mapUnits.find(id);
    if (it == m_mapUnits.end())
    {
        return;
    }
    
    delUnit(it, false);
}

void CWorld::delUnit( MAP_UNITS::iterator it, bool bRevivable /*= false*/ )
{
    onDelUnit(it->second);

    CUnit* pDel = it->second;
    if (bRevivable)
    {
        // 如果单位可以复活，拖进灵魂域
        m_mapUnitsToRevive.addObject(pDel);
    }
    else
    {
        // 如果不可以复活，该单位将不再拥有世界，清除该单位的所有CD中的技能
        pDel->setWorld(NULL);
        cleanSkillsCD(pDel);
    }

    pDel->release();
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

void CWorld::addSkillCD(CSkill* pSkill)
{
    if (!pSkill->isCoolingDown())
    {
        return;
    }
    m_mapSkillsCD.addObject(pSkill);
}

void CWorld::delSkillCD(int id)
{
    m_mapSkillsCD.delObject(id);
}

bool CWorld::isSkillCD(int id) const
{
    return m_mapSkillsCD.find(id) != m_mapSkillsCD.end();
}

CSkill* CWorld::getSkillCD(int id) const
{
    return m_mapSkillsCD.getObject(id);
}

void CWorld::updateSkillCD(int id)
{
    auto it = m_mapSkillsCD.find(id);
    if (it == m_mapSkillsCD.end())
    {
        return;
    }
    
    CSkill* pSkill = it->second;
    if (pSkill->isCoolingDown())
    {
        return;
    }
    
    m_mapSkillsCD.erase(it);
    skillReady(pSkill);
    pSkill->release();
}

void CWorld::cleanSkillsCD(CUnit* pUnit)
{
    M_MAP_FOREACH(pUnit->getActiveSkills())
    {
        CActiveSkill* pSkill = M_MAP_EACH;
        if (pSkill->isCoolingDown())
        {
            m_mapSkillsCD.delObject(pSkill->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getPassiveSkills())
    {
        CPassiveSkill* pSkill = M_MAP_EACH;
        if (pSkill->isCoolingDown())
        {
            m_mapSkillsCD.delObject(pSkill->getId());
        }
        M_MAP_NEXT;
    }
    
    M_MAP_FOREACH(pUnit->getBuffSkills())
    {
        CBuffSkill* pSkill = M_MAP_EACH;
        if (pSkill->isCoolingDown())
        {
            m_mapSkillsCD.delObject(pSkill->getId());
        }
        M_MAP_NEXT;
    }
}

void CWorld::skillReady(CSkill* pSkill)
{
    // 由于技能的所有者可能在等待重生，所以主世界可能不存在该单位，但是单位仍未被释放
    CUnit* o = getUnit(pSkill->getOwner()->getId());
    if (o != NULL)
    {
        // 存在于主世界中，则触发事件
        o->onSkillReady(pSkill);
    }
}

void CWorld::step(float dt)
{
    // 单位死亡后技能CD独立计算，所以放在此处独立计算，不整合到单位onTick中
    M_MAP_FOREACH(m_mapSkillsCD)
    {
        CSkill* pSkill = M_MAP_EACH;
        pSkill->setCoolingDownElapsed(pSkill->getCoolingDownElapsed() + dt);
        if (!pSkill->isCoolingDown())
        {
            // 如果技能已经就绪，从中删除
            skillReady(pSkill);
            pSkill->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapSkillsCD);
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
        
        if (pUnit->isDead())
        {
            // 单位已经死亡
#if 1
            delUnit(it++, pUnit->isRevivable());
#else
            if (pUnit->isRevivable())
            {
                // 如果单位可以复活，拖进灵魂域
                m_mapUnitsToRevive.addObject(pUnit);
            }
            else
            {
                // 如果不可以复活，该单位将不再拥有世界，清除该单位的所有CD中的技能
                pUnit->setWorld(NULL);
                cleanSkillsCD(pUnit);
            }
            pUnit->release();
            
            M_MAP_DEL_CUR_NEXT(m_mapUnits);
#endif
        }
        else
        {
            M_MAP_NEXT;
        }
    }
    
    onTick(dt);
}

int CWorld::addTemplateSkill(CSkill* pSkill)
{
    m_mapTemplateSkills.addObject(pSkill);
    return pSkill->getId();
}

void CWorld::loadTemplateSkills()
{
}

CSkill* CWorld::copySkill(int id) const
{
    CSkill* pSkill = m_mapTemplateSkills.getObject(id);
    if (pSkill == NULL)
    {
        return NULL;
    }
    
    return pSkill->copy()->dcast(pSkill);  // 即时转换失败也不需要释放，因为有CAutoReleasePool
}

