#ifndef __UNITLIBRARYFORCC_H__
#define __UNITLIBRARYFORCC_H__


class CUnit;
class CProjectile;
class CUnitLibraryForCC
{
public:
    CUnitLibraryForCC(void);
    bool init();

    enum ORG_UNIT
    {
        kReinforce = 0x100,
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
        kAlric
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
        kLumberjackProy
    };

    typedef CMultiRefMap<CProjectile*> MAP_PROJECTILES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapProjectiles, Projectiles);
    void addProjectile(int id, CProjectile* p);

    CProjectile* copyProjectile(int id) const;

};

#endif  /* __UNITLIBRARYFORCC_H__ */

