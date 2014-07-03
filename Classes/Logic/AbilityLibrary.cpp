#include "CommInc.h"
#include "AbilityLibrary.h"
#include "Ability.h"


// CAbilityLibrary
CAbilityLibrary::CAbilityLibrary()
{
}

int CAbilityLibrary::addTemplateAbility(CAbility* pAbility)
{
    return addTemplateAbility(pAbility->getId(), pAbility);
}

int CAbilityLibrary::addTemplateAbility(int id, CAbility* pAbility)
{
    m_mapTemplateAbilities.addObject(id, pAbility);
    return id;
}

CAbility* CAbilityLibrary::copyAbility(int id) const
{
    CAbility* pAbility = m_mapTemplateAbilities.getObject(id);
    if (pAbility == nullptr)
    {
        return nullptr;
    }

    return pAbility->copy();  // 即时转换失败也不需要释放，因为有CAutoReleasePool
}
