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

// for cocos2d
#include "CommHeader.h"
#include "DrawForCC.h"


CIceLanceBuff::CIceLanceBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, int iIncreaseTimes, const CAttackValue& rAttackValue)
    :CBuffAbility(pRootId, pName, fDuration, bStackable),m_iIncreaseTimes(iIncreaseTimes)
{
    m_oAttackValue = rAttackValue;
}

CMultiRefObject* CIceLanceBuff::copy() const
{
    CIceLanceBuff* pRet = new CIceLanceBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_iIncreaseTimes, m_oAttackValue);
    return pRet;
}

void CIceLanceBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnit::MAP_BUFF_ABILITIES& buffs = o->getBuffAbilities();
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
            t->setAttackValue(CAttackValue::kMagical, t->getAttackValue().getValue() * m_iIncreaseTimes);
            CUnit* s = o->getUnit(getSrcUnit());
            assert(s != nullptr);
            o->damaged(t, s, CUnit::kNoMasked);
            return;
        }
        M_MAP_NEXT;
    }
    LOG("这下没NB");
    CAttackData* t = new CAttackData;
    t->setAttackValue(m_oAttackValue);
    CUnit* s = o->getUnit(getSrcUnit());
    assert(s != nullptr);
    o->damaged(t, s, CUnit::kNoMasked);
}


CRelievePainBuff::CRelievePainBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fReduce)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_fReduce(fReduce)
, m_fDamageReduce(0.0f)
{
    setTriggerFlags(CUnit::kOnDamagedSurfaceTrigger);
}

CMultiRefObject* CRelievePainBuff::copy() const
{
    CRelievePainBuff* pRet = new CRelievePainBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fReduce);
    return pRet;
}

void CRelievePainBuff::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
    m_fDamageReduce = m_fDamageReduce + pAttack->getAttackValue().getValue() * m_fReduce;
    pAttack->setAttackValue(CAttackValue::kPhysical, pAttack->getAttackValue().getValue() *(1 - m_fReduce));
    LOG("痛苦压制！%d", toInt(m_fDamageReduce));
}

void CRelievePainBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    float f = o->getHp();
    float f2 = m_fDamageReduce;
    if (f - f2 <= 0)
    {
        f2 = f - 1;        
    }
    o->setHp(f - f2);

    // for cocos2d
    CUnitDrawForCC* ccd = nullptr;
    o->getDraw()->dcast(ccd);

    if (ccd != nullptr)
    {
        char sz[64];
        sprintf(sz, "CallBack -%d", toInt(f2));
        ccd->addBattleTip(sz, "Comic Book", 18, Color3B(56, 24, 128));
    }

    LOG("%s压不住了,受到%d伤害(实际%d伤害)", o->getName(), toInt(m_fDamageReduce), toInt(f2));
}

void CRelievePainBuff::onUnitAddAbility()
{
    LOG("BUFF加上了$$$");
}


CNotKillPas::CNotKillPas(const char* pRootId, const char* pName, float fStartPercent /* = 1.0 */, const CExtraCoeff& roDeltaExAttackValue)
    :CPassiveAbility(pRootId, pName), m_fStartPercent(fStartPercent), m_oDeltaExAttackValue(roDeltaExAttackValue)
{
    m_iTimesCount = 0;
    setTriggerFlags(CUnit::kOnChangeHpTrigger);
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
    LOG("StartIf");
    if (f>0 || m_iTimesCount != 0)
    {
        iTimesChange = int(f/0.10) - m_iTimesCount;
        m_iTimesCount = int(f/0.10);
        //o->setExArmorValue(getExAttackValue());

        CAttackAct* atk = nullptr;
        o->getActiveAbility(o->getAttackAbilityId())->dcast(atk);
        CExtraCoeff oPrev = atk->getExAttackValue();
        CExtraCoeff oCurrent = CExtraCoeff(oPrev.getMulriple() + iTimesChange * m_oDeltaExAttackValue.getMulriple()
        , oPrev.getAddend() + iTimesChange * m_oDeltaExAttackValue.getAddend());
        atk->setExAttackValue(oCurrent);
        LOG("Stronger");
    }
    
}

