--require "/sdcard/ts/gamedemo/common.lua"
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
    o:addBattleTip("Curse", "fonts/Arial.ttf", 18, 255, 0, 255)
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
    o:addBattleTip(string.format("-%d", damage), "fonts/Arial.ttf", 18, 255, 0, 0)
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
    --log(string.format("%s attack %s", o:getName(), target:getName()))
    --ad:setAttackValue(ad:getAttackValue() * 10)
    ad:addAttackBuff(self.buff, self:getLevel())
    --o:addBattleTip(math.ceil(ad:getAttackValue()), "fonts/Arial.ttf", 18, 0, 0, 0)
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
    if self.buff ~= 0 then
        nad:addAttackBuff(self.buff, self:getLevel())
    end
    local t, v = ad:getAttack()
    nad:setAttack(t, self.per * v)
    if not o then
        log("o is nil")
        return
    else
        log(string.format("%s %s", nad, o))
        src:damaged(nad, o, 448)
    end
    
    --
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
    
    o:addBattleTip(math.ceil(av), "fonts/Arial.ttf", 18, 0, 0, 0)

end

function ArmorBuff:onUnitDelAbility()
    o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    o:setExArmorValue(exA, exB - self.change)
end 

function onWorldInit()
    showDebug(false)
    math.randomseed(os.time())
    
    c = getControlUnit()
    local x, y = c:getPosition()
    c:setPosition(x + 200, y)
    local x, y = c:getPosition()
    c:setMaxHp(687)

    game01()
    do return true end
    for i = 0, 0 do
        if i == 12 then
            u = createUnit(0x100 + 3)
        else
            u = createUnit(0x100 + i)
        end
        
        u:setForceByIndex(3)
        u:setMaxHp(1000.0)
        u:setBaseArmor(0, 20.0)
        u:setPosition(500 + math.random(-200, 200), 500 + math.random(-200, 200))

        a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -10.0)
        id = addTemplateAbility(a)
        a = DefPas:new(id)
        u:addPassiveAbility(a) 

        

        a = ChangeHpPas:new("Heal", 0.2, 0.002, 0.0, 0.0, 0.0)
        u:addPassiveAbility(a)

        --a = CurseBuff:new(13, false, 1, 4, 20 / 100)
        --id = addTemplateAbility(a)
        a = DamageBackPas:new(0.2, 0)
        u:addPassiveAbility(a)
    
    end
    setControlUnit(u)
    u:setMaxHp(200)
    u:setBaseArmor(0, 20.0)
    u:setForceByIndex(5)
    u:setPosition(1500, 800)
    
    a = SpeedBuff:new("SpeedUp", "SpeedUp", 150, true, 0.5, 0.0, 0.5, 0.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.1, id, true, 10.0, 0.0)
    u:addPassiveAbility(a)
    
    a = VampirePas:new("Vampire", 0.5)
    u:addPassiveAbility(a)
    
    a = ChangeHpBuff:new("+HP", "Heal", 5, true, 0.2, 0.002, 0.0, 0.0, 0.0)
    id = addTemplateAbility(a)
    a = EvadePas:new("Evade", 0.30, id)
    u:addPassiveAbility(a)
    
    a = RebirthPas:new("re", 1,1)
    u:addPassiveAbility(a)
    
    a = ChangeHpPas:new("Heal", 0.2, 0.01, 0.0, 0.0, 0.0)
    u:addPassiveAbility(a)
    
    up = UnitPath:new()
    up:addPoint(2000, 1020)
    up:addPoint(1900, 900)
    up:addPoint(2000, 1500)
    --u:moveAlongPath(up)
    return true
end

function spawnSoldier(id, force)
    u = createUnit(0xff + id)
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
    
    u:moveAlongPath(up, false)
    
    return u
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
function onWorldTick(dt)
    game01_tick(dt)
end

function game01()
    a = ChangeHpBuff:new("heal", "heal", 5, false, 0.3, 0.006, 0, 0, -1)
    id = addTemplateAbility(a)
    a = AuraPas:new("HealAura", 1, id, 200, 5)
    id = addTemplateAbility(a)
    
    u = createUnit(0xff + 26)
    u:setForceByIndex(1)
    u:setPosition(100, 500)
    u:addPassiveAbility(id)
    
    u = createUnit(0xff + 27)
    u:setForceByIndex(2)
    u:setPosition(1900, 500)
    u:addPassiveAbility(id)
    
end

function game01_tick(dt)
    el = el + dt
    if uc < 5 then
        if el > 2 then
            uc = uc + 1
            el = el - 2
            if not isEffectPlaying("sounds/Effect/OpenDoor.mp3") then
                playEffect("sounds/Effect/OpenDoor.mp3")
            end
            if uc == 4 then
                u = spawnSoldier(2, 1)
                u:addPassiveAbility(id)
                spawnSoldier(4, 2)
            else
                spawnSoldier(22, 1)
                spawnSoldier(23, 2)
            end
        end
    else
        if 	el > WAVE then
            -- wave in coming
            playEffect("sounds/Effect/WaveIncoming.mp3")
            uc = 0
            el = el - WAVE
        end
    end
end

