include("Init.lua")
include("AI.lua")
include("AI2.lua")


hero = nil
me = nil

aaa = {}
c = 0

kill = 0
learned = 0

lvl = 1

taid2 = 0

lib = createAbilityLibrary00()

HeroLevelUpdate = class(LevelUpdate)
function HeroLevelUpdate:onChangeLevel(u, change)
    cast(u, Unit)

    u:addBuffAbility(ChangeHpBuff:new("LevelUpHeal", "LevelUpHeal", 5.0, false, 0.2, 0.02, 0.0))
    u:addBuffAbility(ReflectBuff:new("Reflect", "Reflect", 5.0))
    
    u:setBaseMoveSpeed(u:getBaseMoveSpeed() + 1.5)

    local atk = u:getAttackAbility()
    local a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + 0.01, b)

    local t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, v + 7.8)

    t, v = u:getBaseArmor()
    u:setBaseArmor(t, v + 1)

    saveUserData()
end

function HeroLevelUpdate:calcExp(lvl)
    if lvl == 0 then
        return 0
    end

    return lvl * lvl * 9 + lvl * 3 + 8;
end

function onWorldInit()
    showDebug(false)
    math.randomseed(os.time())

    me = getControlUnit()
    --me:setAI(LuaAI:new())

    me:setLevelUpdate(HeroLevelUpdate:new())

    local x, y = me:getPosition()
    me:setPosition(x + 500, y - 200)
    me:setExMaxHp(1.75, 0.0)
    
    me:setAlly(2 ^ 3 + 2 ^ 4 + 2 ^ 2)
    
    game01()
    --test()
    
    return true
end

function onWorldTick(dt)
    game01_tick(dt)
    --test_tick(dt)
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
    
    u:setExArmorValue(1.0, 2)
    u:moveAlongPath(up, false)

    return u
end

function spawnHero(id)
    if not id then
        id = math.random(1, 12)
    end
    hero = spawnSoldier(id, 1)
    hero:addPassiveAbility(OnDyingPas:new())
    hero:setMaxHp(400 + (kill / 2.5) * 100)
    hero:setRewardGold(50 + kill * 20 / 1.5)
    hero:setRewardExp(20 + kill * 10 / 1.5)
    hero:setExArmorValue(1.0, kill * 0.7)
    atk = hero:getAttackAbility()
    t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, v * (1 + kill / 3.0))
    atk:setExAttackSpeed(1.0 + kill / 20, 0.0)
    for i = 1, (3 + kill / 2) do
        hero:addPassiveAbility(aaa[math.random(1, c)])
    end

    if atk:getCastRange() > 100 then
        hero:addActiveAbility(lib.Reflect)
        hero:addActiveAbility(lib.SpeedUp2)
        hero:addActiveAbility(lib.KnockBack)
        hero:setAI(LuaAI:new())
    else
        hero:addActiveAbility(lib.ThrowHammer)
        hero:addActiveAbility(lib.ThunderCap)
        hero:addActiveAbility(lib.SpeedUp)
        hero:setAI(LuaAIWarrior:new())
    end
    
    return hero
end

function initAAA()
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
    --a = AttackBuffMakerPas:new("Betray", 1.0, id, false, 1.0, 0.0)
    --c = c + 1
    --aaa[c] = addTemplateAbility(a)

    a = SpeedBuff:new("SlowDown", "SlowDown", 3, true, -0.1, 0.0, -0.1, 0.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("SpeedAttack", 1.0, id, false, 1.0, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

    a = KnockBackBuff:new("KnockBack", "KnockBack", 0.5, 30)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("KnockBackAttack", 0.2, id, false, 1.5, 0.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = StunBuff:new("Stun", "Stun", 1, false)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("StunAttack", 0.15, id, false, 1.0, 50.0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -1.0)
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
function game01()
    a = ChangeHpBuff:new("TowerHeal", "TowerHeal", 5, false, 0.3, 0.001, 5, 0, -1)
    id = addTemplateAbility(a)
    a = AuraPas:new("HealAura", 1, id, 200, UnitForce.kAlly, false)
    taid = addTemplateAbility(a)

    initAIAbility()
    initAAA()
    
    u = createUnit(UL.kArcane)
    u:setMaxHp(u:getMaxHp() * 3)
    u:setBaseArmor(ArmorValue.kWall, 52)
    u:setForceByIndex(1)
    u:setPosition(100, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    --u:addPassiveAbility(DamageBackPas:new(1.2, 0))
    a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 0.0, 0.0)
    id = addTemplateAbility(a)
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, PL.kArcaneRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0)
    u:addPassiveAbility(a)
    tower1 = u:getId()

    u = createUnit(UL.kTesla)
    u:setMaxHp(u:getMaxHp() * 3)
    u:setBaseArmor(ArmorValue.kWall, 52)
    u:setForceByIndex(2)
    u:setPosition(1900, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    --u:addPassiveAbility(DamageBackPas:new(1.0, 0))
    a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 0.0, 0.0)
    id = addTemplateAbility(a)
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, PL.kTeslaRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0)
    u:addPassiveAbility(a)
    tower2 = u:getId()
    
end

function game01_tick(dt)
    if not getUnit(tower1) then
        endWithVictory(math.random(1, 3))
    end
    
    if not getUnit(tower2) then
        endWithDefeat()
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
                u = spawnSoldier(2, 1)
                u:addPassiveAbility(arid)
                u:addPassiveAbility(SplashPas:new("SplashBolt", 50, 0.75, 0.0, 75, 0.25, 0.0))
                u:setRewardGold(20)
                u:setRewardExp(14)
                u:setMaxHp(200 + lvl * 46)
                atk = u:getAttackAbility()
                atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.2)
                
                u = spawnSoldier(4, 2)
                u:setRewardGold(20)
                u:setRewardExp(14)
                u:setMaxHp(200 + lvl * 46)
                atk = u:getAttackAbility()
                atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.2)
            else
                u = spawnSoldier(22, 1)
                u:setMaxHp(200 + lvl * 63)
                atk = u:getAttackAbility()
                atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.3)
                
                u = spawnSoldier(23, 2)
                u:setMaxHp(200 + lvl * 63)
                atk = u:getAttackAbility()
                atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.3)
            end
            if not hero then
                local u = spawnHero()
                a = u:getAttackAbility()
                if a:getCastRange() < 150 and math.random(0, 100) < 50 then
                    spawnHero(4)
                    hero:addActiveAbility(lib.Cutter)
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

function initAIAbility()
    --a = me:getActiveAbility("WarCry")
    --me:delActiveAbility(a)

    --a = me:getActiveAbility("ThunderCap")
    --me:delActiveAbility(a)
	
	me:addActiveAbility(lib.KnockBackEx)
	me:addActiveAbility(lib.ThrowHammerEx)

    
end

function game02()

    initAIAbility()
    
    -- create hero3
    u = createUnit(UL.kBarracks)
    u:setPosition(700, 500)
    u:setForceByIndex(3)
    u:setAlly(2 ^ 3 + 2 ^ 2)
    u:setHostilityRange(10000)
    u:setMaxHp(300)
    u:setBaseArmor(ArmorValue.kHeavy, 3.0)
    u:setBaseMoveSpeed(80)
    atk = u:getAttackAbility()
    atk:setBaseAttack(AttackValue.kPhysical, 15)
    atk:setBaseAttackInterval(1.5)
    u:addActiveAbility(lib.ThrowHammer)
    u:addActiveAbility(lib.ThunderCap)
    u:addActiveAbility(lib.SpeedUp)
	u:addPassiveAbility(lib.BerserkerBlood)
	u:addPassiveAbility(lib.StunAttack)
    
    u:setAI(LuaAIWarrior:new())
    
    -- create hero4
    u = createUnit(UL.kArcher)
    u:setPosition(1300, 500)
    u:setForceByIndex(4)
    u:setAlly(2 ^ 4 + 2 ^ 2)
    u:setHostilityRange(10000)
    u:setMaxHp(240)
    u:setBaseArmor(ArmorValue.kHeavy, 2.0)
    u:setBaseMoveSpeed(85)
    atk = u:getAttackAbility()
    atk:setBaseAttack(AttackValue.kPhysical, 18)
    atk:setBaseAttackInterval(1.2)
    --u:addActiveAbility(lib.Reflect)
    u:addActiveAbility(lib.Cutter)
    u:addActiveAbility(lib.SpeedUp2)
    u:addActiveAbility(lib.KnockBack)
	u:addPassiveAbility(lib.VampireAttack)
	u:addPassiveAbility(lib.ArmorBreakAttack)
	u:addPassiveAbility(lib.CriticalAttack)
    
    u:setAI(LuaAI:new())
    
end

function game02_tick(dt)
end

function fireStraight(x, y, x1, y1, speed)
    local p = createProjectile(PL.kVoodooProy)
    p:setPenaltyFlags(Projectile.kOnContact)
    p:setFireType(Projectile.kFireStraight)
    p:setMoveSpeed(speed)
    p:setPosition(x, y)
    p:setFromToType(Projectile.kPointToPoint)
    p:setFromPoint(p:getPosition())
    p:setToPoint(x1, y1)
    p:setSrcUnit(me:getId())
    p:setContactLeft(1)
    local ad = AttackData:new()
    ad:setAttack(AttackValue.kMagical, 20)
    p:setAttackData(ad)
    p:fire()
    
end

function game03()
    c = 2
    spawnHero(4)
    hero:addActiveAbility(lib.Cutter)
    hero:setBaseMoveSpeed(85)
    hero:setMaxHp(800)
    me:setMaxHp(1000)
    atk = me:getAttackAbility()
    atk:setExAttackValue(2.0, 0.0)
end

function game03_tick(dt)
    
end

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
