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

CurseBuff = class(BuffAbility)
function CurseBuff:ctor(duration, stackable, damage, interval, multiply)
    self:sctor("curse", "Curse", duration, stackable)
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
    o:addBattleTip("Curse", "Arial", 18, 255, 0, 255)
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
    o:addBattleTip(string.format("-%d", damage), "Arial", 18, 255, 0, 0)
    o:setHp(o:getHp() - damage)
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
    --log("lua ad: %s", ad._p)
    --ad:setAttackValue(ad:getAttackValue() * 10)
    ad:addAttackBuff(self.buff, self:getLevel())
    --o:addBattleTip(math.ceil(ad:getAttackValue()), "Arial", 18, 0, 0, 0)
    return ad
end

DamageBackPas = class(PassiveAbility)
function DamageBackPas:ctor(per, buff)
    self:sctor("DamageBack", "DamageBack", 0)
    self:setTriggerFlags(Ability.kOnDamagedSurfaceTrigger)
    self.per = per
    self.buff = buff
end

function DamageBackPas:onUnitDamaged(ad, src)
    local o = self:getOwner()
    local nad = AttackData:new()
    nad:addAttackBuff(self.buff, self:getLevel())
    local t, v = ad:getAttack()
    nad:setAttack(t, self.per * v)
    src:damaged(nad, o)
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
    
    o:addBattleTip(math.ceil(av), "Arial", 18, 0, 0, 0)

end

function ArmorBuff:onUnitDelAbility()
    o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    o:setExArmorValue(exA, exB - self.change)
end 

function onWorldInit()
    do return end
    math.randomseed(os.time())

    c = getControlUnit()
    local x, y = c:getPosition()
    c:setPosition(x + 200, y)
    local x, y = c:getPosition()
    c:setMaxHp(1000)

    for i = 1, 3 do
        if i == 3 then
            u = createUnit(0x100 + 3)
        else
            u = createUnit(0x100 + math.random(0, 11))
        end

        u:setForceByIndex(3)
        u:setMaxHp(200.0)
        u:setBaseArmor(0, 29.0)
        u:setPosition(500 + math.random(-200, 200), 500 + math.random(-200, 200))

        setControlUnit(u)

        --do return end
    
        --u:setExMoveSpeed(2, 0)
    
        a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -10.0)
        id = addTemplateAbility(a)
        a = DefPas:new(id)
        u:addPassiveAbility(a) 

        a = ChangeHpBuff:new("+HP", "Heal", 5, true, 0.2, 0.002, 0.0, 0.0, 0.0)
        id = addTemplateAbility(a)
        a = EvadePas:new("Evade", 0.30, id)
        u:addPassiveAbility(a)

        a = ChangeHpPas:new("Heal", 0.2, 0.002, 0.0, 0.0, 0.0)
        u:addPassiveAbility(a)

        a = VampirePas:new("Vampire", 0.5)
        u:addPassiveAbility(a)

        a = SpeedBuff:new("SpeedUp", "SpeedUp", 5, true, 2.0, 0.0, 2.0, 0.0)
        id = addTemplateAbility(a)
        a = AttackBuffMakerPas:new("Critical", 0.1, id, true, 10.0, 0.0)
        u:addPassiveAbility(a)
    
        a = CurseBuff:new(13, true, 1, 4, 20 / 100)
        id = addTemplateAbility(a)
        a = DamageBackPas:new(1.0, id)
        u:addPassiveAbility(a)
    
    end
    
end

function onWorldTick(dt)
end
