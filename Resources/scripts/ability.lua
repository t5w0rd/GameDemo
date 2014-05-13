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
    atk = me:getAttackAbility()
    atk:setExAttackValue(1.0 + kill / 20.0, kill * 3.9 + 17)
    a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + kill / 50, b)
    local level = math.ceil(kill / 2)
    if level >= 1 and level <= c then
        if level > learned then
            me:addPassiveAbility(aaa[level])
            learned = level
            me:addBattleTip(string.format("Learn New Ability (%d/%d)", learned, c), "fonts/Comic Book.fnt", 32, 235, 170, 68)
        end
    end

end

loadAnimation("Sprites/VoodooProy/move", "Sprites/VoodooProy/move", 0.1)
loadAnimation("Sprites/VoodooProy/die", "Sprites/VoodooProy/die", 0.1)
local sp = Sprite:new("VoodooProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
local u = Unit:new(sp, "Orb")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
addTemplateUnit(10001, u)

loadAnimation("Sprites/AlienProy/move", "Sprites/AlienProy/move", 0.1)
loadAnimation("Sprites/AlienProy/die", "Sprites/AlienProy/die", 0.1)
local sp = Sprite:new("AlienProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
local u = Unit:new(sp, "Orb")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
addTemplateUnit(10002, u)

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
