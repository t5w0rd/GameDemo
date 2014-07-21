#include "CommInc.h"
#include "UnitLibrary.h"
#include "Unit.h"


// CUnitLibrary
CUnitLibrary* CUnitLibrary::m_ul = nullptr;

CUnitLibrary::CUnitLibrary()
{
}

CUnitLibrary* CUnitLibrary::instance()
{
    return m_ul;
}

void CUnitLibrary::instance(CUnitLibrary* ul)
{
    if (ul == m_ul)
    {
        return;
    }

    M_SAFE_RETAIN(ul);
    M_SAFE_RELEASE(m_ul);
    m_ul = ul;
}

void CUnitLibrary::loadDefaultLibrary()
{
}

void CUnitLibrary::addUnit(int id, CUnit* u)
{
}

CUnit* CUnitLibrary::copyUnit(int id) const
{
    return nullptr;
}

void CUnitLibrary::addProjectile(int id, CProjectile* p)
{
}

CProjectile* CUnitLibrary::copyProjectile(int id) const
{
    return nullptr;
}
