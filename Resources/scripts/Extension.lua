if __EXTENSION__ then return end
__EXTENSION__ = true


function loadAnimations(name, anis)
	for n, d in pairs(anis) do
		local p = string.format("Sprites/%s/%s", name, n)
		loadAnimation(p, p, d)
	end
end

function logf(...)
    log(string.format(table.unpack({...})))
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
Ability.kOnHpChangedTrigger = 2 ^ 3
Ability.kOnTickTrigger = 2 ^ 4
Ability.kOnAttackTargetTrigger = 2 ^ 5
Ability.kOnAttackedTrigger = 2 ^ 6
Ability.kOnDamagedSurfaceTrigger = 2 ^ 7
Ability.kOnDamagedInnerTrigger = 2 ^ 8
Ability.kOnDamagedDoneTrigger = 2 ^ 9
Ability.kOnDamageTargetDoneTrigger = 2 ^ 10
Ability.kOnProjectileEffectTrigger = 2 ^ 11
Ability.kOnProjectileArriveTrigger = 2 ^ 12
Ability.kOnCalcDamageTargetTrigger = 2 ^ 13

Ability.kNoMasked = 0
Ability.kMaskAll = 0xFFFFFFFF
Ability.kMaskActiveTrigger = Ability.kOnAttackTargetTrigger + Ability.kOnDamageTargetDoneTrigger

Ability.kNormal = 0
Ability.kRare = 1
Ability.kEpic = 2
Ability.kLegend = 3


function Unit:sayf(...)
    self:say(string.format(table.unpack({...})))
end

function Unit:getEffectiveUnitsInRange(radius, forceFlags, func)
	local us = getUnits(function (u)
		return self:canEffect(u, forceFlags) and self:getTouchDistance(u) < radius and (not func or func(u))
	end)
	
	return us
end

function Unit:getNearestEffectiveUnitInRange(radius, forceFlags)
	return self:getNearestUnitInRange(radius, function (u)
		return self:canEffect(u, forceFlags)
	end)
end

function Unit:getNearestEffectiveInjuredUnitInRange(radius, forceFlags, exMaxHpA, exMaxHpB)
	return self:getNearestUnitInRange(radius, function (u)
		local maxHp = u:getRealMaxHp()
		return self:canEffect(u, forceFlags) and u:getHp() <= (exMaxHpA * maxHp + exMaxHpB)
	end)
end

function Projectile:fireStraight(x, y, x1, y1, speed)
    self:setPenaltyFlags(Projectile.kOnContact)
    self:setFireType(Projectile.kFireStraight)
    self:setMoveSpeed(speed)
    self:setPosition(x, y)
    self:setFromToType(Projectile.kPointToPoint)
    self:setFromPoint(self:getPosition())
    self:setToPoint(x1, y1)
    --self:setSrcUnit(me:getId())
    --self:setContactLeft(1)
    --local ad = AttackData:new()
    --ad:setAttack(AttackValue.kMagical, 20)
    --self:setAttackData(ad)
    self:fire()
    
end
