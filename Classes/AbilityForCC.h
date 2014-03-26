#ifndef __ABILITYFORCC_H__
#define	__ABILITYFORCC_H__


#include "Ability.h"


class CCProgressBar;

class CStatusShowPas : public CPassiveAbility
{
public:
    CStatusShowPas();
    virtual CMultiRefObject* copy() const;

    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();

    virtual void onUnitChangeHp(float fChanged);
    virtual void onUnitRevive();
    virtual void onUnitDying();

public:
    CCProgressBar* m_pProgressBar;
};

#endif	/* __ABILITYFORCC_H__ */
