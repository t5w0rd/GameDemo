/* 
 * File:   wAbility.cpp
 * Author: wwww
 * 
 * Created on 2014年2月19日, 下午9:25
 */
#include <stdlib.h>

#include "CommInc.h"

#include "Unit.h"
#include "BingAbility.h"
#include "MultiRefObject.h"
#include "Application.h"
#include "Draw.h"


CEvadeBuff::CEvadeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance)
    :CBuffAbility(pRootId, pName, fDuration, bStackable),m_fChance(fChance)
{
    
}

CMultiRefObject* CEvadeBuff::copy() const
{
    CEvadeBuff* pRet = new CEvadeBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fChance);
    return pRet;
}


CAttackData* CEvadeBuff::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if(M_RAND_HIT(m_fChance))
    {
        return NULL;
    }
    return pAttack;
}

CIceLanceBuff::CIceLanceBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, int iIncreaseTimes, const CAttackValue& pAttackValue)
    :CBuffAbility(pRootId, pName, fDuration, bStackable),m_iIncreaseTimes(iIncreaseTimes)
{
    m_oAttackValue.setAttackValue(pAttackValue);
}

CMultiRefObject* CIceLanceBuff::copy() const
{
    CIceLanceBuff* pRet = new CIceLanceBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_iIncreaseTimes, m_oAttackValue);
    return pRet;
}

void CIceLanceBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnit::MAP_BUFF_ABILITYS& buffs = o->getBuffAbilitys();
    M_MAP_FOREACH(buffs)
    {
        CBuffAbility* buff = M_MAP_EACH;
        if(getId() != buff->getId() && (0 == strcmp(buff->getRootId(), getRootId())))
        {
            M_MAP_DEL_CUR_NEXT(buffs);
            LOG("这下NB了");
            setDuration(0.0f);
            CAttackData* t = new CAttackData;
            t->setAttackValue(m_oAttackValue);
            t->setAttackValue(CAttackValue::kMagical, t->getAttackValue().getValue(CAttackValue::kMagical) * m_iIncreaseTimes);
            CUnit* s = o->getUnit(getSrcUnit());
            assert(s != NULL);
            o->damagedAdv(t, s, CUnit::kNoMasked);
            return;
        }
        M_MAP_NEXT;
    }
    LOG("这下没NB");
    CAttackData* t = new CAttackData;
    t->setAttackValue(m_oAttackValue);
    CUnit* s = o->getUnit(getSrcUnit());
    assert(s != NULL);
    o->damagedAdv(t, s, CUnit::kNoMasked);
}


CRelievePainBuff::CRelievePainBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fReduce)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_fReduce(fReduce)
, m_fDamageReduce(0.0f)
{
    setTriggerFlags(CUnit::kDamagedSurfaceTrigger);
}

CMultiRefObject* CRelievePainBuff::copy() const
{
    CRelievePainBuff* pRet = new CRelievePainBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fReduce);
    return pRet;
}

void CRelievePainBuff::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
    m_fDamageReduce = m_fDamageReduce + pAttack->getAttackValue().getValue(CAttackValue::kMagical) * m_fReduce;
    pAttack->setAttackValue(CAttackValue::kMagical, pAttack->getAttackValue().getValue(CAttackValue::kMagical) *(1 - m_fReduce) );
    LOG("痛苦压制！%d", toInt(m_fDamageReduce));
}

void CRelievePainBuff::onUnitDelAbility()
{
    CAttackData* t = new CAttackData;
    t->setAttackValue(CAttackValue::kMagical, m_fDamageReduce);
    LOG("%s压不住了,受到%d伤害", getOwner()->getName(), toInt(m_fDamageReduce));
}

void CRelievePainBuff::onUnitAddAbility()
{
    LOG("BUFF加上了$$$");
}


CNotKillPas::CNotKillPas(const char* pRootId, const char* pName, float fStartPercent /* = 1.0 */, const CExtraCoeff& roDeltaExAttackValue)
    :CPassiveAbility(pRootId, pName)
{
    m_iTimesCount = 0;
    setTriggerFlags(CUnit::kHpChangeTrigger);
}

CMultiRefObject* CNotKillPas::copy() const
{
    CNotKillPas* pRet = new CNotKillPas(getRootId(), getName(), m_fStartPercent, m_oDeltaExAttackValue);
    return pRet;
}

void CNotKillPas::onUnitChangeHp(float fChanged)
{
    CUnit* o = getOwner();
    float f =  m_fStartPercent - o->getHp()/o->getMaxHp();
    int iTimesChange = 0;
    if (f>0 || m_iTimesCount != 0)
    {
        iTimesChange = m_iTimesCount - int(f/0.10) ;
        m_iTimesCount = int(f/0.10);
        //o->setExArmorValue(getExAttackValue());

        CAttackAct* atk = NULL;
        o->getActiveAbility(o->getAttackAbilityId())->dcast(atk);
        CExtraCoeff oPrev = atk->getExAttackValue(CAttackValue::kPhysical);
        CExtraCoeff oCurrent = CExtraCoeff(oPrev.getMulriple() + iTimesChange * m_oDeltaExAttackValue.getMulriple()
            , oPrev.getAddend() + iTimesChange * m_oDeltaExAttackValue.getAddend());
        atk->setExAttackValue(CAttackValue::kPhysical, oCurrent);
        LOG("Stronger");
    }
    
}

