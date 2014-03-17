#include "CommHeader.h"

#include "UnitLibraryForCC.h"
#include "Unit.h"
#include "Ability.h"
#include "GameControl.h"
#include "DrawForCC.h"


CUnitLibraryForCC::CUnitLibraryForCC(void)
{
}

bool CUnitLibraryForCC::init()
{
    CProjectileForCC* p = NULL;
    CUnitDrawForCC* d = NULL;
    CUnit* u = NULL;
    CAttackAct* atk = NULL;
    M_DEF_GC(gc);

    // MageBolt
    gc->loadAnimation("Units/MageBolt/move", "Units/MageBolt/move", 0.1f);
    gc->loadAnimation("Units/MageBolt/die", "Units/MageBolt/die", 0.1f);
    p = new CProjectileForCC("MageBolt");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(50.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kMageBolt, p);

    // FrostBolt
    gc->loadAnimation("Units/FrostBolt/move", "Units/FrostBolt/move", 0.1f);
    gc->loadAnimation("Units/FrostBolt/die", "Units/FrostBolt/die", 0.1f);
    p = new CProjectileForCC("FrostBolt");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(10.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kFrostBolt, p);

    // ArcherArrow
    gc->loadAnimation("Units/ArcherArrow/move", "Units/ArcherArrow/move", 0.1f);
    gc->loadAnimation("Units/ArcherArrow/die", "Units/ArcherArrow/die", 0.1f);
    p = new CProjectileForCC("ArcherArrow");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(20.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kArcherArrow, p);

    // Lightning
    gc->loadAnimation("Units/Lightning/die", "Units/Lightning/die", 0.05f);
    p = new CProjectileForCC("Lightning");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kLightning, p);

    // ThorHammer
    gc->loadAnimation("Units/ThorHammer/move", "Units/ThorHammer/move", 0.1f);
    gc->loadAnimation("Units/ThorHammer/die", "Units/ThorHammer/die", 0.1f);
    p = new CProjectileForCC("ThorHammer");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setMoveSpeed(800.0f);
    p->setMaxHeightDelta(0.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kThorHammer, p);

    // LumberjackProy
    gc->loadAnimation("Units/LumberjackProy/move", "Units/LumberjackProy/move", 0.1f);
    gc->loadAnimation("Units/LumberjackProy/die", "Units/LumberjackProy/die", 0.1f);
    p = new CProjectileForCC("LumberjackProy");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(20.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    addProjectile(kLumberjackProy, p);

    // ArcaneRay
    gc->loadAnimation("Units/ArcaneRay/die", "Units/ArcaneRay/die", 0.1f);
    p = new CProjectileForCC("ArcaneRay");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kArcaneRay, p);

    // ArcaneRay2
    gc->loadAnimation("Units/ArcaneRay2/die", "Units/ArcaneRay2/die", 0.1f);
    p = new CProjectileForCC("ArcaneRay2");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kArcaneRay2, p);

    // TeslaRay
    gc->loadAnimation("Units/TeslaRay/die", "Units/TeslaRay/die", 0.1f);
    p = new CProjectileForCC("TeslaRay");
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    addProjectile(kTeslaRay, p);


    // Malik
    gc->loadAnimation("Units/Malik/move", "Units/Malik/move", 0.08f);
    gc->loadAnimation("Units/Malik/die", "Units/Malik/die", 0.1f);
    gc->loadAnimation("Units/Malik/act1", "Units/Malik/act1", 0.1f);
    gc->loadAnimation("Units/Malik/act2", "Units/Malik/act2", 0.1f);
    gc->loadAnimation("Units/Malik/act3", "Units/Malik/act3", 0.1f);
    gc->loadAnimation("Units/Malik/act4", "Units/Malik/act4", 0.1f);
    gc->loadAnimation("Units/Malik/act5", "Units/Malik/act5", 0.1f);
    d = new CUnitDrawForCC("Malik");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 3);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 3);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 8);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 7);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 8);
    d->setGeometry(31.0f, 30.0f, ccp(96.0 / 192, 20.0 / 200), CPoint(40.0f, 26.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kMalik, u);

    // Mage
    gc->loadAnimation("Units/Mage/move", "Units/Mage/move", 0.08f);
    gc->loadAnimation("Units/Mage/die", "Units/Mage/die", 0.1f);
    gc->loadAnimation("Units/Mage/act1", "Units/Mage/act1", 0.1f);
    gc->loadAnimation("Units/Mage/act2", "Units/Mage/act2", 0.1f);
    gc->loadAnimation("Units/Mage/act3", "Units/Mage/act3", 0.1f);
    gc->loadAnimation("Units/Mage/act4", "Units/Mage/act4", 0.1f);
    gc->loadAnimation("Units/Mage/act5", "Units/Mage/act5", 0.1f);
    d = new CUnitDrawForCC("Mage");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 5);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 6);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 10);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 4);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 9);
    d->setGeometry(22.0f, 18.0f, ccp(61.0 / 120, 21.0 / 152), CPoint(15.0f, 26.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kMagical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(250.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    atk->setTemplateProjectile(kMageBolt);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kMage, u);

    // Barracks
    gc->loadAnimation("Units/Barracks/move", "Units/Barracks/move", 0.08f);
    gc->loadAnimation("Units/Barracks/die", "Units/Barracks/die", 0.1f);
    gc->loadAnimation("Units/Barracks/act1", "Units/Barracks/act1", 0.1f);
    gc->loadAnimation("Units/Barracks/act2", "Units/Barracks/act2", 0.1f);
    gc->loadAnimation("Units/Barracks/act3", "Units/Barracks/act3", 0.1f);
    gc->loadAnimation("Units/Barracks/act4", "Units/Barracks/act4", 0.1f);
    gc->loadAnimation("Units/Barracks/act5", "Units/Barracks/act5", 0.1f);
    d = new CUnitDrawForCC("Barracks");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 3);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 3);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 5);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 18);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 8);
    d->setGeometry(28.0f, 19.0f, ccp(64.0 / 128, 22.0 / 152), CPoint(37.0f, 23.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kBarracks, u);

    // Archer
    gc->loadAnimation("Units/Archer/move", "Units/Archer/move", 0.08f);
    gc->loadAnimation("Units/Archer/die", "Units/Archer/die", 0.1f);
    gc->loadAnimation("Units/Archer/act1", "Units/Archer/act1", 0.1f);
    gc->loadAnimation("Units/Archer/act2", "Units/Archer/act2", 0.1f);
    gc->loadAnimation("Units/Archer/act3", "Units/Archer/act3", 0.1f);
    gc->loadAnimation("Units/Archer/act4", "Units/Archer/act4", 0.1f);
    gc->loadAnimation("Units/Archer/act5", "Units/Archer/act5", 0.1f);
    d = new CUnitDrawForCC("Archer");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 4);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 13);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 6);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(20.0f, 17.0f, ccp(60.0 / 120, 24.0 / 152), CPoint(28.0f, 35.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(250.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->setTemplateProjectile(kArcherArrow);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kArcher, u);

    // Frost
    gc->loadAnimation("Units/Frost/move", "Units/Frost/move", 0.08f);
    gc->loadAnimation("Units/Frost/die", "Units/Frost/die", 0.1f);
    gc->loadAnimation("Units/Frost/act1", "Units/Frost/act1", 0.1f);
    gc->loadAnimation("Units/Frost/act2", "Units/Frost/act2", 0.1f);
    gc->loadAnimation("Units/Frost/act3", "Units/Frost/act3", 0.1f);
    gc->loadAnimation("Units/Frost/act4", "Units/Frost/act4", 0.1f);
    gc->loadAnimation("Units/Frost/act5", "Units/Frost/act5", 0.1f);
    d = new CUnitDrawForCC("Frost");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 6);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 5);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 7);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(27.0f, 27.0f, ccp(83.0 / 164, 36.0 / 208), CPoint(42.0f, 31.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(500.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kMagical, 60.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(250.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->setTemplateProjectile(kFrostBolt);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(50.0f);
    d->setHostilityRange(300.0f);
    addUnit(kFrost, u);

    // Artillery
    gc->loadAnimation("Units/Artillery/move", "Units/Artillery/move", 0.08f);
    gc->loadAnimation("Units/Artillery/die", "Units/Artillery/die", 0.1f);
    gc->loadAnimation("Units/Artillery/act1", "Units/Artillery/act1", 0.1f);
    gc->loadAnimation("Units/Artillery/act2", "Units/Artillery/act2", 0.1f);
    gc->loadAnimation("Units/Artillery/act3", "Units/Artillery/act3", 0.1f);
    gc->loadAnimation("Units/Artillery/act4", "Units/Artillery/act4", 0.1f);
    gc->loadAnimation("Units/Artillery/act5", "Units/Artillery/act5", 0.1f);
    d = new CUnitDrawForCC("Artillery");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 8);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 2);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 3);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(26.0f, 20.0f, ccp(91.0 / 184, 39.0 / 164), CPoint(36.0f, 46.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(180.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kArtillery, u);

    // Elemental
    gc->loadAnimation("Units/Elemental/move", "Units/Elemental/move", 0.08f);
    gc->loadAnimation("Units/Elemental/die", "Units/Elemental/die", 0.1f);
    gc->loadAnimation("Units/Elemental/act1", "Units/Elemental/act1", 0.1f);
    gc->loadAnimation("Units/Elemental/act2", "Units/Elemental/act2", 0.1f);
    gc->loadAnimation("Units/Elemental/act3", "Units/Elemental/act3", 0.1f);
    gc->loadAnimation("Units/Elemental/act5", "Units/Elemental/act5", 0.1f);
    d = new CUnitDrawForCC("Elemental");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 4);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 6);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 8);
    d->setGeometry(31.0f, 28.0f, ccp(59.0 / 120, 16.0 / 144), CPoint(40.0f, 30.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kElemental, u);

    // Oni
    gc->loadAnimation("Units/Oni/move", "Units/Oni/move", 0.08f);
    gc->loadAnimation("Units/Oni/die", "Units/Oni/die", 0.1f);
    gc->loadAnimation("Units/Oni/act1", "Units/Oni/act1", 0.08f);
    gc->loadAnimation("Units/Oni/act2", "Units/Oni/act2", 0.1f);
    gc->loadAnimation("Units/Oni/act3", "Units/Oni/act3", 0.1f);
    gc->loadAnimation("Units/Oni/act5", "Units/Oni/act5", 0.1f);
    d = new CUnitDrawForCC("Oni");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 9);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 9);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 19);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(38.0f, 31.0f, ccp(130.0 / 256, 33.0 / 224), CPoint(105.0f, 10.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct3);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kOni, u);

    // Thor
    gc->loadAnimation("Units/Thor/move", "Units/Thor/move", 0.08f);
    gc->loadAnimation("Units/Thor/die", "Units/Thor/die", 0.1f);
    gc->loadAnimation("Units/Thor/act1", "Units/Thor/act1", 0.1f);
    gc->loadAnimation("Units/Thor/act2", "Units/Thor/act2", 0.08f);
    gc->loadAnimation("Units/Thor/act3", "Units/Thor/act3", 0.1f);
    gc->loadAnimation("Units/Thor/act5", "Units/Thor/act5", 0.1f);
    d = new CUnitDrawForCC("Thor");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 6);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 5);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 4);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 3);
    d->setGeometry(40.0f, 29.0f, ccp(124.0 / 240, 52.0 / 192), CPoint(59.0f, 30.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 6.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 55.5f),
        0.1261f);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    //atk->setTemplateProjectile(kLightning);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kThor, u);
    
    // Viking
    gc->loadAnimation("Units/Viking/move", "Units/Viking/move", 0.08f);
    gc->loadAnimation("Units/Viking/die", "Units/Viking/die", 0.1f);
    gc->loadAnimation("Units/Viking/act1", "Units/Viking/act1", 0.1f);
    gc->loadAnimation("Units/Viking/act2", "Units/Viking/act2", 0.1f);
    gc->loadAnimation("Units/Viking/act3", "Units/Viking/act3", 0.1f);
    gc->loadAnimation("Units/Viking/act5", "Units/Viking/act5", 0.1f);
    d = new CUnitDrawForCC("Viking");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 7);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 8);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(38.0f, 32.0f, ccp(141.0 / 284, 44.0 / 232), CPoint(83.0f, 22.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kViking, u);

    // Lumberjack
    gc->loadAnimation("Units/Lumberjack/move", "Units/Lumberjack/move", 0.08f);
    gc->loadAnimation("Units/Lumberjack/die", "Units/Lumberjack/die", 0.1f);
    gc->loadAnimation("Units/Lumberjack/act1", "Units/Lumberjack/act1", 0.1f);
    gc->loadAnimation("Units/Lumberjack/act2", "Units/Lumberjack/act2", 0.1f);
    gc->loadAnimation("Units/Lumberjack/act3", "Units/Lumberjack/act3", 0.1f);
    gc->loadAnimation("Units/Lumberjack/act5", "Units/Lumberjack/act5", 0.1f);
    d = new CUnitDrawForCC("Lumberjack");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 6);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 6);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 6);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 6);
    d->setGeometry(51.0f, 45.0f, ccp(91.0 / 180, 31.0 / 220), CPoint(62.0f, 41.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    atk->addCastAnimation(CUnitDraw::kAniAct3);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kLumberjack, u);

    // Alric
    gc->loadAnimation("Units/Alric/move", "Units/Alric/move", 0.08f);
    gc->loadAnimation("Units/Alric/die", "Units/Alric/die", 0.1f);
    gc->loadAnimation("Units/Alric/act1", "Units/Alric/act1", 0.1f);
    gc->loadAnimation("Units/Alric/act2", "Units/Alric/act2", 0.1f);
    gc->loadAnimation("Units/Alric/act3", "Units/Alric/act3", 0.1f);
    gc->loadAnimation("Units/Alric/act4", "Units/Alric/act4", 0.1f);
    gc->loadAnimation("Units/Alric/act5", "Units/Alric/act5", 0.1f);
    d = new CUnitDrawForCC("Alric");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 3);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 8);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 10);
    d->prepareAnimation(CUnitDraw::kAniAct5, "act5", 3);
    d->setGeometry(26.0f, 24.0f, ccp(83.0 / 164, 17.0 / 180), CPoint(62.0f, 26.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1000.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 0.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    atk->addCastAnimation(CUnitDraw::kAniAct3);
    u->addActiveAbility(atk);
    d->setBaseMoveSpeed(80.0f);
    addUnit(kAlric, u);



    // Soldier
    gc->loadAnimation("Units/Soldier/move", "Units/Soldier/move", 0.08f);
    gc->loadAnimation("Units/Soldier/die", "Units/Soldier/die", 0.1f);
    gc->loadAnimation("Units/Soldier/act1", "Units/Soldier/act1", 0.08f);
    gc->loadAnimation("Units/Soldier/act2", "Units/Soldier/act2", 0.1f);
    gc->loadAnimation("Units/Soldier/act3", "Units/Soldier/act3", 0.1f);
    gc->loadAnimation("Units/Soldier/act5", "Units/Soldier/act5", 0.1f);
    d = new CUnitDrawForCC("Soldier");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 2);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 3);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 11);
    d->prepareAnimation(CUnitDraw::kAniAct4, "act4", 9);
    d->setGeometry(25.0f, 21.0f, ccp(94.0 / 188, 17.0 / 104), CPoint(40.0f, 21.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(550.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 2.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kPhysical, 21.0),
        0.19f);
    atk->setCastMinRange(-9.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);
    u->setRewardGold(10);
    u->setRewardExp(7);
    d->setBaseMoveSpeed(50.0f);
    addUnit(kSoldier, u);

    // Templar
    gc->loadAnimation("Units/Templar/move", "Units/Templar/move", 0.08f);
    gc->loadAnimation("Units/Templar/die", "Units/Templar/die", 0.1f);
    gc->loadAnimation("Units/Templar/act1", "Units/Templar/act1", 0.07f);
    gc->loadAnimation("Units/Templar/act2", "Units/Templar/act2", 0.1f);
    gc->loadAnimation("Units/Templar/act3", "Units/Templar/act3", 0.1f);
    d = new CUnitDrawForCC("Templar");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 3);
    d->prepareAnimation(CUnitDraw::kAniAct2, "act2", 9);
    d->prepareAnimation(CUnitDraw::kAniAct3, "act3", 2);
    d->setGeometry(19.0f, 20.0f, ccp(74.0 / 144, 16.0 / 84), CPoint(32.0f, 20.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(550.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 2.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kPhysical, 21.0),
        0.19f);
    atk->setCastMinRange(-7.0f);
    atk->setCastRange(20.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    u->addActiveAbility(atk);
    u->setRewardGold(10);
    u->setRewardExp(7);
    d->setBaseMoveSpeed(50.0f);
    addUnit(kTemplar, u);

    // Arcane
    gc->loadAnimation("Units/Arcane/die", "Units/Arcane/die", 0.1f);
    gc->loadAnimation("Units/Arcane/act1", "Units/Arcane/act1", 0.08f);
    d = new CUnitDrawForCC("Arcane");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 9);
    d->setGeometry(63.0f, 60.0f, ccp(120.0 / 240, 44.0 / 184), CPoint(0.0f, 118.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1300.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 18.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kMagical, 110.0),
        0.1818f);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(250.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->setTemplateProjectile(kArcaneRay);
    u->addActiveAbility(atk);
    d->setFixed();
    d->setHostilityRange(800.0f);
    addUnit(kArcane, u);

    // Tesla
    gc->loadAnimation("Units/Tesla/die", "Units/Tesla/die", 0.1f);
    gc->loadAnimation("Units/Tesla/act1", "Units/Tesla/act1", 0.031f);
    d = new CUnitDrawForCC("Tesla");
    d->prepareFrame(CUnitDraw::kFrmDefault, "default");
    d->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    d->prepareAnimation(CUnitDraw::kAniAct1, "act1", 23);
    d->setGeometry(63.0f, 60.0f, ccp(120.0 / 240, 44.0 / 184), CPoint(0.0f, 118.0f));
    u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(d->getName());
    u->setMaxHp(1300.0f);
    u->setBaseArmor(CArmorValue(CArmorValue::kNormal, 18.0f));
    atk = new CAttackAct(
        "Attack",
        "¹¥»÷",
        1.00,
        CAttackValue(CAttackValue::kMagical, 110.0),
        0.1818f);
    atk->setCastMinRange(0.0f);
    atk->setCastRange(250.0f);
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->setTemplateProjectile(kTeslaRay);
    u->addActiveAbility(atk);
    d->setFixed();
    d->setHostilityRange(800.0f);
    addUnit(kTesla, u);

    return true;
}

void CUnitLibraryForCC::addUnit( int id, CUnit* u )
{
    m_mapUnits.addObject(id, u);
}

CUnit* CUnitLibraryForCC::copyUnit( int id ) const
{
    auto it = m_mapUnits.find(id);
    if (it == m_mapUnits.end())
    {
        return NULL;
    }

    return DCAST(it->second->copy(), CUnit*);
}

void CUnitLibraryForCC::addProjectile( int id, CProjectile* p )
{
    m_mapProjectiles.addObject(id, p);
}

CProjectile* CUnitLibraryForCC::copyProjectile( int id ) const
{
    auto it = m_mapProjectiles.find(id);
    if (it == m_mapProjectiles.end())
    {
        return NULL;
    }

    return DCAST(it->second->copy(), CProjectile*);
}
