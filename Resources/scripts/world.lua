include("common.lua")
include("ability.lua")
include("ai.lua")
include("ai2.lua")

hero = nil
me = nil

aaa = {}
c = 0

kill = 0
learned = 0

taid2 = 0

function onWorldInit()
    showDebug(false)
    math.randomseed(os.time())

    me = getControlUnit()
    --me:setAI(LuaAI:new())

    local x, y = me:getPosition()
    me:setPosition(x + 500, y - 200)
    --me:setMaxHp(687)
    
    me:setAlly(2 ^ 3 + 2 ^ 4 + 2 ^ 2)
    
    game02()
    
    return true
end

function onWorldTick(dt)
    game02_tick(dt)
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
    atk:setExAttackSpeed(1.0 + kill / 15, 0.0)
    for i = 1, (3 + kill) do
        hero:addPassiveAbility(aaa[math.random(1, c)])
    end

    a = SpeedBuff:new("SlowDownBuff", "SlowDown", 5.0, false, -0.8, 0.0, -0.8, 0.0)
    id = addTemplateAbility(a)
    a = KnockBackBuff:new("KnockBackBuff", "KnockBackBuff", 0.5, true, 40)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = DamageBuff:new("dmg", AttackValue.kMagical, 150.0, 1.0, false, 0.0, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = BuffMakerAct:new("ThunderCap", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Unit.kAniAct2)
    a:addEffectSound("sounds/Effect/ThunderCap.mp3")
    hero:addActiveAbility(a)
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 150.0, 1.0, false, 0.0, 0.0)
    --a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = BuffMakerAct:new("Wave", 8.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(600.0)
    a:addCastAnimation(Unit.kAniAct3)
    a:setTemplateProjectile(PL.kAlienProy)
    hero:addActiveAbility(a)
    
    hero:setAI(LuaAIWarrior:new())
end

function initAAA()
    a = DamageBackPas:new(0.5)
    c = c + 1
    aaa[c] = addTemplateAbility(a)

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
            playSound("sounds/Effect/OpenDoor.mp3")
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
            playSound("sounds/Effect/WaveIncoming.mp3")
            uc = 0
            el = el - WAVE
        end
    end
end

function game02()
    a = StunBuff:new("Stun", "Stun", 2.0, false)
    id = addTemplateAbility(a)
    a = BuffMakerAct:new("HammerThrow", 18.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(300.0)
    a:addCastAnimation(UnitDrawkAniAct2)
    a:setTemplateProjectile(UL.kThorHammer)
    a:addEffectSound("sounds/Effect/LightningLink.mp3")
    hammerThrow = addTemplateAbility(a)
    
    a = SpeedBuff:new("ThunderCap", "ThunderCap", 3.0, false, -0.5, 0.0, -0.5, 0.0)
    id = addTemplateAbility(a)
    a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = BuffMakerAct:new("ThunderCap", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Unit.kAniAct3)
    a:addEffectSound("sounds/Effect/ThunderCap.mp3")
    thunderCap = addTemplateAbility(a)
    
    a = SpeedBuff:new("SpeedUp", "SpeedUp", 5.0, false, 0.8, 0.0, 0.8, 0.0)
    id = addTemplateAbility(a)
    a = BuffMakerAct:new("ThunderCap", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Unit.kAniAct4)
    a:addEffectSound("sounds/Effect/KRF_sfx_minotauro_grito.mp3")
    speedUp = addTemplateAbility(a)
    
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
    u:addActiveAbility(hammerThrow)
    u:addActiveAbility(thunderCap)
    u:addActiveAbility(speedUp)
    
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
    
end

function game02_tick(dt)
end
