function table.copy(st)
    local tab = {}
    st = st or {}
    for k, v in pairs(st) do
        if type(v) ~= "table" then
            tab[k] = v
        else
            tab[k] = copyTab(v)
        end
    end
    setmetatable(tab, getmetatable(st))
    return tab
end

CommandTarget = {}
CommandTarget.kNoTarget = 0
CommandTarget.kUnitTarget = 1
CommandTarget.kPointTarget = 2

UnitForce = {}
UnitForce.kSelf = 2 ^ 0
UnitForce.kOwn = 2 ^ 1
UnitForce.kAlly = 2 ^ 2
UnitForce.kEnemy = 2 ^ 3

AttackValue = {}
AttackValue.kPhysical = 0
AttackValue.kMagical = 1
AttackValue.kSiege = 2
AttackValue.kHoly = 3

ArmorValue = {}
ArmorValue.kHeavy = 0
ArmorValue.kCrystal = 1
ArmorValue.kWall = 2
ArmorValue.kHero = 3
ArmorValue.kHoly = 4

Unit.kAniMove = 0
Unit.kAniDie = 1
Unit.kAniAct1 = 2
Unit.kAniAct2 = 3
Unit.kAniAct3 = 4
Unit.kAniAct4 = 5
Unit.kAniAct5 = 6
Unit.kAniAct6 = 7
Unit.kAniAct7 = 8
Unit.kAniAct8 = 9

Unit.kDying = 2 ^ 16
Unit.kMoving = 2 ^ 17
Unit.kObstinate = 2 ^ 18
Unit.kAttacking = 2 ^ 19
Unit.kCasting = 2 ^ 20
Unit.kSpinning = 2 ^ 21

Ability.kOnReviveTrigger = 2 ^ 0
Ability.kOnDyingTrigger = 2 ^ 1
Ability.kOnDeadTrigger = 2 ^ 2
Ability.kOnChangeHpTrigger = 2 ^ 3
Ability.kOnTickTrigger = 2 ^ 4
Ability.kOnAttackTargetTrigger = 2 ^ 5
Ability.kOnAttackedTrigger = 2 ^ 6
Ability.kOnDamagedSurfaceTrigger = 2 ^ 7
Ability.kOnDamagedInnerTrigger = 2 ^ 8
Ability.kOnDamagedDoneTrigger = 2 ^ 9
Ability.kOnDamageTargetDoneTrigger = 2 ^ 10
Ability.kOnProjectileEffectTrigger = 2 ^ 11
Ability.kOnProjectileArriveTrigger = 2 ^ 12

Ability.kNoMasked = 0
Ability.kMaskAll = 0xFFFFFFFF
Ability.kMaskActiveTrigger = Ability.kOnAttackTargetTrigger + Ability.kOnDamageTargetDoneTrigger

UL = {}
UL.kMalik = 0x100 + 0
UL.kMage = 0x100 + 1
UL.kBarracks = 0x100 + 2
UL.kArcher = 0x100 + 3
UL.kFrost = 0x100 + 4
UL.kArtillery = 0x100 + 5
UL.kElemental = 0x100 + 6
UL.kOni = 0x100 + 7
UL.kThor = 0x100 + 8
UL.kViking = 0x100 + 9
UL.kLumberjack = 0x100 + 10
UL.kAlric = 0x100 + 11
UL.kAlien = 0x100 + 12
UL.kBeastMaster = 0x100 + 13
UL.kCrabman = 0x100 + 14
UL.kGiant = 0x100 + 15
UL.kMinotaur = 0x100 + 16
UL.kMirage = 0x100 + 17
UL.kPirate = 0x100 + 18
UL.kPriest = 0x100 + 19
UL.kVhelsing = 0x100 + 20
UL.kSoldier = 0x100 + 21
UL.kTemplar = 0x100 + 22
UL.kBarrack = 0x100 + 23
UL.kTemplars = 0x100 + 24
UL.kArcane = 0x100 + 25
UL.kTesla = 0x100 + 26
UL.kWizard = 0x100 + 27

PL = {}
PL.kMageBolt = 0x100 + 0
PL.kFrostBolt = 0x100 + 1
PL.kArcherArrow = 0x100 + 2
PL.kLightning = 0x100 + 3
PL.kThorHammer = 0x100 + 4
PL.kLumberjackProy = 0x100 + 5
PL.kAlienProy = 0x100 + 6
PL.kVoodooProy = 0x100 + 7
PL.kGiantProy = 0x100 + 8
PL.kMirageProy = 0x100 + 9
PL.kKungFuProy = 0x100 + 10
PL.kPirateProy = 0x100 + 11
PL.kPriestBolt = 0x100 + 12
PL.kWizardBolt = 0x100 + 13
PL.kWizardLightning = 0x100 + 14
PL.kWizardProy = 0x100 + 15
PL.kArcaneRay = 0x100 + 16
PL.kArcaneRay2 = 0x100 + 17
PL.kTeslaRay = 0x100 + 18

