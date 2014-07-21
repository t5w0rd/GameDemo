#ifndef __UNITLIBRARY_H__
#define __UNITLIBRARY_H__

#include "MultiRefObject.h"


class CUnitLibrary : public CMultiRefObject
{
public:
    CUnitLibrary();
    static CUnitLibrary* instance();
    static void instance(CUnitLibrary* ul);

    virtual void loadDefaultLibrary();
    virtual void addUnit(int id, CUnit* u);
    virtual CUnit* copyUnit(int id) const;
    virtual void addProjectile(int id, CProjectile* p);
    virtual CProjectile* copyProjectile(int id) const;

protected:
    static CUnitLibrary* m_ul;
    bool m_bDefLoaded;

};


#endif // __UNITLIBRARY_H__
