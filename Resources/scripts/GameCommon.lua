if __GAME_COMMON__ then return end
__GAME_COMMON__ = true


include("Common.lua")


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
Ability.kOnCalcDamageTargetTrigger = 2 ^ 13

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
include("AbilityLibrary.lua")
AL = {}
--AL.BASE_ID = 0x100
--AL.kCurse = AL.BASE_ID + 0
loadAbilityLibrary()

function Unit:sayf(self, ...)
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

UnitGroup = class()
function UnitGroup:ctor()
	self.units = {}
end

function UnitGroup.getEffectiveUnitsInRange(x, y, unit, radius, forceFlags, func)
	local ug = UnitGroup:new()
	ug.units = getUnits(function (u)
		return unit:canEffect(u, forceFlags) and u:getTouchDistance(x, y) < radius and (not func or func(u))
	end)
	
	return ug
end

function UnitGroup:getRandomUnit()
	local num = #self.units
	if num == 0 then
		return nil
	end
	
	return self.units[math.random(num)]
end

function UnitGroup:addBuffAbility(buff, src, level)
	for _, u in ipairs(self.units) do
		u:addBuffAbility(buff, src, level)
	end
end

function UnitGroup:damaged(ad, src, mask)
	for _, u in ipairs(self.units) do
		u:damaged(ad, src, mask)
	end
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


UnitCreator = class()
function UnitCreator:ctor()
	
	-- id
	self.id = UL.kSoldier
	
	-- name
	self.nm = "Unnamed"
	
	-- level
	self.l = 1
	
	-- max hp
	self.hp = 10
	
	-- move speed
	self.ms = 50
	
	-- hostility range
	self.hr = 400
	
	-- force
	self.force = {--[[index: force index; ally: ally mask]]}
	self.force.index = 0
	self.force.ally = 0
	
	-- attack
	self.atk = {--[[t: type; v: value; i: interval]]}
	self.atk.t = AttackValue.kPhysical
	self.atk.v = 10.0
	self.atk.i = 1.5
	self.atk.r = 0.15
	
	-- armor
	self.amr = {--[[t: type; v: value]]}
	self.amr.t = ArmorValue.kHeavy
	self.amr.v = 0.0
	
	-- ability
	self.act = {--[[array of active ability]]}
	self.pas = {--[[array of passive ability]]}
	
	-- reward
	self.reward = {--[[g: gold; e: exp]]}
	self.reward.g = 10
	self.reward.e = 10
	
	-- AI(Optional)
	self.ai = nil
	
	-- LevelUpdate(Optional)
	self.lu = nil
end

function UnitCreator:createUnit()
	local u = createUnit(self.id)
	
	u:setName(self.nm)
	
	u:setMaxLevel(self.l)
	u:setLevel(self.l)
	
	u:setMaxHp(self.hp)

	u:setBaseMoveSpeed(self.ms)

	u:setHostilityRange(self.hr)

	u:setForceByIndex(self.force.index)
	u:setAlly(self.force.ally)
	
	local atk = u:getAttackAbility()
    atk:setBaseAttack(self.atk.t, self.atk.v)
    atk:setBaseAttackInterval(self.atk.i)
	atk:setAttackValueRandomRange(self.atk.r)
	
	u:setBaseArmor(self.amr.t, self.amr.v)
	
	for _, act in ipairs(self.act) do
		u:addActiveAbility(act)
	end
	
	for _, pas in ipairs(self.pas) do
		u:addPassiveAbility(pas)
	end
	
	u:setRewardGold(self.reward.g)
	u:setRewardExp(self.reward.e)
	
	if self.ai then
		u:setAI(self.ai)
	end
	
	if self.lu then
		u:setLevelUpdate(self.lu)
	end
	
	return u
end

Battle = class()
function Battle:ctor()
end

function Battle:onInit()
end

function Battle:onTick(dt)
end

Battle.DEFAULT = Battle:new()
Battle.running = Battle.DEFAULT

function Battle:run()
	Battle.running = self
	onWorldInit = function()
		showDebug(false)
		math.randomseed(os.time())
		Battle.running:onInit()
	end
	
	onWorldTick = function(dt)
		Battle.running:onTick(dt)
	end
end

include("AI.lua")
include("AI2.lua")

include("LevelUpdate.lua")

function addAbilitiesForUnit(u, id)
	if id == UL.kMage then
		u:addActiveAbility(AL.kGravitySurf:getId())
		u:addActiveAbility(SAL.kMageRain, 3)
		u:addActiveAbility(AL.kKnockBack:getId())
		
	elseif id == UL.kFrost then
		u:addActiveAbility(AL.kSnowStorm:getId())
		u:addActiveAbility(AL.kKnockBack:getId())
		u:addPassiveAbility(AL.kSlowDownAttack:getId())
		
	elseif id == UL.kArcher then
		u:addActiveAbility(AL.kArrowWind:getId())
		u:addActiveAbility(AL.kSpeedUp2:getId())
		u:addPassiveAbility(AL.kCriticalAttack:getId())
		
	elseif id == UL.kElemental then
		u:addActiveAbility(AL.kSerialExplode:getId())
		u:addActiveAbility(AL.kBouncingFireBall:getId())
		u:addPassiveAbility(AL.kArmorBreakAttack:getId())
		u:addPassiveAbility(AL.kAttackedSpeedUp:getId())
		
	elseif id == SUL.kPriest then
		u:addActiveAbility(AL.kHealWind:getId())
		u:addActiveAbility(AL.kSweetDew:getId())
		
	elseif id == UL.kArtillery then
		u:addActiveAbility(AL.kCutter:getId())
		u:addActiveAbility(AL.kShotGun:getId())
		u:addPassiveAbility(AL.kKnockBackAttack:getId())
		u:addPassiveAbility(AL.kChangeAttributeAttack:getId())
		
	elseif id == UL.kLumberjack then
		u:addActiveAbility(AL.kMissileStorm:getId())
		u:addPassiveAbility(AL.kStrikeBack:getId())
		u:addPassiveAbility(AL.kTransitiveAttack:getId())
		
	elseif id == UL.kThor then
		u:addActiveAbility(AL.kThunderHammer:getId())
		u:addPassiveAbility(AL.kLightningAttack:getId())
		u:addPassiveAbility(AL.kStunAttack:getId())
		
	elseif id == UL.kAlric then
		u:addActiveAbility(SAL.kMultySlash, 3)
		u:addActiveAbility(AL.kFastMoveToBack:getId())
		
	elseif id == UL.kMalik then
		u:addActiveAbility(AL.kThunderCap:getId())
		u:addPassiveAbility(AL.kSpeedUpAttack:getId())
		u:addPassiveAbility(AL.kDoubleAttack:getId())
		u:addPassiveAbility(AL.kStunAttack:getId())
		u:addPassiveAbility(AL.kThunderCapAttack:getId())
		
	elseif id == UL.kBarracks then
		u:addActiveAbility(AL.kShieldBash:getId())
		u:addActiveAbility(AL.kArmorUpCry:getId())
		u:addActiveAbility(AL.kSpeedUpCry:getId())
		u:addPassiveAbility(AL.kEvadeHeal:getId())
		
	elseif id == UL.kViking then
		u:addActiveAbility(AL.kSpeedUp:getId())
		u:addActiveAbility(AL.kChargeJump:getId())
		u:addPassiveAbility(AL.kBerserkerBlood:getId())
		u:addPassiveAbility(AL.kVampireAttack:getId())
		
	elseif id == UL.kOni then
		u:addActiveAbility(AL.kSwordWave:getId())
		u:addPassiveAbility(AL.kDamageIncreaceAttack:getId())
		u:addPassiveAbility(AL.kBleedAttack:getId())
		u:addPassiveAbility(AL.kSplashAttack:getId())
		
	end
end

function initForHero()
	local hero = getControlUnit()
    hero:setLevelUpdate(HeroLevelUpdate:new())
	hero:setForceByIndex(0)
	hero:setAlly(2 ^ 0 + 2 ^ 2)
	
	--local a = TeleportAct:new("TP", 1.0, Sprite.kAniAct6, Sprite.kAniAct7)
	--a:setImageName("UI/Ability/Ability03.png")
	--hero:addActiveAbility(a)
	--hero:addActiveAbility(AL.kBuffMaker:getId())
	--hero:addActiveAbility(AL.kChangeAttack:getId())
	hero:addActiveAbility(AL.kFastMoveToBack:getId())
	--hero:addActiveAbility(AL.kMissileStorm:getId())
	hero:addActiveAbility(AL.kSwordWave:getId())
	--hero:addActiveAbility(AL.kBouncingFireBall:getId())
	--hero:addActiveAbility(AL.kArrowWind:getId())
	
	
	return hero
end

