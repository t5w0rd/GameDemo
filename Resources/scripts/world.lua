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

hero = nil
me = nil

aaa = {}
c = 0

kill = 0
learned = 0

taid2 = 0

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
    --o:addBattleTip(string.format("-%d", damage), "fonts/Arial.ttf", 18, 255, 0, 0)
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
        --log("o is nil")
        return
    else
        --log(string.format("%s %s", nad, o))
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
    o:addBattleTip(math.ceil(o:getRealArmorValue() - av), "fonts/Arial.ttf", 18, 0, 0, 0)
end

function ArmorBuff:onUnitDelAbility()
    o = self:getOwner()
    local exA, exB = o:getExArmorValue()
    o:setExArmorValue(exA, exB - self.change)
end

OnDyingPas = class(PassiveAbility)
function OnDyingPas:ctor()
    self:sctor("OnDying", "OnDying", 0)
    self:setTriggerFlags(Ability.kOnDyingTrigger)
end

function OnDyingPas:onUnitDying()
    hero = nil
    if me:isDead() then
        return
    end
    
    kill = kill + 1
    atk = me:getAttackAbility()
    atk:setExAttackValue(1.0 + kill / 3.0, kill * 5.9 + 17)
    a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + kill / 10, b)
    local level = math.ceil(kill / 2)
    if level >= 1 and level <= c then
        if level > learned then
            me:addPassiveAbility(aaa[level])
            learned = level
            me:addBattleTip(string.format("Learn New Ability (%d/%d)", learned, c), "fonts/Arial.ttf", 32, 235, 170, 68)
        end
    end

end

function onWorldInit()
    showDebug(false)
    math.randomseed(os.time())

    me = getControlUnit()

    local x, y = me:getPosition()
    me:setPosition(x + 500, y - 200)
    me:setMaxHp(687)

    game01()
    
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

function spawnHero()
    hero = spawnSoldier(math.random(1, 12), 1)
    hero:addPassiveAbility(OnDyingPas:new())
    hero:setMaxHp(me:getMaxHp() * 1.0 + (kill / 1.5) * 100)
    hero:setRewardGold(50 + kill * 20 / 1.5)
    hero:setRewardExp(20 + kill * 10 / 1.5)
    atk = hero:getAttackAbility()
    t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, v * (1 + kill / 1.5))
    atk:setExAttackSpeed(1.0 + kill / 15, 0.0);
    for i = 1, (3 + kill) do
        hero:addPassiveAbility(aaa[math.random(1, c)])
    end
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

function initAAA()

    a = DoubleAttackPas:new("DA", 0.2, 0.5, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    a = ChangeHpPas:new("Heal", 0.2, 0.0, 0.5, 0.0, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = SplashPas:new("SplashBolt", 75, 1.0, 0.0, 150, 0.5, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    --a = BetrayBuff:new(5)
    --id = addTemplateAbility(a)
    --a = AttackBuffMakerPas:new("Betray", 1.0, id, false, 1.0, 0.0)
    --c = c + 1
    --aaa[c] = addTemplateAbility(a)

    a = SpeedBuff:new("SlowDown", "SlowDown", 3, true, -0.1, 0.0, -0.1, 0.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("SpeedAttack", 1.0, id, false, 1.0, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    a = KnockBackBuff:new("KnockBack", "KnockBack", 0.5, true, 30)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("KnockBackAttack", 0.2, id, false, 1.5, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = StunBuff:new("Stun", "Stun", 1, false)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("StunAttack", 0.15, id, false, 1.0, 50.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -5.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.5, id, false, 1.0, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = CurseBuff:new(13, false, 15, 4, 30 / 100)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.2, id, false, 1.0, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    a = ChangeHpBuff:new("+HP", "Heal", 3, true, 0.2, 0.0, 1, 0.0, 0.0)
    id = addTemplateAbility(a)
    a = EvadePas:new("Evade", 0.20, id)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = VampirePas:new("Vampire", 0.5)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    a = SpeedBuff:new("SpeedUp", "SpeedUp", 10, true, 0.05, 0.0, 0.1, 0.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.5, id, true, 1.5, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    taid2 = aaa[c]
end

function game01()
    a = ChangeHpBuff:new("heal", "heal", 5, false, 0.3, 0.006, 0, 0, -1)
    id = addTemplateAbility(a)
    a = AuraPas:new("HealAura", 1, id, 200, 5)
    taid = addTemplateAbility(a)

    initAAA()
    u = createUnit(0xff + 26)
    u:setMaxHp(u:getMaxHp() * 7)
    u:setBaseArmor(2, 52)
    u:setForceByIndex(1)
    u:setPosition(100, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    u:addPassiveAbility(DamageBackPas:new(1.2, 0))
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 0, 350, 0x100 + 16)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0)
    u:addPassiveAbility(a)

    u = createUnit(0xff + 27)
    u:setMaxHp(u:getMaxHp() * 7)
    u:setBaseArmor(2, 52)
    u:setForceByIndex(2)
    u:setPosition(1900, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    u:addPassiveAbility(DamageBackPas:new(1.0, 0))
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 0, 350, 0x100 + 18)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0)
    u:addPassiveAbility(a)
    
end

function game01_tick(dt)
    a = ChangeHpBuff:new("MageHeal", "MageHeal", 5, false, 0.3, 0.006, 0, 0, -1)
    arid = addTemplateAbility(a)
    a = AuraPas:new("HealAura", 1, arid, 220, 4)
    arid = addTemplateAbility(a)
    
    el = el + dt
    if uc < 5 then
        if el > 2 then
            uc = uc + 1
            el = el - 2
            playEffect("sounds/Effect/OpenDoor.mp3")
            if uc == 4 then
                u = spawnSoldier(2, 1)
                u:addPassiveAbility(arid)
                u:addPassiveAbility(SplashPas:new("SplashBolt", 75, 1.0, 0.0, 150, 0.5, 0.0))
                u:setRewardGold(20)
                u:setRewardExp(14)
                u = spawnSoldier(4, 2)
                u:setRewardGold(20)
                u:setRewardExp(14)
            else
                spawnSoldier(22, 1)
                spawnSoldier(23, 2)
            end
            if not hero then
                spawnHero()
            end
        end
    else
        if el > WAVE then
            -- wave in coming
            playEffect("sounds/Effect/WaveIncoming.mp3")
            uc = 0
            el = el - WAVE
        end
    end
end

