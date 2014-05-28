function table.copy(st)
    local tab = {}
    st = st or {}
    for k, v in pairs(st) do
        if type(v) ~= "table" then
            tab[k] = v
        else
            tab[k] = table.copy(v)
        end
    end
    setmetatable(tab, getmetatable(st))
    return tab
end

function getDirectionPoint(x0, y0, a, dis)
    return x0 + math.cos(-a) * dis, y0 + math.sin(a) * dis
end

function getForwardPoint(x0, y0, x1, y1, dis)
    local a = math.atan2(y1 - y0, x1 - x0)
    return getDirectionPoint(x0, y0, a, dis)
end

if not log then
	--log = print
end

function logf(...)
    log(string.format(table.unpack({...})))
end

function Unit:sayf(self, ...)
    self:say(string.format(table.unpack({...})))
end

INFINITE = -1

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

Sprite.kAniMove = 0
Sprite.kAniDie = 1
Sprite.kAniAct1 = 2
Sprite.kAniAct2 = 3
Sprite.kAniAct3 = 4
Sprite.kAniAct4 = 5
Sprite.kAniAct5 = 6
Sprite.kAniAct6 = 7
Sprite.kAniAct7 = 8
Sprite.kAniAct8 = 9

Sprite.kFrmDefault = 0

Unit.kDying = 2 ^ 16
Unit.kMoving = 2 ^ 17
Unit.kObstinate = 2 ^ 18
Unit.kAlongPath = 2 ^ 19
Unit.kCasting = 2 ^ 20
Unit.kSpinning = 2 ^ 21

Projectile.kOnDying = 2 ^ 0
Projectile.kOnContact = 2 ^ 1

Projectile.kPointToUnit = 0
Projectile.kPointToPoint = 1
Projectile.kUnitToUnit = 2
Projectile.kUnitToPoint = 3

Projectile.kFireFollow = 0
Projectile.kFireLink = 1
Projectile.kFireStraight = 2

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

Ability.kNormal = 0
Ability.kRare = 1
Ability.kEpic = 2
Ability.kLegend = 3

-- Unit Library
UL = {}
UL.BASE_ID = 0x100
UL.kMalik = UL.BASE_ID + 0
UL.kMage = UL.BASE_ID + 1
UL.kBarracks = UL.BASE_ID + 2
UL.kArcher = UL.BASE_ID + 3
UL.kFrost = UL.BASE_ID + 4
UL.kArtillery = UL.BASE_ID + 5
UL.kElemental = UL.BASE_ID + 6
UL.kOni = UL.BASE_ID + 7
UL.kThor = UL.BASE_ID + 8
UL.kViking = UL.BASE_ID + 9
UL.kLumberjack = UL.BASE_ID + 10
UL.kAlric = UL.BASE_ID + 11
UL.kAlien = UL.BASE_ID + 12
UL.kBeastMaster = UL.BASE_ID + 13
UL.kCrabman = UL.BASE_ID + 14
UL.kGiant = UL.BASE_ID + 15
UL.kMinotaur = UL.BASE_ID + 16
UL.kMirage = UL.BASE_ID + 17
UL.kPirate = UL.BASE_ID + 18
UL.kPriest = UL.BASE_ID + 19
UL.kVhelsing = UL.BASE_ID + 20
UL.kSoldier = UL.BASE_ID + 21
UL.kTemplar = UL.BASE_ID + 22
UL.kBarrack = UL.BASE_ID + 23
UL.kTemplars = UL.BASE_ID + 24
UL.kArcane = UL.BASE_ID + 25
UL.kTesla = UL.BASE_ID + 26
UL.kWizard = UL.BASE_ID + 27

-- Projectile Library
PL = {}
PL.BASE_ID = 0x100
PL.kMageBolt = PL.BASE_ID + 0
PL.kFrostBolt = PL.BASE_ID + 1
PL.kArcherArrow = PL.BASE_ID + 2
PL.kLightning = PL.BASE_ID + 3
PL.kThorHammer = PL.BASE_ID + 4
PL.kLumberjackProy = PL.BASE_ID + 5
PL.kAlienProy = PL.BASE_ID + 6
PL.kVoodooProy = PL.BASE_ID + 7
PL.kGiantProy = PL.BASE_ID + 8
PL.kMirageProy = PL.BASE_ID + 9
PL.kKungFuProy = PL.BASE_ID + 10
PL.kPirateProy = PL.BASE_ID + 11
PL.kPriestBolt = PL.BASE_ID + 12
PL.kWizardBolt = PL.BASE_ID + 13
PL.kWizardLightning = PL.BASE_ID + 14
PL.kWizardProy = PL.BASE_ID + 15
PL.kArcaneRay = PL.BASE_ID + 16
PL.kArcaneRay2 = PL.BASE_ID + 17
PL.kTeslaRay = PL.BASE_ID + 18
PL.kArcherArrow2 = PL.BASE_ID + 19

-- Ability Library
AL = {}
AL.BASE_ID = 0x100
AL.kCurse = AL.BASE_ID + 0
