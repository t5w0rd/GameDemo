#pragma once

#include "Logic/Ability.h"


class CCProgressBar;

class CStatusShowPas : public CPassiveAbility
{
public:
    CStatusShowPas();
    virtual CMultiRefObject* copy() const;

    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();

    virtual void onUnitChangeHp(float fChanged);

public:
    CCProgressBar* m_pProgressBar;
};
