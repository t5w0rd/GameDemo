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

    virtual void onUnitChangeHp(float fChanged);
    virtual void onUnitRevive();
    virtual void onUnitDying();

public:
    ProgressBar* m_pProgressBar;
};

#endif	/* __ABILITYFORCC_H__ */
