#ifndef __ABILITYFORCC_H__
#define	__ABILITYFORCC_H__


#include "Ability.h"


class ProgressBar;

class CShowStatusPas : public CPassiveAbility
{
public:
    CShowStatusPas();
    virtual CShowStatusPas* copy() override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;

    virtual void onUnitHpChanged(float fChanged) override;
    virtual void onUnitRevive() override;
    virtual void onUnitDying() override;
    virtual void onUnitInterval() override;
    virtual void onUnitCalcDamageTarget(float fDamage, CUnit* pTarget);

public:
    ProgressBar* m_pHpBar;
    ProgressBar* m_pHpDecBar;
    Label* m_pDps;

protected:
    static const float CONST_INTERVAL;
    static const float CONST_DPS_INTERVAL;
    static const float CONST_DPS_WINDOW;
    float m_fTotalDamage;
    float m_fDpsDuration;
    float m_fDpsUpdate;
    float m_f0DpsDuration;
    float m_fFrontTotalDamage;
    float m_fFrontDpsDuration;

};

#endif	/* __ABILITYFORCC_H__ */
