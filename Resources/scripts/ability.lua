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

function CurseBuff:onUnitAddAbility()
    o = self:getOwner()
    self.hp = o:getHp()
    o:addBattleTip("Curse", "fonts/Comic Book.fnt", 18, 255, 0, 255)
end

function CurseBuff:onUnitInterval()
    o = self:getOwner()
    local damage = 0
    self.pass = self.pass + 0.5
    if self.pass > self.interval then
        self.pass = 0
        damage = (self.hp - o:getHp()) * self.multiply
    end

    if damage < 0 then
        damage = 0
    end
    damage = damage + self.damage
    --o:addBattleTip(string.format("-%d", damage), "fonts/Comic Book.fnt", 18, 255, 0, 0)
    ad = AttackData:new()
    ad:setAttack(2, damage)
    s = self:getSrcUnit()
    if s then
        o:damaged(ad, s, Ability.kMaskAll)
    end
    
end

DefPas = class(PassiveAbility)
function DefPas:ctor(buff)
    self:sctor("root", "name", 0)
    self.buff = buff
    self:setTriggerFlags(Ability.kOnAttackTargetTrigger)
end

function DefPas:onUnitAddAbility()
    print("DefPas:onUnitAddAbility()")
end

function DefPas:onUnitAttackTarget(ad, target)
    o = self:getOwner()
    if target:getId() ~= me:getId() and target:isFixed() == false then
        target:setForceByIndex(1)
    end
    --log(string.format("%s attack %s", o:getName(), target:getName()))
    --ad:setAttackValue(ad:getAttackValue() * 10)
    --ad:addAttackBuff(self.buff, self:getLevel())
    --o:addBattleTip(math.ceil(ad:getAttackValue()), "fonts/Comic Book.fnt", 18, 0, 0, 0)
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

    a = DamageBuff:new("dmg", AttackValue.kMagical, 15.0, 1.0, false, 0.0, 0.0)
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


