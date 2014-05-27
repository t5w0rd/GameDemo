#ifndef __UNITLIBRARYFORCC_H__
#define __UNITLIBRARYFORCC_H__

#include "MultiRefObject.h"


class CUnit;
class CProjectile;

class CUnitLibraryForCC : public CMultiRefObject
{
public:
    CUnitLibraryForCC();
    M_SINGLETON(CUnitLibraryForCC);

    enum ORG_UNIT
    {
        kMalik = 0x100,
        kMage,
        kBarracks,
        kArcher,
        kFrost,
        kArtillery,
        kElemental,
        kOni,
        kThor,
        kViking,
        kLumberjack,
        kAlric,
        kAlien,
        kBeastMaster,
        kCrabman,
        kGiant,
        kMinotaur,
        kMirage,
        kPirate,
        kPriest,
        kVhelsing,
        kSoldier,
        kTemplar,
        kBarrack,
        kTemplars,
        kArcane,
        kTesla,
        kWizard
    };

    typedef CMultiRefMap<CUnit*> MAP_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnits, Units);
    void addUnit(int id, CUnit* u);

    CUnit* copyUnit(int id) const;

    enum ORG_PROJECTILE
    {
        kMageBolt = 0x100,
        kFrostBolt,
        kArcherArrow,
        kLightning,
        kThorHammer,
        kLumberjackProy,
        kAlienProy,
        kVoodooProy,
        kGiantProy,
        kMirageProy,
        kKungFuProy,
        kPirateProy,
        kPriestBolt,
        kWizardBolt,
        kWizardLightning,
        kWizardProy,
        kArcaneRay,
        kArcaneRay2,
        kTeslaRay,
        kArcherArrow2
    };

    typedef CMultiRefMap<CProjectile*> MAP_PROJECTILES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapProjectiles, Projectiles);
    void addProjectile(int id, CProjectile* p);

    CProjectile* copyProjectile(int id) const;

};

#endif  /* __UNITLIBRARYFORCC_H__ */

