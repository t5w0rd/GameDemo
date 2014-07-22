include("Init.lua")



hero = nil
me = nil

aaa = {}
c = 0

kill = 0
learned = 0

lvl = 1

taid2 = 0

function spawnSoldier(id, force)
	local u = createUnit(id)
	u:setForceByIndex(force)

	if force == 1 then
		u:setPosition(100 + math.random(-50, 50), 500 + math.random(-50, 50))
		up = UnitPath:new()
		up:addPoint(u:getPosition())
		up:addPoint(1900 + math.random(-50, 50), 500 + math.random(-50, 50))
	else
		u:setPosition(1900 + math.random(-50, 50), 500 + math.random(-50, 50))
		up = UnitPath:new()
		up:addPoint(u:getPosition())
		up:addPoint(100 + math.random(-50, 50), 500 + math.random(-50, 50))
	end
	
	--u:setExArmorValue(1.0, 2)
	u:moveAlongPath(up, false)

	return u
end

function spawnHero(id, force)
	if not id then
		id = math.random(1, 12) + 0xff
	end
	
	if not force then
		force = 1
	end
	
	local u = spawnSoldier(id, force)
	u:setMaxHp(me:getRealMaxHp() * 0.4 + 400 + (kill / 1.0) * 150)
	if me:getLevel() > 15 and math.random() < 0.1 then
		u:setExArmorValue(1 + kill / 10, kill)
	end
	u:setRewardGold(50 + kill * 20 / 1.5)
	u:setRewardExp(20 + kill * 10 / 1.5)
	local at, av = u:getBaseArmor()
	u:setBaseArmor(at, av + kill * 0.7)
	local atk = me:getAttackAbility()
	local mt, mv = atk:getBaseAttack()
	atk = u:getAttackAbility()
	local t, v = atk:getBaseAttack()
	atk:setBaseAttack(t, (v * (1 + kill / 5.0) + kill * 4) * 0.5 + mv * 0.5)
	atk:setExAttackSpeed(1.0 + kill / 30, 0.0)
	for i = 1, (3 + kill / 2) do
		u:addPassiveAbility(aaa[math.random(1, c)])
	end

	if force == 1 then
		hero = u
		u:addPassiveAbility(OnDyingPas:new())
		if atk:getCastRange() > 100 then
			u:setAI(LuaAI:new())
			--u:addActiveAbility(AL.kReflect:getId())
			--u:addActiveAbility(AL.kSpeedUp2:getId())
			--u:addActiveAbility(AL.kKnockBack:getId())
		else
			u:setAI(LuaAI:new())
			--u:addActiveAbility(AL.kThrowHammer:getId())
			--u:addActiveAbility(AL.kThunderCap:getId())
			--u:addActiveAbility(AL.kSpeedUp:getId())
			--u:addActiveAbility(AL.kChargeJump:getId())
		end
	else
		u:setExMaxHp(0.20, 0.0)
		u:setAI(LuaAI:new())
		u:addActiveAbility(AL.kKnockBack:getId())
	end
	
	addAbilitiesForUnit(u, id)
	
	return u
end

function initAbilityForLevelUp()
	a = DoubleAttackPas:new("DA", 0.2, 0.5, 0.0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)

	a = ChangeHpPas:new("Heal", 0.2, 0.0, 0.5, 0.0, 0.0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	
	a = SplashPas:new("SplashBolt", 25, 0.50, 0.0, 50, 0.25, 0.0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)

	--a = BetrayBuff:new(5)
	--id = addTemplateAbility(a)
	--a = AttackBuffMakerPas:new("Betray", 1.0, id, false, 1.0, 0.0, 0)
	--c = c + 1
	--aaa[c] = addTemplateAbility(a)

	a = SpeedBuff:new("SlowDown", "SlowDown", 3, true, -0.1, 0.0, -0.1, 0.0)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("SpeedAttack", 1.0, id, false, 1.0, 0.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)

	a = KnockBackBuff:new("KnockBack", 0.5, 30)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("KnockBackAttack", 0.2, id, false, 1.5, 0.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	
	a = StunBuff:new("Stun", "Stun", 1, false)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("StunAttack", 0.15, id, false, 1.0, 50.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	
	a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -2.0)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("Critical", 0.5, id, false, 1.0, 0.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	
	a = CurseBuff:new(13, false, 15, 4, 30 / 100)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("Critical", 0.2, id, false, 1.0, 0.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)

	a = ChangeHpBuff:new("+HP", "Heal", 3, true, 0.2, 0.0002, 1, 0.0, 0.0)
	id = addTemplateAbility(a)
	a = EvadePas:new("Evade", 0.20, id)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	
	a = VampirePas:new("Vampire", 0.5)
	c = c + 1
	aaa[c] = addTemplateAbility(a)

	a = SpeedBuff:new("SpeedUp", "SpeedUp", 10, true, 0.05, 0.0, 0.1, 0.0)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("Critical", 0.5, id, true, 1.5, 0.0, 0)
	c = c + 1
	aaa[c] = addTemplateAbility(a)
	taid2 = aaa[c]
end

WAVE = 30
el = 0
up1 = UnitPath:new()
up1:addPoint(100, 500)
up1:addPoint(1900, 500)
up2 = UnitPath:new()
up2:addPoint(1900, 500)
up2:addPoint(100, 500)
uc = 0
tower1 = 0
tower2 = 0

Stage00 = class(Battle)
function Stage00:onInit()
	self.healer = 0
	initAbilityForLevelUp()
	
	me = getHero()
	--me:setMaxLevel(100)
	me:addExp(5000)
	
	a = ChangeHpBuff:new("TowerHeal", "TowerHeal", 5, false, 0.3, 0.001, 5, 0, -1)
	id = addTemplateAbility(a)
	a = AuraPas:new("HealAura", 1, id, 200, UnitForce.kAlly + UnitForce.kSelf, false)
	taid = addTemplateAbility(a)
	
	u = createUnit(UL.kArcane)
	u:setMaxLevel(1)
	u:setLevel(1)
	u:setMaxHp(u:getMaxHp() * 5)
	u:setBaseArmor(ArmorValue.kWall, 52)
	u:setForceByIndex(1)
	u:setPosition(100, 500)
	u:addPassiveAbility(taid)
	u:addPassiveAbility(taid2)
	u:addPassiveAbility(AL.kAutoHeal:getId())
	--u:addPassiveAbility(DamageBackPas:new(1.2, 0))
	a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
	id = addTemplateAbility(a)
	a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, false, PL.kArcaneRay)
	a:setAppendBuff(id)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0, 0)
	u:addPassiveAbility(a)
	tower1 = u:getId()

	u = createUnit(UL.kTesla)
	u:setMaxLevel(1)
	u:setLevel(1)
	u:setMaxHp(u:getMaxHp() * 5)
	u:setBaseArmor(ArmorValue.kWall, 52)
	u:setForceByIndex(0)
	u:setPosition(1900, 500)
	u:addPassiveAbility(taid)
	u:addPassiveAbility(taid2)
	u:addPassiveAbility(AL.kAutoHeal:getId())
	--u:addPassiveAbility(DamageBackPas:new(1.0, 0))
	a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
	id = addTemplateAbility(a)
	a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, false, PL.kTeslaRay)
	a:setAppendBuff(id)
	id = addTemplateAbility(a)
	a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0, 0)
	u:addPassiveAbility(a)
	tower2 = u:getId()
	
end

function Stage00:onTick(dt)
	if not getUnit(tower1) then
		stageVictory(math.random(1, 3))
	end
	
	if not getUnit(tower2) then
		stageDefeat()
	end
	
	a = ChangeHpBuff:new("MageHeal", "MageHeal", 5, false, 0.3, 0.006, 0, 0, -1)
	arid = addTemplateAbility(a)
	a = AuraPas:new("HealAura", 1, arid, 220, UnitForce.kAlly + UnitForce.kSelf, false)
	arid = addTemplateAbility(a)
	
	el = el + dt
	if uc < 5 then
		if el > 2 then
			uc = uc + 1
			el = el - 2
			playSound("sounds/Effects/OpenDoor.mp3")
			if uc == 4 then
				u = spawnSoldier(UL.kMage, 1)
				u:setMaxLevel(lvl)
				u:setLevel(lvl)
				u:addPassiveAbility(arid)
				u:addPassiveAbility(SplashPas:new("SplashBolt", 50, 0.75, 0.0, 75, 0.25, 0.0))
				u:setRewardGold(20)
				u:setRewardExp(14)
				u:setMaxHp(200 + lvl * 46)
				atk = u:getAttackAbility()
				atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.2)
				
				u = spawnSoldier(UL.kArcher, 0)
				u:setMaxLevel(lvl)
				u:setLevel(lvl)
				u:setRewardGold(20)
				u:setRewardExp(14)
				u:setMaxHp(200 + lvl * 46)
				atk = u:getAttackAbility()
				atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.2)
				
				if not getUnit(self.healer) then
					u = spawnHero(SUL.kPriest, 0)
					self.healer = u:getId()
					u:setMaxLevel(lvl)
					u:setLevel(lvl)
				end
			else
				u = spawnSoldier(UL.kSoldier, 1)
				u:setMaxLevel(lvl)
				u:setLevel(lvl)
				u:setMaxHp(200 + lvl * 63)
				atk = u:getAttackAbility()
				atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.3)
				u = spawnSoldier(UL.kTemplar, 0)
				u:setMaxLevel(lvl)
				u:setLevel(lvl)
				u:setMaxHp(200 + lvl * 63)
				atk = u:getAttackAbility()
				atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.3)
			end
			if not hero then
				local u = spawnHero()
				u:setMaxLevel(lvl)
				u:setLevel(lvl)
				a = u:getAttackAbility()
				if a:getCastRange() < 150 and math.random(0, 100) < 50 then
					u = spawnHero()
					u:setMaxLevel(lvl)
					u:setLevel(lvl)
					hero:setBaseMoveSpeed(85)
					hero = u
				end
			end
		end
	else
		if el > WAVE then
			-- wave in coming
			playSound("sounds/Effects/WaveIncoming.mp3")
			uc = 0
			el = el - WAVE
		end
	end
end

Stage00:new():run()

pass = 0
interval = 0
angle = 0

Barrage = class()
function Barrage:ctor(interval)
	self.interval = interval
end

function Barrage:getInterval()
	return self.interval
end

function Barrage:onInterval()
end

B01 = class(Barrage)
function B01:onInterval()
	angle = angle + interval * 10
	local a = math.pi * 0.5 * math.sin(angle)
	local x, y = me:getPosition()
	local dis = 1000
	local speed = 500
	local x1, y1 = getDirectionPoint(x, y, a, dis)
	fireStraight(x, y, x1, y1, speed)
	local x1, y1 = getDirectionPoint(x, y, a + math.pi, dis)
	fireStraight(x, y, x1, y1, speed)
end

B02 = class(Barrage)
function B02:onInterval()
	local x, y = me:getPosition()
	local dis = 1000
	local speed = 500
	local b = math.random(0, 360)
	for i = 0 + b, 360 + b, 5 do
		local x1, y1 = getDirectionPoint(x, y, i, dis)
		fireStraight(x, y, x1, y1, speed)
	end
end

B03 = class(Barrage)
function B03:onInterval()
	angle = angle + interval * 20
	local x, y = me:getPosition()
	local dis = 1000
	local speed = 500
	local a = 100
	y = y + math.sin(angle) * a
	local x1, y1 = getForwardPoint(x, y, x - 10, y, dis)
	fireStraight(x, y, x1, y1, speed)
end

B04 = class(Barrage)
function B04:onInterval()
	local tx, ty = 1000, 500
	local x, y = me:getPosition()
	local a = math.atan2(ty - y, tx - x)
	local dis = 1000
	local speed = 400
	local dt = math.random(-300, 300)
	
	x, y = getDirectionPoint(x, y, a + math.pi / 2, dt)
	local x0, y0 = getDirectionPoint(x, y, a, -dis / 2)
	local x1, y1 = getDirectionPoint(x0, y0, a, dis)
	fireStraight(x0, y0, x1, y1, speed)
end

local brg = nil
function test()
	--brg = B01:new(0.01)
	brg = B02:new(2.0)
	--brg = B03:new(0.01)
	--brg = B04:new(0.05)
	interval = brg:getInterval()
	--interval = 0.01
	--p:setPenaltyFlags(Projectile.kOnContact)
	--p:setFireType(Projectile.kFireStraight)
end

function test_tick(dt)
	pass = pass + dt
	if pass >= interval then
		pass = 0
		test_interval()
	end
end

function test_interval()
	--brg:onInterval()
end
