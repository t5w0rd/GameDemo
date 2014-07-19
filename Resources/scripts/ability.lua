if not DEBUG and __ABILITY__ then return end
__ABILITY__ = true

include("UnitLibrary.lua")
include("EffectLibrary.lua")


TestAct = class(ActiveAbility)
function TestAct:ctor(name, cd, castTarget, effectFlags)
	self:sctor("RainAct", name, cd, castTarget, effectFlags)
end

function TestAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	local p = createProjectile(SPL.kPriestBolt)
    p:setPenaltyFlags(Projectile.kOnDying)
	--p:setPenaltyFlags(Projectile.kOnContact)
	p:setFireType(Projectile.kFireFollow)
    --p:setFireType(Projectile.kFireStraight)
    p:setMoveSpeed(400.0)
    p:setPosition(x, y)
    p:setFromToType(Projectile.kPointToUnit)
    p:setFromPoint(p:getPosition())
    --p:setToPoint(x1, y1)
	local t = o:getCastTarget()
	p:setToUnit(t.unit)
    p:setSrcUnit(o:getId())
    --p:setContactLeft(1)
    local ad = AttackData:new()
    ad:setAttack(AttackValue.kMagical, 50)
    p:setAttackData(ad)
    p:fire()
end

BetrayBuff = class(BuffAbility)
function BetrayBuff:ctor(duration)
    self:sctor("Betray", "Betray", duration, false)
end

function BetrayBuff:onUnitAddAbility()
    o = self:getOwner()
    if o:getId() ~= me:getId() and o:isFixed() == false and o:getMaxHp() < 600 then

        o:setForceByIndex(1)
        o:addBuffAbility(StunBuff:new("Stun", "Stun", 2, false))
    end
end

function BetrayBuff:onUnitDelAbility()
    o = self:getOwner()
    o:setForceByIndex(2)
end

CurseBuff = class(BuffAbility)
function CurseBuff:ctor(duration, stackable, damage, interval, multiply)
    self:sctor("CurseBuff", "Curse", duration, stackable)
    self.damage = damage
    self.interval = interval
    self.pass = 0
    self.multiply = multiply
    self.hp = 0
    self:setInterval(0.5)
end

function CurseBuff:onChangeLevel(changed)
	--logf("CurseBuff Level Changed: Lv%d", self:getLevel())
end

function CurseBuff:onUnitAddAbility()
    o = self:getOwner()
    self.hp = o:getHp()
    o:addBattleTip("Curse", nil, 18, 255, 0, 255)
end

function CurseBuff:onUnitInterval()
    local o = self:getOwner()
    local damage = 0
	local show = false
    self.pass = self.pass + 0.5
    if self.pass >= self.interval then
        self.pass = self.pass - self.interval
        damage = (self.hp - o:getHp()) * self.multiply
		show = true
    end

    if damage < 0 then
        damage = 0
    end
    damage = damage + self.damage
    
    local ad = AttackData:new()
    ad:setAttack(AttackValue.kMagical, damage)
    local s = self:getSrcUnit()
    if s then
		if show then
			o:addBattleTip(string.format("-%d", damage), "fonts/Comic Book.fnt", 18, 255, 0, 255)
		end
		
        o:damaged(ad, s, Ability.kMaskActiveTrigger)
    end
    
end

DamageBackPas = class(PassiveAbility)
function DamageBackPas:ctor(per)
    self:sctor("DamageBackPas", "DamageBack", 0)
    self:setTriggerFlags(Ability.kOnDamagedDoneTrigger)
    self.per = per
end

function DamageBackPas:onUnitDamagedDone(damage, src)
    local o = self:getOwner()
    if not o then
        --log("o is nil")
        return
    else
        src:damagedLow(self.per * damage, o, 0xFFFFFFFF)
    end
end

ArmorBuff = class(BuffAbility)
function ArmorBuff:ctor(root, name, dur, stackable, exA, exB)
    self:sctor(root, name, dur, stackable)
    self.exA = exA
    self.exB = exB
    self.change = 0
end

function ArmorBuff:onUnitAddAbility()
    local o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    local av = o:getRealArmorValue()
    --local s = string.format()
    self.change = self.exA * av + self.exB
    o:setExArmorValue(exA, exB + self.change)
    o:addBattleTip(math.ceil(o:getRealArmorValue() - av), "fonts/Comic Book.fnt", 18, 0, 0, 0)
	
	if self.change < 0 then
		local eff = SEL.createEffectOnUnit(SEL.kArmorBreak, o)
		eff:playRelease()
	end
end

function ArmorBuff:onUnitDelAbility()
    o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    o:setExArmorValue(exA, exB - self.change)
end

OnDyingPas = class(PassiveAbility)
OnDyingPas.words = {"Aha, Weakness!", "Goodnight, My Son!", "Taste My Arrow!", "Go To Hell!"}
function OnDyingPas:ctor()
    self:sctor("OnDying", "OnDying", 0)
    self:setTriggerFlags(Ability.kOnDyingTrigger)
end

function OnDyingPas:onUnitDying()
    hero = nil
    if me:isDead() then
        return
    end

	if math.random() < 0.3 then
		me:say(OnDyingPas.words[math.random(1, #OnDyingPas.words)])
	end
	
	local o = self:getOwner()
	if me:getDistance(o) > 400 then
		me:addExp(o:getRewardExp())
	end
	
    lvl = me:getLevel()
    kill = kill + 1
    local atk = me:getAttackAbility()
    local a, b = atk:getExAttackValue()
    atk:setExAttackValue(a + 1 / 50.0, b + 1 * 1.3)
    a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + 1 / 50, b)
    local level = math.ceil(kill / 2)
    if level >= 1 and level <= c then
        if level > learned then
            me:addPassiveAbility(aaa[level])
            learned = level
            me:addBattleTip(string.format("Learn New Ability (%d/%d)", learned, c), "fonts/Comic Book.fnt", 32, 235, 170, 68)
        end
    end

end

SummonUnitAct = class(ActiveAbility)
function SummonUnitAct:ctor(name, cd, castType, id, duration)
    self:sctor("SummonUnit", name, cd, castType, UnitForce.kSelf)
    self.id = id
    self.duration = duration
end

function SummonUnitAct:onUnitAbilityEffect(projectile, target)
    local o = self:getOwner()
    local x, y = self:getAbilityEffectPoint(projectile, target)
    local u = createUnit(self.id)
    u:setMaxHp(100)
    u:setPosition(x, y)
    u:setForce(o:getForce())
    u:setAlly(o:getAlly())
    u:setLevel(self:getLevel())
    u:setGhost(o)
    u:doAnimation(Sprite.kAniMove, INFINITE)
	u:doRotateBy(360, 1.0, INFINITE)

    u:addBuffAbility(LimitedLifeBuff:new("LimitedLife", "LimitedLife", self.duration))
	u:addPassiveAbility(AL.kAttractAura:getId())
    
end

BerserkerBloodPas = class(PassiveAbility)
function BerserkerBloodPas:ctor(name, exMaxHpDeltaA, exMaxHpDeltaB, exMoveSpeedDeltaA, exMoveSpeedDeltaB, exAttackSpeedDeltaA, exAttackSpeedDeltaB, exAttackValueDeltaA, exAttackValueDeltaB)
    self:sctor("BerserkerBloodPas", name)
    self:setTriggerFlags(Ability.kOnChangeHpTrigger)
    self.exMaxHpDeltaA = exMaxHpDeltaA
    self.exMaxHpDeltaB = exMaxHpDeltaB
    self.exMoveSpeedDeltaA = exMoveSpeedDeltaA
    self.exMoveSpeedDeltaB = exMoveSpeedDeltaB
    self.exAttackSpeedDeltaA = exAttackSpeedDeltaA
    self.exAttackSpeedDeltaB = exAttackSpeedDeltaB
    self.exAttackValueDeltaA = exAttackValueDeltaA
    self.exAttackValueDeltaB = exAttackValueDeltaB
    self.lastLvl = 0
end

function BerserkerBloodPas:onUnitAddAbility()
    self.lastLvl = 0
    self:onUnitChangeHp(0)
end

function BerserkerBloodPas:onUnitDelAbility()
    self:changeValueByDelta(-self.lastLvl)
    self.lastLvl = 0
end

function BerserkerBloodPas:onUnitChangeHp(hp)
    local o = self:getOwner()
    local maxHp = o:getRealMaxHp()
    local lvl = (maxHp - o:getHp()) / (self.exMaxHpDeltaA * maxHp + self.exMaxHpDeltaB)
    --logf("lvl %f", lvl)
    local dt = lvl - self.lastLvl
    
    if dt > -1 and dt < 1 then
        return
    end
    
    self:changeValueByDelta(dt)
    
    self.lastLvl = lvl
end

function BerserkerBloodPas:changeValueByDelta(dt)
    --logf("%f x %f = %f", (self.lastLvl + dt), self.exAttackValueDeltaA, (self.lastLvl + dt) * self.exAttackValueDeltaA)
    local o = self:getOwner()
    local exMoveSpeedDeltaA = self.exMoveSpeedDeltaA * dt
    local exMoveSpeedDeltaB = self.exMoveSpeedDeltaB * dt
    local exAttackSpeedDeltaA = self.exAttackSpeedDeltaA * dt
    local exAttackSpeedDeltaB = self.exAttackSpeedDeltaB * dt
    local exAttackValueDeltaA = self.exAttackValueDeltaA * dt
    local exAttackValueDeltaB = self.exAttackValueDeltaB * dt
    
    local a, b = o:getExMoveSpeed()
    o:setExMoveSpeed(a + exMoveSpeedDeltaA, b + exMoveSpeedDeltaB)
    
    local atk = o:getAttackAbility()
    
    a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + exAttackSpeedDeltaA, b + exAttackSpeedDeltaB)
    
    a, b = atk:getExAttackValue()
    atk:setExAttackValue(a + exAttackValueDeltaA, b + exAttackValueDeltaB)
    
end

PressureBombBuff = class(BuffAbility)
function PressureBombBuff:ctor(name, duration, maxDamage, damageType, damageValue, range)
    self:sctor("PressureBombBuff", name, duration, true)
    self:setTriggerFlags(Ability.kOnDamagedDoneTrigger)    
    self.maxDamage = maxDamage
    self.dt = damageType
    self.dv = damageValue
    self.effected = false
	self.range = range
end

function PressureBombBuff:buffEffect(per)
    if self.effected then
        return
    end
    self.effected = true    
    
    local s = self:getSrcUnit()
    if not s then
        return
    end
    
    local o = self:getOwner()
	eff = SEL.createEffectOnUnit(SEL.kBurn, o)
	eff:playRelease()
	
    local ad = AttackData:new()
    ad:setAttack(self.dt, self.dv * per)
	
	x, y = o:getPosition()
	local s = self:getSrcUnit()
	local ug = UnitGroup.getEffectiveUnitsInRange(x, y, s, self.range, UnitForce.kEnemy)
    ug:damaged(ad, s, Ability.kMaskActiveTrigger)
end

function PressureBombBuff:onUnitDelAbility()
    self:buffEffect(0.5)
end

function PressureBombBuff:onUnitDamagedDone(damage, source)
    self.maxDamage = self.maxDamage - damage
    if self.maxDamage <= 0 then
        self:buffEffect(1.0)
        self:setDuration(0.0)
    end
end

DamageIncreaceBuff = class(BuffAbility)
function DamageIncreaceBuff:ctor(root, name, duration, damageType, damageIncreace, exAtkA, exAtkB)
    self:sctor(root, name, duration, false)
    self.dt = damageType
	self.dvi = damageIncreace
    self.damage = 0
	self.exAtkA = exAtkA
	self.exAtkB = exAtkB
end

function DamageIncreaceBuff:onUnitAddAbility()
	local s = self:getSrcUnit()
    if not s then
        return
    end
	
	if self.dvi == 0 then
		local atk = s:getAttackAbility()
		local av = atk:getRealAttackValue()
		av = av * self.exAtkA + self.exAtkB
		self.dvi = av
	end
	
	self.damage = self.dvi
end

function DamageIncreaceBuff:onUnitDisplaceAbility(new)
    local s = self:getSrcUnit()
    if not s then
        return
    end
    
    local o = self:getOwner()
    local ad = AttackData:new()
    ad:setAttack(self.dt, self.damage)
    o:damaged(ad, s, Ability.kMaskActiveTrigger)
    
    self.damage = self.damage + self.dvi
end

StrikeBackPas = class(PassiveAbility)
function StrikeBackPas:ctor(name, range, probability, buff)
	self:sctor("StrikeBackPas", name)
	self:setTriggerFlags(Ability.kOnAttackedTrigger)
	self.range = range
	self.probability = probability
	self.buff = buff
end

function StrikeBackPas:onUnitAttacked(ad, s)
	if math.random() >= self.probability then
		return true
	end
	
	local o = self:getOwner()
	local x, y = o:getPosition()
	local ug = UnitGroup.getEffectiveUnitsInRange(x, y, o, self.range, UnitForce.kEnemy)
	ug:addBuffAbility(self.buff, o)
	
	return true
end

ChangeAttributeBuff = class(BuffAbility)
function ChangeAttributeBuff:ctor(root, name, duration, stackable, exMaxHpDeltaA, exMaxHpDeltaB, exMoveSpeedDeltaA, exMoveSpeedDeltaB, exAttackSpeedDeltaA, exAttackSpeedDeltaB, exAttackValueDeltaA, exAttackValueDeltaB, exArmorValueDeltaA, exArmorValueDeltaB)
    self:sctor(root, name, duration, stackable)
    self.exMaxHpDeltaA = exMaxHpDeltaA
    self.exMaxHpDeltaB = exMaxHpDeltaB
    self.exMoveSpeedDeltaA = exMoveSpeedDeltaA
    self.exMoveSpeedDeltaB = exMoveSpeedDeltaB
    self.exAttackSpeedDeltaA = exAttackSpeedDeltaA
    self.exAttackSpeedDeltaB = exAttackSpeedDeltaB
    self.exAttackValueDeltaA = exAttackValueDeltaA
    self.exAttackValueDeltaB = exAttackValueDeltaB
	self.exArmorValueDeltaA = exArmorValueDeltaA
    self.exArmorValueDeltaB = exArmorValueDeltaB
    self.lastLvl = 0
end

function ChangeAttributeBuff:onUnitAddAbility()
    self:changeValueByDelta(1)
end

function ChangeAttributeBuff:onUnitDelAbility()
    self:changeValueByDelta(-1)
end

function ChangeAttributeBuff:changeValueByDelta(dt)
    local o = self:getOwner()
	local exMaxHpDeltaA = self.exMaxHpDeltaA * dt
    local exMaxHpDeltaB = self.exMaxHpDeltaB * dt
    local exMoveSpeedDeltaA = self.exMoveSpeedDeltaA * dt
    local exMoveSpeedDeltaB = self.exMoveSpeedDeltaB * dt
    local exAttackSpeedDeltaA = self.exAttackSpeedDeltaA * dt
    local exAttackSpeedDeltaB = self.exAttackSpeedDeltaB * dt
    local exAttackValueDeltaA = self.exAttackValueDeltaA * dt
    local exAttackValueDeltaB = self.exAttackValueDeltaB * dt
	local exArmorValueDeltaA = self.exArmorValueDeltaA * dt
    local exArmorValueDeltaB = self.exArmorValueDeltaB * dt
    
    local a, b = o:getExMaxHp()
	o:setExMaxHp(a + exMaxHpDeltaA, b + exMaxHpDeltaB)
	
	a, b = o:getExMoveSpeed()
    o:setExMoveSpeed(a + exMoveSpeedDeltaA, b + exMoveSpeedDeltaB)
    
    local atk = o:getAttackAbility()
    
    a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + exAttackSpeedDeltaA, b + exAttackSpeedDeltaB)
    
    a, b = atk:getExAttackValue()
    atk:setExAttackValue(a + exAttackValueDeltaA, b + exAttackValueDeltaB)
	
	if self:getDuration() <= 0.5 and self.exAttackValueDeltaA * dt > 1 then
		o:addBattleTip(math.ceil(atk:getRealAttackValue()), "fonts/orange.fnt", 32)
	end
	
	a, b = o:getExArmorValue()
	o:setExArmorValue(a + exArmorValueDeltaA, b + exArmorValueDeltaB)
end

RainAct = class(ActiveAbility)
RainAct.INTERVAL = 0.05
function RainAct:ctor(name, cd, eff, intensity, duration, buff, castTarget, effectFlags)
	self:sctor("RainAct", name, cd, castTarget, effectFlags)
	self:setTriggerFlags(Ability.kOnTickTrigger)
	self:setCastRange(500)
	self:setCastTargetRadius(100)
	
	self.eff = eff
	self.x = 0.0
	self.y = 0.0
	self.intensity = intensity
	self.duration = duration
	self.elapsed = 0.0
	self.buff = buff
	self.total = 0
	self.act = 0
end

function RainAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	self.x, self.y = self:getAbilityEffectPoint(proj, target)
	self:setInterval(RainAct.INTERVAL)
	self.elapsed = 0.0
	self.total = 1
	o:stop()
	o:suspend()
	self.act = o:doAnimation(self:getCastRandomAnimation(), INFINITE)
	self:onUnitInterval()
end

function RainAct:onUnitInterval()
	self.elapsed = self.elapsed + RainAct.INTERVAL
	local o = self:getOwner()
	local intensity = self.total / self.elapsed
	while intensity <= self.intensity do
		self.total = self.total + 1
	
		local x, y = getDirectionPoint(self.x, self.y, math.random(2 * math.pi), math.random(self:getCastTargetRadius()))
		eff = SEL.createEffectAtPosition(self.eff, x, y, 0.0)
		eff:playRelease()
		
		local ug = UnitGroup.getEffectiveUnitsInRange(self.x, self.y, o, self:getCastTargetRadius(), self:getEffectiveTypeFlags())
		ug:addBuffAbility(self.buff, o)
		
		intensity = self.total / self.elapsed
	end
	
	if self.elapsed >= self.duration then
		self:setInterval(0.0)
		o:stopAction(self.act)
		o:resume()
		o:stop()
	end
end

ProjectileRainAct = class(ActiveAbility)
ProjectileRainAct.INTERVAL = 0.05
function ProjectileRainAct:ctor(name, cd, proj, intensity, duration, buff, castTarget, effectFlags, sound, dur)
	self:sctor("ProjectileRainAct", name, cd, castTarget, effectFlags)
	self:setTriggerFlags(Ability.kOnTickTrigger)
	self:setCastRange(500)
	self:setCastTargetRadius(150)
	self.proj = proj
	self.x = 0.0
	self.y = 0.0
	self.intensity = intensity
	self.duration = duration
	--self.play = false
	self.elapsed = 0.0
	self.buff = buff
	self.total = 0
	self.act = 0
	
	self.sound = sound
	self.dur = dur
	self.sid = 0
end

function ProjectileRainAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	if not proj then
		self.x, self.y = self:getAbilityEffectPoint(proj, target)
		self:setInterval(ProjectileRainAct.INTERVAL)
		self.elapsed = 0.0
		self.total = 1
		o:stop()
		o:suspend()
		self.act = o:doAnimation(self:getCastRandomAnimation(), INFINITE)
		self:onUnitInterval()
		return
	end
	
	if target then
		target:addBuffAbility(self.buff, o)
	end
end

function ProjectileRainAct:onUnitInterval()
	self.elapsed = self.elapsed + ProjectileRainAct.INTERVAL
	local o = self:getOwner()
	
	local intensity = self.total / self.elapsed
	while intensity <= self.intensity do
		self.total = self.total + 1
		
		local ug = UnitGroup.getEffectiveUnitsInRange(self.x, self.y, o, self:getCastTargetRadius(), self:getEffectiveTypeFlags())

		local t = ug:getRandomUnit()
		if t then
			local p = createProjectile(self.proj)
			p:setPenaltyFlags(Projectile.kOnDying)
			p:setFireType(Projectile.kFireFollow)
			p:setMaxHeightDelta(math.random(-100, 100))
			p:setMoveSpeed(p:getMoveSpeed() * (math.random(1.0, 2.0) - 0.3))
			p:setFromToType(Projectile.kUnitToUnit)
			p:setFromUnit(o)
			p:setToUnit(t)
			p:setSrcUnit(o:getId())
			--p:setContactLeft(1)
			p:setSrcAbility(self)
			p:fire()
			
			if not isSoundPlaying(self.sid) then
				self.sid = playSound(self.sound, self.dur)
			end
		end
		
		intensity = self.total / self.elapsed
	end
	
	if self.elapsed >= self.duration then
		self:setInterval(0.0)
		o:stopAction(self.act)
		o:resume()
		o:stop()
	end
end

ProjectileWindAct = class(ActiveAbility)
ProjectileWindAct.INTERVAL = 0.05
function ProjectileWindAct:ctor(name, cd, proj, intensity, width, speed, duration, buff, effectFlags)
	self:sctor("ProjectileWindAct", name, cd, CommandTarget.kPointTarget, effectFlags)
	self:setTriggerFlags(Ability.kOnTickTrigger)
	self:setCastRange(400)
	
	self.proj = proj
	self.intensity = intensity
	self.width = width
	self.speed = speed
	self.duration = duration
	self.buff = buff
	
	self.x = 0
	self.y = 0
	self.elapsed = 0.0
	self.buff = buff
	self.total = 0
	self.act = 0
	
end
function ProjectileWindAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	if not proj then
		self.x, self.y = self:getAbilityEffectPoint(proj, target)
		self:setInterval(ProjectileWindAct.INTERVAL)
		self.elapsed = 0.0
		self.total = 1
		o:stop()
		o:suspend()
		self.act = o:doAnimation(self:getCastRandomAnimation(), INFINITE)
		self:onUnitInterval()
		return
	end
	
	if target then
		target:addBuffAbility(self.buff, o)
	end
end

function ProjectileWindAct:onUnitInterval()
	self.elapsed = self.elapsed + ProjectileWindAct.INTERVAL
	local o = self:getOwner()
	
	local intensity = self.total / self.elapsed
	while intensity <= self.intensity do
		self.total = self.total + 1
		
		local x, y = o:getPosition()
		local a = math.atan2(self.y - y, self.x - x)
		local dis = self:getCastRange()
		local speed = 400
		local dt = math.random(-self.width * 0.5, self.width * 0.5)
		
		x, y = getDirectionPoint(x, y, a + math.pi / 2, dt)
		local x0, y0 = getDirectionPoint(x, y, a, -dis + math.random(-100, 100))
		local x1, y1 = getDirectionPoint(x0, y0, a, dis * 2 + math.random(-100, 100))
		
		local p = createProjectile(self.proj)
		p:setSrcUnit(o:getId())
		p:setEffectiveTypeFlags(self:getEffectiveTypeFlags())
		p:setContactLeft(1)
		p:setSrcAbility(self)
		p:fireStraight(x0, y0, x1, y1, speed)
		intensity = self.total / self.elapsed
	end
	
	if self.elapsed >= self.duration then
		self:setInterval(0.0)
		o:stopAction(self.act)
		o:resume()
		o:stop()
	end
end

WhenDamagedBuff = class(BuffAbility)
function WhenDamagedBuff:ctor(root, name, duration, stackable, minDamageA, minDamageB, buff)
	self:sctor(root, name, duration, stackable)
	self:setTriggerFlags(Ability.kOnDamagedDoneTrigger)
	self.minDamageA = minDamageA
	self.minDamageB = minDamageB
	self.buff = buff
end

function WhenDamagedBuff:onUnitDamagedDone(damage, source)
	local o = self:getOwner()
	if damage < self.minDamageA * o:getRealMaxHp() + self.minDamageB or not source then
		return
	end
	
	local s = self:getSrcUnit()
	if s then
		o:addBuffAbility(self.buff, s)
	end
end

TeleportAct = class(ActiveAbility)
function TeleportAct:ctor(name, cd, startAct, endAct)
	self:sctor("TeleportAct", name, cd, CommandTarget.kPointTarget, UnitForce.kSelf)
	self:setCastRange(800)
	--self:setCastTargetRadius(0)
	self:addCastAnimation(startAct)
	
	self.endAct = endAct
end

function TeleportAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	local ct = o:getCastTarget()
	
	o:setPosition(ct.x, ct.y)
	o:doAnimation(self.endAct, 1)
end

SerialExplodeAct = class(ActiveAbility)
function SerialExplodeAct:ctor(name, cd, dis, count, interval, range, buff)
	self:sctor("SerialExplodeAct", name, cd, CommandTarget.kPointTarget, UnitForce.kEnemy)
	self:setTriggerFlags(Ability.kOnTickTrigger)
	self:setCastRange(dis * count)
	self.dis = dis
	self.count = count
	self.index = 0
	self.interval = interval
	
	self.range = range
	self.buff = buff
	
	self.sx = 0
	self.sy = 0
	self.angle = 0
end

function SerialExplodeAct:onUnitAbilityEffect(proj, target)
	if not proj then
		local o = self:getOwner()
		self.sx, self.sy = o:getPosition()
		local tx, ty = self:getAbilityEffectPoint(proj, target)
		self.angle = math.atan2(ty - self.sy, tx - self.sx)
		
		self.index = 0
		self:setInterval(self.interval)
		self:onUnitInterval()
		return
	end
end

function SerialExplodeAct:onUnitInterval()
	self.index = self.index + 1
	
	local x, y = getDirectionPoint(self.sx, self.sy, self.angle, self.dis * self.index)
	eff = SEL.createEffectAtPosition(SEL.kExplosionNormal, x, y, 0.0)
	eff:playRelease()
	
	local o = self:getOwner()
	local ug = UnitGroup.getEffectiveUnitsInRange(x, y, o, self.range, self:getEffectiveTypeFlags())
	
	for _, u in ipairs(ug.units) do
		local buff = KnockBackBuff:new("KnockBack", 0.5, 100, x, y)
		buff:setAppendBuff(self.buff)
		buff:setSrcUnit(o)
		buff:setLevel(self:getLevel())
		u:addBuffAbility(buff)
	end
	
	if self.index >= self.count then
		self:setInterval(0.0)
	end
end

ChangeAttackBuff = class(BuffAbility)
function ChangeAttackBuff:ctor(name, duration, atkType, interval, minRange, maxRange, horizontal, proj)
	self:sctor("ChangeAttributeBuff", name, duration, false)
	self.at = atkType
	self.interval = interval  -- 基础攻击间隔不会随等级提升而改变，所以可以更改
	self.mr = minRange
	self.r = maxRange
	self.hor = horizontal
	self.proj = proj
end

function ChangeAttackBuff:onUnitAddAbility()
	self:swapAttackAttribute()
end

function ChangeAttackBuff:onUnitDelAbility()
	self:swapAttackAttribute()
end

function ChangeAttackBuff:swapAttackAttribute()
	local o = self:getOwner()
	local atk = o:getAttackAbility()
	if not atk then
		return
	end
	
	local tmp, tmp2
	
	if self.at ~= nil then
		tmp, tmp2 = atk:getBaseAttack()
		atk:setBaseAttack(self.at, tmp2)
		self.at = tmp
	end
	
	if self.interval ~= nil then
		tmp = atk:getBaseAttackInterval()
		atk:setBaseAttackInterval(self.interval)
		self.interval = tmp
	end
	
	if self.mr ~= nil then
		tmp = atk:getCastMinRange()
		atk:setCastMinRange(self.mr)
		self.mr = tmp
	end
	
	if self.r ~= nil then
		tmp = atk:getCastRange()
		atk:setCastRange(self.r)
		self.r = tmp
	end
	
	if self.hor ~= nil then
		tmp = atk:isCastHorizontal()
		atk:setCastHorizontal(self.hor)
		self.hor = tmp
	end
	
	if self.proj ~= nil then
		tmp = atk:getTemplateProjectile()
		atk:setTemplateProjectile(self.proj)
		self.proj = tmp
	end
	
end

EffectBuff = class(BuffAbility)
function EffectBuff:ctor(eff, attached)
	self:sctor("EffectBuff", "", 0.0, true)
	self.eff = eff
	self.attached = attached
end

function EffectBuff:onUnitAddAbility()
	local o = self:getOwner()
	local eff
	if self.attached then
		eff = SEL.createEffectOnUnit(self.eff, o)
	else
		local x, y = o:getPosition()
		eff = SEL.createEffectAtPosition(self.eff, x, y, 0.0)
	end
	
	eff:playRelease()
end

BackStabBuff = class(BuffAbility)
function BackStabBuff:ctor(root, name, duration, stackable, buffForSelf, buffForTarget)
    self:sctor(root, name, duration, stackable)
    self:setTriggerFlags(Ability.kOnAttackTargetTrigger)
    self.buffForSelf = buffForSelf
	self.buffForTarget = buffForTarget
end

function BackStabBuff:onUnitAttackTarget(ad, t)
    local o = self:getOwner()
    if not t then
		return
	end
	local x, y = o:getPosition()
	local tx, ty = t:getPosition()
	local back = (x > tx) == (t:isFlippedX()) and not t:isFixed()
	if not back then
		return
	end
	
	if self.buffForSelf > 0 then
		o:addBuffAbility(self.buffForSelf, o, self:getLevel())
	end
	
	if self.buffForTarget > 0 then
		ad:addAttackBuff(self.buffForTarget, self:getLevel())
	end
end
