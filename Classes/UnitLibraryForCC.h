#ifndef __UNITLIBRARYFORCC_H__
#define __UNITLIBRARYFORCC_H__

#include "UnitLibrary.h"


class CUnit;
class CProjectile;

class CUnitLibraryForCC : public CUnitLibrary
{
public:
    CUnitLibraryForCC();

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
    virtual void addUnit(int id, CUnit* u) override;
    virtual CUnit* copyUnit(int id) const override;

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
    virtual void addProjectile(int id, CProjectile* p) override;
    virtual CProjectile* copyProjectile(int id) const override;

    virtual void loadDefaultLibrary() override;

protected:
    bool m_bDefLoaded;
};

#endif  /* __UNITLIBRARYFORCC_H__ */

