#include "CommHeader.h"

#include "UnitLibraryForCC.h"
#include "Unit.h"
#include "Ability.h"
#include "GameControl.h"
#include "DrawForCC.h"


// CUnitLibraryForCC
CUnitLibraryForCC::CUnitLibraryForCC()
: m_bDefLoaded(false)
{
    setDbgClassName("CUnitLibraryForCC");
}

void CUnitLibraryForCC::addUnit(int id, CUnit* u)
{
    m_mapUnits.addObject(id, u);
}

CUnit* CUnitLibraryForCC::copyUnit(int id) const
{
    auto it = m_mapUnits.find(id);
    if (it == m_mapUnits.end())
    {
        return nullptr;
    }

    return DCAST(it->second->copy(), CUnit*);
}

void CUnitLibraryForCC::addProjectile(int id, CProjectile* p)
{
    m_mapProjectiles.addObject(id, p);
}

CProjectile* CUnitLibraryForCC::copyProjectile(int id) const
{
    auto it = m_mapProjectiles.find(id);
    if (it == m_mapProjectiles.end())
    {
        return nullptr;
    }

    return DCAST(it->second->copy(), CProjectile*);
}

void CUnitLibraryForCC::loadDefaultLibrary()
{
    if (m_bDefLoaded)
    {
        return;
    }

    CSpriteInfo* si = nullptr;
    CProjectileForCC* p = nullptr;
    CUnitDrawForCC* d = nullptr;
    CUnit* u = nullptr;
    CAttackAct* atk = nullptr;
    M_DEF_GC(gc);

    // MageBolt
    gc->loadAnimation("Sprites/MageBolt/move", "Sprites/MageBolt/move", 0.1f);
    gc->loadAnimation("Sprites/MageBolt/die", "Sprites/MageBolt/die", 0.1f);

    si = new CSpriteInfo("MageBolt");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(50.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->addFireSound("sounds/Effects/MageShot.mp3");
    addProjectile(kMageBolt, p);

    // FrostBolt
    gc->loadAnimation("Sprites/FrostBolt/move", "Sprites/FrostBolt/move", 0.1f);
    gc->loadAnimation("Sprites/FrostBolt/die", "Sprites/FrostBolt/die", 0.1f);

    si = new CSpriteInfo("FrostBolt");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(10.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->addFireSound("sounds/Effects/MageShot.mp3");
    addProjectile(kFrostBolt, p);

    // ArcherArrow
    si = new CSpriteInfo("ArcherArrow");
    si->prepareFrame(CProjectile::kFrmDefault, "default");

    p = new CProjectileForCC(si);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(20.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->addFireSound("sounds/Effects/ArrowRelease00.mp3");
    p->addFireSound("sounds/Effects/ArrowRelease01.mp3");
    addProjectile(kArcherArrow, p);

    // ArcherArrow2
    si = new CSpriteInfo("ArcherArrow2");
    si->prepareFrame(CProjectile::kFrmDefault, "default", "ArcherArrow");

    p = new CProjectileForCC(si);
    p->setMoveSpeed(600.0f);
    p->setMaxHeightDelta(20.0f);
    p->setPenaltyFlags(CProjectile::kOnContact);
    p->setFireType(CProjectile::kFireStraight);
    //p->addFireSound("sounds/Effects/ArrowRelease00.mp3");
    //p->addFireSound("sounds/Effects/ArrowRelease01.mp3");
    addProjectile(kArcherArrow2, p);

    // Lightning
    gc->loadAnimation("Sprites/Lightning/die", "Sprites/Lightning/die", 0.05f);

    si = new CSpriteInfo("Lightning");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kLightning, p);

    // ThorHammer
    si = new CSpriteInfo("ThorHammer");
    si->prepareFrame(CProjectile::kFrmDefault, "default");

    p = new CProjectileForCC(si);
    p->setMoveSpeed(800.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->addFireSound("sounds/Effects/ThrowHammer.mp3");
    addProjectile(kThorHammer, p);

    // LumberjackProy
    gc->loadAnimation("Sprites/LumberjackProy/move", "Sprites/LumberjackProy/move", 0.1f);
    gc->loadAnimation("Sprites/LumberjackProy/die", "Sprites/LumberjackProy/die", 0.1f);

    si = new CSpriteInfo("LumberjackProy");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(500.0f);
    p->setMaxHeightDelta(00.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kLumberjackProy, p);

    // ArcaneRay
    gc->loadAnimation("Sprites/ArcaneRay/die", "Sprites/ArcaneRay/die", 0.1f);

    si = new CSpriteInfo("ArcaneRay");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    p->addFireSound("sounds/Effects/ArcaneRay.mp3");
    addProjectile(kArcaneRay, p);

    // ArcaneRay2
    gc->loadAnimation("Sprites/ArcaneRay2/die", "Sprites/ArcaneRay2/die", 0.1f);

    si = new CSpriteInfo("ArcaneRay2");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kArcaneRay2, p);

    // TeslaRay
    gc->loadAnimation("Sprites/TeslaRay/die", "Sprites/TeslaRay/die", 0.1f);

    si = new CSpriteInfo("TeslaRay");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    p->addFireSound("sounds/Effects/TeslaRay00.mp3");
    p->addFireSound("sounds/Effects/TeslaRay01.mp3");
    addProjectile(kTeslaRay, p);

    // AlienProy
    gc->loadAnimation("Sprites/AlienProy/move", "Sprites/AlienProy/move", 0.1f);
    gc->loadAnimation("Sprites/AlienProy/die", "Sprites/AlienProy/die", 0.1f);

    si = new CSpriteInfo("AlienProy");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(500.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnContact);
    p->setFireType(CProjectile::kFireStraight);
    p->addFireSound("sounds/Effects/MageShot.mp3");
    addProjectile(kAlienProy, p);

    // WizardBolt
    gc->loadAnimation("Sprites/WizardBolt/move", "Sprites/WizardBolt/move", 0.1f);
    gc->loadAnimation("Sprites/WizardBolt/die", "Sprites/WizardBolt/die", 0.05f);

    si = new CSpriteInfo("WizardBolt");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(50.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->addFireSound("sounds/Effects/MageShot.mp3");
    addProjectile(kWizardBolt, p);

    // VoodooProy
    gc->loadAnimation("Sprites/VoodooProy/move", "Sprites/VoodooProy/move", 0.1f);
    gc->loadAnimation("Sprites/VoodooProy/die", "Sprites/VoodooProy/die", 0.1f);

    si = new CSpriteInfo("VoodooProy");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(600.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnContact);
    p->setFireType(CProjectile::kFireStraight);
    addProjectile(kVoodooProy, p);

    // MirageProy
    gc->loadAnimation("Sprites/MirageProy/die", "Sprites/MirageProy/die", 0.1f);

    si = new CSpriteInfo("MirageProy");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(600.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnContact);
    p->setFireType(CProjectile::kFireStraight);
    p->addFireSound("sounds/Effects/ArrowRelease00.mp3");
    p->addFireSound("sounds/Effects/ArrowRelease01.mp3");
    addProjectile(kMirageProy, p);

    // PirateProy
    gc->loadAnimation("Sprites/PirateProy/move", "Sprites/PirateProy/move", 0.1f);
    gc->loadAnimation("Sprites/PirateProy/die", "Sprites/PirateProy/die", 0.1f);

    si = new CSpriteInfo("PirateProy");
    si->prepareFrame(CProjectile::kFrmDefault, "default");
    si->prepareAnimation(CProjectile::kAniMove, "move", -1);
    si->prepareAnimation(CProjectile::kAniDie, "die", 0);

    p = new CProjectileForCC(si);
    p->setMoveSpeed(600.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnContact);
    p->setFireType(CProjectile::kFireStraight);
    p->addFireSound("sounds/Effects/Sound_FireballUnleash.mp3");
    addProjectile(kPirateProy, p);

    // Malik
    gc->loadAnimation("Sprites/Malik/move", "Sprites/Malik/move", 0.08f);
    gc->loadAnimation("Sprites/Malik/die", "Sprites/Malik/die", 0.1f);
    gc->loadAnimation("Sprites/Malik/act1", "Sprites/Malik/act1", 0.1f);
    gc->loadAnimation("Sprites/Malik/act2", "Sprites/Malik/act2", 0.1f);
    gc->loadAnimation("Sprites/Malik/act3", "Sprites/Malik/act3", 0.08f);
    gc->loadAnimation("Sprites/Malik/act4", "Sprites/Malik/act4", 0.1f);
    gc->loadAnimation("Sprites/Malik/act5", "Sprites/Malik/act5", 0.1f);

    si = new CSpriteInfo("Malik");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 3);
    si->prepareAnimation(kAniAct2, "act2", 3);
    si->prepareAnimation(kAniAct3, "act3", 8);
    si->prepareAnimation(kAniAct4, "act4", 7);
    si->prepareAnimation(kAniAct5, "act5", 8);

    d = new CUnitDrawForCC(si);
    d->setGeometry(31.0f, 30.0f, Point(96.0f / 192, 20.0f / 200), CPoint(40.0f, 26.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kMalik, u);

    // Mage
    gc->loadAnimation("Sprites/Mage/move", "Sprites/Mage/move", 0.08f);
    gc->loadAnimation("Sprites/Mage/die", "Sprites/Mage/die", 0.1f);
    gc->loadAnimation("Sprites/Mage/act1", "Sprites/Mage/act1", 0.1f);
    gc->loadAnimation("Sprites/Mage/act2", "Sprites/Mage/act2", 0.1f);
    gc->loadAnimation("Sprites/Mage/act3", "Sprites/Mage/act3", 0.1f);
    gc->loadAnimation("Sprites/Mage/act4", "Sprites/Mage/act4", 0.1f);
    gc->loadAnimation("Sprites/Mage/act5", "Sprites/Mage/act5", 0.1f);

    si = new CSpriteInfo("Mage");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 5);
    si->prepareAnimation(kAniAct2, "act2", 6);
    si->prepareAnimation(kAniAct3, "act3", 10);
    si->prepareAnimation(kAniAct4, "act4", 4);
    si->prepareAnimation(kAniAct5, "act5", 9);

    d = new CUnitDrawForCC(si);
    d->setGeometry(22.0f, 18.0f, Point(61.0f / 120, 21.0f / 152), CPoint(15.0f, 26.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kCrystal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kMagical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(200.0f);
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    atk->setTemplateProjectile(kMageBolt);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kMage, u);

    // Barracks
    gc->loadAnimation("Sprites/Barracks/move", "Sprites/Barracks/move", 0.08f);
    gc->loadAnimation("Sprites/Barracks/die", "Sprites/Barracks/die", 0.1f);
    gc->loadAnimation("Sprites/Barracks/act1", "Sprites/Barracks/act1", 0.1f);
    gc->loadAnimation("Sprites/Barracks/act2", "Sprites/Barracks/act2", 0.1f);
    gc->loadAnimation("Sprites/Barracks/act3", "Sprites/Barracks/act3", 0.07f);
    gc->loadAnimation("Sprites/Barracks/act4", "Sprites/Barracks/act4", 0.07f);
    gc->loadAnimation("Sprites/Barracks/act5", "Sprites/Barracks/act5", 0.1f);

    si = new CSpriteInfo("Barracks");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0, "sounds/Sprites/Barracks/die/00.mp3");
    si->prepareAnimation(kAniAct1, "act1", 3, "sounds/Effects/MetalHeavySliceFlesh00.mp3");
    si->addAnimationSound(kAniAct1, "sounds/Effects/MetalHeavySliceFlesh01.mp3");
    si->addAnimationSound(kAniAct1, "sounds/Effects/MetalHeavySliceFlesh02.mp3");
    si->prepareAnimation(kAniAct2, "act2", 3, "sounds/Effects/MetalHeavySliceFlesh00.mp3");
    si->addAnimationSound(kAniAct2, "sounds/Effects/MetalHeavySliceFlesh01.mp3");
    si->addAnimationSound(kAniAct2, "sounds/Effects/MetalHeavySliceFlesh02.mp3");
    si->prepareAnimation(kAniAct3, "act3", 5);
    si->prepareAnimation(kAniAct4, "act4", 18);
    si->prepareAnimation(kAniAct5, "act5", 8);

    d = new CUnitDrawForCC(si);
    d->setGeometry(28.0f, 25.0f, Point(64.0f / 128, 22.0f / 152), CPoint(37.0f, 23.0f));
    d->addCtrlSound("sounds/Sprites/Barracks/move/00.mp3", 1.697f);
    d->addCtrlSound("sounds/Sprites/Barracks/move/01.mp3", 1.201f);
    d->addCtrlSound("sounds/Sprites/Barracks/move/02.mp3", 1.959f);
    d->addCtrlSound("sounds/Sprites/Barracks/move/03.mp3", 2.115f);
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kBarracks, u);

    // Archer
    gc->loadAnimation("Sprites/Archer/move", "Sprites/Archer/move", 0.08f);
    gc->loadAnimation("Sprites/Archer/die", "Sprites/Archer/die", 0.1f);
    gc->loadAnimation("Sprites/Archer/act1", "Sprites/Archer/act1", 0.1f);
    gc->loadAnimation("Sprites/Archer/act2", "Sprites/Archer/act2", 0.1f);
    gc->loadAnimation("Sprites/Archer/act3", "Sprites/Archer/act3", 0.07f);
    gc->loadAnimation("Sprites/Archer/act4", "Sprites/Archer/act4", 0.07f);
    gc->loadAnimation("Sprites/Archer/act5", "Sprites/Archer/act5", 0.1f);

    si = new CSpriteInfo("Archer");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0, "sounds/Sprites/Archer/die/00.mp3");
    si->prepareAnimation(kAniAct1, "act1", 4);
    si->prepareAnimation(kAniAct2, "act2", 4);
    si->prepareAnimation(kAniAct3, "act3", 13);
    si->prepareAnimation(kAniAct4, "act4", 6);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(20.0f, 20.0f, Point(60.0f / 120, 24.0f / 152), CPoint(28.0f, 35.0f));
    d->addCtrlSound("sounds/Sprites/Archer/move/00.mp3", 1.044f);
    d->addCtrlSound("sounds/Sprites/Archer/move/01.mp3", 2.533f);
    d->addCtrlSound("sounds/Sprites/Archer/move/02.mp3", 2.246f);
    d->addCtrlSound("sounds/Sprites/Archer/move/03.mp3", 1.436f);
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 60.0),
        0.5);
    atk->setCastMinRange(-25.0f);
    atk->setCastRange(200.0f);
    atk->addCastAnimation(kAniAct1);
    atk->setTemplateProjectile(kArcherArrow);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kArcher, u);

    // Frost
    gc->loadAnimation("Sprites/Frost/move", "Sprites/Frost/move", 0.08f);
    gc->loadAnimation("Sprites/Frost/die", "Sprites/Frost/die", 0.1f);
    gc->loadAnimation("Sprites/Frost/act1", "Sprites/Frost/act1", 0.1f);
    gc->loadAnimation("Sprites/Frost/act2", "Sprites/Frost/act2", 0.1f);
    gc->loadAnimation("Sprites/Frost/act3", "Sprites/Frost/act3", 0.1f);
    gc->loadAnimation("Sprites/Frost/act4", "Sprites/Frost/act4", 0.1f);
    gc->loadAnimation("Sprites/Frost/act5", "Sprites/Frost/act5", 0.1f);

    si = new CSpriteInfo("Frost");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 4);
    si->prepareAnimation(kAniAct2, "act2", 5);
    si->prepareAnimation(kAniAct3, "act3", 6);
    si->prepareAnimation(kAniAct4, "act4", 7);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(27.0f, 27.0f, Point(83.0f / 164, 36.0f / 208), CPoint(42.0f, 31.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kCrystal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kMagical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(200.0f);
    atk->addCastAnimation(kAniAct1);
    atk->setTemplateProjectile(kFrostBolt);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kFrost, u);

    // Artillery
    gc->loadAnimation("Sprites/Artillery/move", "Sprites/Artillery/move", 0.08f);
    gc->loadAnimation("Sprites/Artillery/die", "Sprites/Artillery/die", 0.1f);
    gc->loadAnimation("Sprites/Artillery/act1", "Sprites/Artillery/act1", 0.1f);
    gc->loadAnimation("Sprites/Artillery/act2", "Sprites/Artillery/act2", 0.1f);
    gc->loadAnimation("Sprites/Artillery/act3", "Sprites/Artillery/act3", 0.1f);
    gc->loadAnimation("Sprites/Artillery/act4", "Sprites/Artillery/act4", 0.1f);
    gc->loadAnimation("Sprites/Artillery/act5", "Sprites/Artillery/act5", 0.1f);

    si = new CSpriteInfo("Artillery");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 4, "sounds/Effects/ShootGun.mp3");
    si->prepareAnimation(kAniAct2, "act2", 3);
    si->prepareAnimation(kAniAct3, "act3", 2);
    si->prepareAnimation(kAniAct4, "act4", 8);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(26.0f, 20.0f, Point(91.0f / 184, 39.0f / 164), CPoint(36.0f, 46.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(200.0f);
    atk->addCastAnimation(kAniAct1);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kArtillery, u);

    // Elemental
    gc->loadAnimation("Sprites/Elemental/move", "Sprites/Elemental/move", 0.08f);
    gc->loadAnimation("Sprites/Elemental/die", "Sprites/Elemental/die", 0.1f);
    gc->loadAnimation("Sprites/Elemental/act1", "Sprites/Elemental/act1", 0.1f);
    gc->loadAnimation("Sprites/Elemental/act2", "Sprites/Elemental/act2", 0.1f);
    gc->loadAnimation("Sprites/Elemental/act3", "Sprites/Elemental/act3", 0.1f);
    gc->loadAnimation("Sprites/Elemental/act5", "Sprites/Elemental/act5", 0.1f);

    si = new CSpriteInfo("Elemental");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 4);
    si->prepareAnimation(kAniAct2, "act2", 4);
    si->prepareAnimation(kAniAct3, "act3", 6);
    si->prepareAnimation(kAniAct5, "act5", 8);

    d = new CUnitDrawForCC(si);
    d->setGeometry(31.0f, 28.0f, Point(59.0f / 120, 16.0f / 144), CPoint(40.0f, 30.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kElemental, u);

    // Oni
    gc->loadAnimation("Sprites/Oni/move", "Sprites/Oni/move", 0.08f);
    gc->loadAnimation("Sprites/Oni/die", "Sprites/Oni/die", 0.1f);
    gc->loadAnimation("Sprites/Oni/act1", "Sprites/Oni/act1", 0.08f);
    gc->loadAnimation("Sprites/Oni/act2", "Sprites/Oni/act2", 0.06f);
    gc->loadAnimation("Sprites/Oni/act3", "Sprites/Oni/act3", 0.06f);
    gc->loadAnimation("Sprites/Oni/act5", "Sprites/Oni/act5", 0.1f);

    si = new CSpriteInfo("Oni");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 9);
    si->prepareAnimation(kAniAct2, "act2", 9);
    si->prepareAnimation(kAniAct3, "act3", 19);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(38.0f, 31.0f, Point(130.0f / 256, 33.0f / 224), CPoint(105.0f, 10.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct3);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kOni, u);

    // Thor
    gc->loadAnimation("Sprites/Thor/move", "Sprites/Thor/move", 0.08f);
    gc->loadAnimation("Sprites/Thor/die", "Sprites/Thor/die", 0.1f);
    gc->loadAnimation("Sprites/Thor/act1", "Sprites/Thor/act1", 0.1f);
    gc->loadAnimation("Sprites/Thor/act2", "Sprites/Thor/act2", 0.05f);
    gc->loadAnimation("Sprites/Thor/act3", "Sprites/Thor/act3", 0.1f);
    gc->loadAnimation("Sprites/Thor/act5", "Sprites/Thor/act5", 0.1f);

    si = new CSpriteInfo("Thor");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0, "sounds/Sprites/Thor/die/00.mp3");
    si->prepareAnimation(kAniAct1, "act1", 6, "sounds/Effects/MetalHeavyBashFlesh00.mp3");
    si->addAnimationSound(kAniAct1, "sounds/Effects/MetalHeavyBashFlesh01.mp3");
    si->addAnimationSound(kAniAct1, "sounds/Effects/MetalHeavyBashFlesh02.mp3");
    si->prepareAnimation(kAniAct2, "act2", 5);
    si->prepareAnimation(kAniAct3, "act3", 4);
    si->prepareAnimation(kAniAct5, "act5", 3, "sounds/Effects/LevelUp.mp3");

    d = new CUnitDrawForCC(si);
    d->setGeometry(40.0f, 29.0f, Point(124.0f / 240, 52.0f / 192), CPoint(59.0f, 30.0f));
    d->addCtrlSound("sounds/Sprites/Thor/move/00.mp3", 1.488f);
    d->addCtrlSound("sounds/Sprites/Thor/move/01.mp3", 2.612f);
    d->addCtrlSound("sounds/Sprites/Thor/move/02.mp3", 2.429f);
    d->addCtrlSound("sounds/Sprites/Thor/move/03.mp3", 1.880f);
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 6.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 55.5f),
        0.1261f);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    //atk->setTemplateProjectile(kLightning);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kThor, u);

    // Viking
    gc->loadAnimation("Sprites/Viking/move", "Sprites/Viking/move", 0.08f);
    gc->loadAnimation("Sprites/Viking/die", "Sprites/Viking/die", 0.1f);
    gc->loadAnimation("Sprites/Viking/act1", "Sprites/Viking/act1", 0.1f);
    gc->loadAnimation("Sprites/Viking/act2", "Sprites/Viking/act2", 0.1f);
    gc->loadAnimation("Sprites/Viking/act3", "Sprites/Viking/act3", 0.1f);
    gc->loadAnimation("Sprites/Viking/act5", "Sprites/Viking/act5", 0.1f);

    si = new CSpriteInfo("Viking");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 4);
    si->prepareAnimation(kAniAct2, "act2", 7);
    si->prepareAnimation(kAniAct3, "act3", 8);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(38.0f, 32.0f, Point(141.0f / 284, 44.0f / 232), CPoint(83.0f, 22.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kViking, u);

    // Lumberjack
    gc->loadAnimation("Sprites/Lumberjack/move", "Sprites/Lumberjack/move", 0.08f);
    gc->loadAnimation("Sprites/Lumberjack/die", "Sprites/Lumberjack/die", 0.1f);
    gc->loadAnimation("Sprites/Lumberjack/act1", "Sprites/Lumberjack/act1", 0.1f);
    gc->loadAnimation("Sprites/Lumberjack/act2", "Sprites/Lumberjack/act2", 0.1f);
    gc->loadAnimation("Sprites/Lumberjack/act3", "Sprites/Lumberjack/act3", 0.1f);
    gc->loadAnimation("Sprites/Lumberjack/act5", "Sprites/Lumberjack/act5", 0.1f);

    si = new CSpriteInfo("Lumberjack");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 6);
    si->prepareAnimation(kAniAct2, "act2", 6);
    si->prepareAnimation(kAniAct3, "act3", 6);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(51.0f, 45.0f, Point(91.0f / 180, 31.0f / 220), CPoint(62.0f, 41.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    atk->addCastAnimation(kAniAct3);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kLumberjack, u);

    // Alric
    gc->loadAnimation("Sprites/Alric/move", "Sprites/Alric/move", 0.08f);
    gc->loadAnimation("Sprites/Alric/die", "Sprites/Alric/die", 0.1f);
    gc->loadAnimation("Sprites/Alric/act1", "Sprites/Alric/act1", 0.08f);
    gc->loadAnimation("Sprites/Alric/act2", "Sprites/Alric/act2", 0.08f);
    gc->loadAnimation("Sprites/Alric/act3", "Sprites/Alric/act3", 0.05f);
    gc->loadAnimation("Sprites/Alric/act4", "Sprites/Alric/act4", 0.05f);
    gc->loadAnimation("Sprites/Alric/act5", "Sprites/Alric/act5", 0.1f);

    si = new CSpriteInfo("Alric");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 4);
    si->prepareAnimation(kAniAct2, "act2", 3);
    si->prepareAnimation(kAniAct3, "act3", 8, "sounds/Effects/hero_alric_flurry.mp3");
    si->prepareAnimation(kAniAct4, "act4", 10);
    si->prepareAnimation(kAniAct5, "act5", 3);

    d = new CUnitDrawForCC(si);
    d->setGeometry(26.0f, 24.0f, Point(83.0f / 164, 17.0f / 180), CPoint(62.0f, 26.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kAlric, u);

    // Soldier
    gc->loadAnimation("Sprites/Soldier/move", "Sprites/Soldier/move", 0.08f);
    gc->loadAnimation("Sprites/Soldier/die", "Sprites/Soldier/die", 0.1f);
    gc->loadAnimation("Sprites/Soldier/act1", "Sprites/Soldier/act1", 0.08f);
    gc->loadAnimation("Sprites/Soldier/act2", "Sprites/Soldier/act2", 0.1f);
    gc->loadAnimation("Sprites/Soldier/act3", "Sprites/Soldier/act3", 0.1f);
    gc->loadAnimation("Sprites/Soldier/act4", "Sprites/Soldier/act4", 0.1f);

    si = new CSpriteInfo("Soldier");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->addAnimationSound(kAniDie, "sounds/Effects/HumenDie00.mp3");
    si->addAnimationSound(kAniDie, "sounds/Effects/HumenDie01.mp3");
    si->addAnimationSound(kAniDie, "sounds/Effects/HumenDie02.mp3");
    si->addAnimationSound(kAniDie, "sounds/Effects/HumenDie03.mp3");
    si->prepareAnimation(kAniAct1, "act1", 2);
    si->prepareAnimation(kAniAct2, "act2", 3);
    si->prepareAnimation(kAniAct3, "act3", 11);
    si->prepareAnimation(kAniAct4, "act4", 9);

    d = new CUnitDrawForCC(si);
    d->setGeometry(25.0f, 21.0f, Point(94.0f / 188, 17.0f / 104), CPoint(40.0f, 21.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(550.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 2.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kPhysical, 21.0),
        0.19f);
    atk->setCastMinRange(-9.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    atk->addCastAnimation(kAniAct2);
    u->addActiveAbility(atk);
    u->setRewardGold(10);
    u->setRewardExp(7);
    d->setBaseMoveSpeed(50.0f);
    addUnit(kSoldier, u);

    // Templar
    gc->loadAnimation("Sprites/Templar/move", "Sprites/Templar/move", 0.08f);
    gc->loadAnimation("Sprites/Templar/die", "Sprites/Templar/die", 0.1f);
    gc->loadAnimation("Sprites/Templar/act1", "Sprites/Templar/act1", 0.07f);
    gc->loadAnimation("Sprites/Templar/act2", "Sprites/Templar/act2", 0.1f);
    gc->loadAnimation("Sprites/Templar/act3", "Sprites/Templar/act3", 0.1f);

    si = new CSpriteInfo("Templar");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 3);
    si->prepareAnimation(kAniAct2, "act2", 9);
    si->prepareAnimation(kAniAct3, "act3", 2);

    d = new CUnitDrawForCC(si);
    d->setGeometry(19.0f, 20.0f, Point(74.0f / 144, 16.0f / 84), CPoint(32.0f, 20.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(550.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 2.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kPhysical, 21.0),
        0.19f);
    atk->setCastMinRange(-7.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(kAniAct1);
    u->addActiveAbility(atk);
    u->setRewardGold(10);
    u->setRewardExp(7);
    d->setBaseMoveSpeed(50.0f);
    addUnit(kTemplar, u);

    // Arcane
    gc->loadAnimation("Sprites/Arcane/die", "Sprites/Arcane/die", 0.1f);
    gc->loadAnimation("Sprites/Arcane/act1", "Sprites/Arcane/act1", 0.08f);

    si = new CSpriteInfo("Arcane");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 9);

    d = new CUnitDrawForCC(si);
    d->setGeometry(63.0f, 60.0f, Point(120.0f / 240, 44.0f / 184), CPoint(0.0f, 118.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1300.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 18.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kMagical, 110.0),
        0.1818f);
    atk->setCastMinRange(-60.0f);
    atk->setCastRange(300.0f);
    atk->addCastAnimation(kAniAct1);
    atk->setTemplateProjectile(kArcaneRay);
    u->addActiveAbility(atk);
    d->setFixed();
    d->setHostilityRange(300.0f);
    addUnit(kArcane, u);

    // Tesla
    gc->loadAnimation("Sprites/Tesla/die", "Sprites/Tesla/die", 0.1f);
    gc->loadAnimation("Sprites/Tesla/act1", "Sprites/Tesla/act1", 0.031f);

    si = new CSpriteInfo("Tesla");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 17);

    d = new CUnitDrawForCC(si);
    d->setGeometry(63.0f, 60.0f, Point(120.0f / 240, 44.0f / 184), CPoint(0.0f, 118.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(1300.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kHeavy, 18.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kMagical, 110.0),
        0.1818f);
    atk->setCastMinRange(-60.0f);
    atk->setCastRange(300.0f);
    atk->addCastAnimation(kAniAct1);
    atk->setTemplateProjectile(kTeslaRay);
    u->addActiveAbility(atk);
    d->setFixed();
    d->setHostilityRange(300.0f);
    addUnit(kTesla, u);

    // Wizard
    gc->loadAnimation("Sprites/Wizard/move", "Sprites/Wizard/move", 0.08f);
    gc->loadAnimation("Sprites/Wizard/die", "Sprites/Wizard/die", 0.1f);
    gc->loadAnimation("Sprites/Wizard/act1", "Sprites/Wizard/act1", 0.1f);
    gc->loadAnimation("Sprites/Wizard/act2", "Sprites/Wizard/act2", 0.1f);
    gc->loadAnimation("Sprites/Wizard/act3", "Sprites/Wizard/act3", 0.1f);
    //gc->loadAnimation("Sprites/Wizard/act4", "Sprites/Wizard/act4", 0.1f);
    gc->loadAnimation("Sprites/Wizard/act5", "Sprites/Wizard/act5", 0.1f);

    si = new CSpriteInfo("Wizard");
    si->prepareFrame(kFrmDefault, "default");
    si->prepareAnimation(kAniMove, "move", -1);
    si->prepareAnimation(kAniDie, "die", 0);
    si->prepareAnimation(kAniAct1, "act1", 10);
    si->prepareAnimation(kAniAct2, "act2", 3);
    si->prepareAnimation(kAniAct3, "act3", 10);
    //si->prepareAnimation(kAniAct4, "act4", 6);
    si->prepareAnimation(kAniAct5, "act5", 6);

    d = new CUnitDrawForCC(si);
    d->setGeometry(31.0f, 25.0f, Point(72.0f / 144, 37.0f / 156), CPoint(33.0f, 67.0f));
    u = new CUnit(si->getName(), d);
    u->setName(si->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kCrystal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kMagical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(200.0f);
    atk->addCastAnimation(kAniAct1);
    atk->setTemplateProjectile(kWizardBolt);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kWizard, u);

    m_bDefLoaded = true;
}
