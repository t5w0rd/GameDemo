/* 
 * File:   wAbility.h
 * Author: wwww
 *
 * Created on 2014年2月19日, 下午9:25
 */

#ifndef WABILITY_H
#define	WABILITY_H

#include "CommInc.h"
#include "Ability.h"

//class CChargeOfDarkness : public CActiveAbility
//{
//    public:
//        CChargeOfDarkness();
//};


class CIceLanceBuff : public CBuffAbility
{
public:
    CIceLanceBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, int iIncreaseTimes, const CAttackValue& pAttackValue);
    virtual CMultiRefObject* copy() const;
    M_SYNTHESIZE(int, m_iIncreaseTimes, increaseTimes);
    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAttackValue, AttackValue);
    
    virtual void onUnitAddAbility();


};


class CRelievePainBuff : public CBuffAbility
{
public:
    CRelievePainBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fReduce);
    virtual CMultiRefObject* copy() const;
    M_SYNTHESIZE(float, m_fReduce, fReduce);
    M_SYNTHESIZE(float, m_fDamageReduce, fDamageReduce);
    
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDelAbility();
    virtual void onUnitAddAbility();


};


class CNotKillPas : public CPassiveAbility
{
public:
    CNotKillPas(const char* pRootId, const char* pName, float fStartPercent = 1.0, const CExtraCoeff& roExAttackValue = CExtraCoeff(0, 0));
    virtual CMultiRefObject* copy() const;
    M_SYNTHESIZE(int, m_iTimesCount, iTimesCount);
    M_SYNTHESIZE(float, m_fStartPercent, fStartPercent);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oDeltaExAttackValue, oDeltaExAttackValue);

    virtual void onUnitChangeHp(float fChanged);
};

/*
class CRuleNumberOneAi : public CUnitEventAdapter
{
public:

};
*/
#endif	/* WABILITY_H */

