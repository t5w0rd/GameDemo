if __ABILITY__ then return end
__ABILITY__ = true

include("UnitLibrary.lua")

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
    self:sctor("DamageBack", "DamageBack", 0)
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
    o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    local av = o:getRealArmorValue()
    --local s = string.format()
    self.change = self.exA * av + self.exB
    o:setExArmorValue(exA, exB + self.change)
    o:addBattleTip(math.ceil(o:getRealArmorValue() - av), "fonts/Comic Book.fnt", 18, 0, 0, 0)
	
	if self.change < 0 then
		local eff = Effect:new("Effects/ArmorBreak", 0.08, 0.5, 0.5, o)
		local x, y = o:getAnchorPointInPoints()
		y = y + o:getHalfOfHeight()
		eff:setPosition(x, y)
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

    me:say(OnDyingPas.words[math.random(1, #OnDyingPas.words)])

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
    u:setGhost()
    u:doAnimation(Sprite.kAniMove, INFINITE)

    u:addBuffAbility(LimitedLifeBuff:new("LimitedLife", "LimitedLife", self.duration))

    a = AttractBuff:new("Attract", "Attract", 1, 10.0)
    id = addTemplateAbility(a)
    a = AuraPas:new("AttractAura", 0.5, id, self:getCastTargetRadius(), UnitForce.kEnemy, false)
    u:addPassiveAbility(a)

    a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.25, 0.0)
    id = addTemplateAbility(a)
    a = AuraPas:new("DamageAura", 1.0, id, self:getCastTargetRadius(), UnitForce.kEnemy, false)
    u:addPassiveAbility(a)
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
function PressureBombBuff:ctor(name, duration, maxDamage, damageType, damageValue)
    self:sctor("PressureBombBuff", name, duration, true)
    self:setTriggerFlags(Ability.kOnDamagedDoneTrigger)    
    self.maxDamage = maxDamage
    self.dt = damageType
    self.dv = damageValue
    self.effected = false
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
    local ad = AttackData:new()
    ad:setAttack(self.dt, self.dv * per)
    o:damaged(ad, s, Ability.kMaskActiveTrigger)
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
function DamageIncreaceBuff:ctor(root, name, duration, damageType, damageIncreace)
    self:sctor(root, name, duration, false)
    self.dt = damageType
    self.dv = damageIncreace
    self.damage = damageIncreace
end

function DamageIncreaceBuff:onUnitDisplaceAbility(new)
    local s = self:getSrcUnit()
    if not s then
        return
    end
    
    local o = self:getOwner()
    local ad = AttackData:new()
    ad:setAttack(self.dt, self.damage)
    o:damaged(ad, s)--, Ability.kMaskActiveTrigger)
    
    self.damage = self.damage + self.dv
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
	local targets = getUnits(function (u, p)
		return p:isMyEnemy(u) and p:getTouchDistance(u) < self.range
	end, o)
	
	for _, u in ipairs(targets) do
		u:addBuffAbility(self.buff, o)
	end
	
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
		o:addBattleTip(math.ceil(atk:getRealAttackValue()), nil, 32, 250, 0, 0)
	end
	
	a, b = o:getExArmorValue()
	o:setExArmorValue(a + exArmorValueDeltaA, b + exArmorValueDeltaB)
end

RainAct = class(ActiveAbility)
RainAct.INTERVAL = 0.1
RainAct.kBolt = 0
RainAct.kIce = 1
RainAct.kSharpIce = 2
function RainAct:ctor(name, cd, effect, intensity, duration, buff, castTarget, effectFlags)
	self:sctor("RainAct", name, cd, castTarget, effectFlags)
	self:setCastRange(500)
	self:setCastTargetRadius(100)
	
	self.effect = effect
	self.x = 0.0
	self.y = 0.0
	self.intensity = intensity
	self.duration = duration
	self.play = false
	self.elapsed = 0.0
	self.buff = buff
	
	self:setInterval(RainAct.INTERVAL)
end

function RainAct:onUnitAbilityEffect(proj, target)
	self.x, self.y = self:getAbilityEffectPoint(proj, target)
	self:setInterval(RainAct.INTERVAL)
	self.play = true
	self.elapsed = 0.0
	self.total = 1
end

function RainAct:spawnEffect(index)
	local eff
	if index == 0 then
		eff = Effect:new("Effects/Rain", 0.03, 0.5, 25 / 320)
	elseif index == 1 then
		eff = Effect:new("Effects/Rain2", 0.05, 0.5, 40 / 208)
	elseif index == 2 then
		eff = Effect:new("Effects/Rain3", 0.05, 0.5, 40 / 172)
	end
	
	return eff
end

function RainAct:onUnitInterval()
	if not self.play then
		return
	end
	
	self.elapsed = self.elapsed + RainAct.INTERVAL
	
	local intensity = self.total / self.elapsed
	if intensity <= self.intensity then
		self.total = self.total + 1
	
		local eff = self:spawnEffect(self.effect)
		local x, y = getDirectionPoint(self.x, self.y, math.random(2 * math.pi), math.random(self:getCastTargetRadius()))
		eff:setLogicPosition(x, y)
		eff:playRelease()
		
		local o = self:getOwner()
		local targets = getUnits(function (u, p)
			return p:canEffect(u, self:getEffectiveTypeFlags()) and u:getTouchDistance(self.x, self.y) < self:getCastTargetRadius()
		end, o)
		
		for _, u in ipairs(targets) do
			u:addBuffAbility(self.buff, o)
		end
		
	end
	
	if self.elapsed >= self.duration then
		self:setInterval(0.0)
		self.play = false
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
TeleportAct.INTERVAL = 0.001
function TeleportAct:ctor(name, cd, startAct, endAct)
	self:sctor("TeleportAct", name, cd, CommandTarget.kPointTarget, UnitForce.kSelf)
	self:setCastRange(800)
	--self:setCastTargetRadius(0)
	self:addCastAnimation(startAct)
	self:setInterval(TeleportAct.INTERVAL)
	
	self.endAct = endAct
end

function TeleportAct:onUnitAbilityEffect(proj, target)
	local o = self:getOwner()
	local ct = o:getCastTarget()
	
	o:setPosition(ct.x, ct.y)
	o:doAnimation(self.endAct, 1)
end
