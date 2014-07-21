#ifndef __ABILITYLIBRARY_H__
#define __ABILITYLIBRARY_H__

#include "MultiRefObject.h"


class CAbilityLibrary : public CMultiRefObject
{
public:
    CAbilityLibrary();
    M_SINGLETON(CAbilityLibrary);

    typedef CMultiRefMap<CAbility*> MAP_ABILITIES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ABILITIES, m_mapTemplateAbilities, TemplateAbilities);
    int addTemplateAbility(CAbility* pAbility);
    int addTemplateAbility(int id, CAbility* pAbility);
    void loadTemplateAbilities();
    virtual CAbility* copyAbility(int id) const;
};


#endif // __ABILITYLIBRARY_H__
