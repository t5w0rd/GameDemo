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
    hero:setMaxHp(me:getRealMaxHp() * 0.4 + 400 + (kill / 1.0) * 150)
    if me:getLevel() > 15 and math.random() < 0.1 then
        hero:setExMaxHp(1 + kill / 10, 0.0)
    end
    hero:setRewardGold(50 + kill * 20 / 1.5)
    hero:setRewardExp(20 + kill * 10 / 1.5)
    hero:setExArmorValue(1.0, kill * 0.7)
    atk = hero:getAttackAbility()
    t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, v * (1 + kill / 5.0) + kill * 4)
    atk:setExAttackSpeed(1.0 + kill / 30, 0.0)
    for i = 1, (3 + kill / 2) do
        hero:addPassiveAbility(aaa[math.random(1, c)])
    end

    if atk:getCastRange() > 100 then
        --hero:addActiveAbility(AL.kReflect:getId())
        hero:addActiveAbility(AL.kSpeedUp2:getId())
        hero:addActiveAbility(AL.kKnockBack:getId())
        hero:setAI(LuaAI:new())
    else
        hero:addActiveAbility(AL.kThrowHammer:getId())
        hero:addActiveAbility(AL.kThunderCap:getId())
        hero:addActiveAbility(AL.kSpeedUp:getId())
        hero:setAI(LuaAIWarrior:new())
    end
    
    return hero
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

    a = KnockBackBuff:new("KnockBack", "KnockBack", 0.5, 30)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("KnockBackAttack", 0.2, id, false, 1.5, 0.0, 0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = StunBuff:new("Stun", "Stun", 0.2, false)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("StunAttack", 0.15, id, false, 1.0, 50.0, 0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = ArmorBuff:new("ArmorBreak", "ArmorBreak", 12, true, -0.0, -1.0)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.5, id, false, 1.0, 0.0, 0)
    c = c + 1
    aaa[c] = addTemplateAbility(a)
    
    a = CurseBuff:new(13, false, 15, 4, 30 / 100)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("Critical", 0.2, id, false, 1.0, 0.0, 0)
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

Stage02 = class(Battle)
function Stage02:onInit()

	initAbilityForLevelUp()
	
	me = initForHero()
	me:setMaxLevel(100)
	

    
	
	
	--a = me:getActiveAbility("WarCry")
    --me:delActiveAbility(a)

    --a = me:getActiveAbility("ThunderCap")
    --me:delActiveAbility(a)
    
    --me:addActiveAbility(AL.kMultiSlash:getId())
    --me:addActiveAbility(AL.kKnockBackEx:getId())
    --me:addActiveAbility(AL.kThrowHammerEx:getId())
	--me:addActiveAbility(AL.kBuffMaker:getId())
	--me:addActiveAbility(AL.kMagicalRain:getId())
	--me:addActiveAbility(AL.kWarCry:getId())
    
    me:addPassiveAbility(AL.kStrikeBack:getId())
	me:addPassiveAbility(AL.kDeadlyAttack:getId())
	me:addPassiveAbility(AL.kChangeAttributeAttack:getId())
	--me:addPassiveAbility(AL.kVampireAttack:getId())
    --me:addPassiveAbility(AL.kCriticalAttack:getId())
    --me:addPassiveAbility(AL.kRebirth:getId())
    --me:addPassiveAbility(AL.kThunderCapAttack:getId())
    --me:addPassiveAbility(AL.kThrowHammerAttack:getId())
    --me:addPassiveAbility(AL.kCutterAttack:getId())
	
	
    a = ChangeHpBuff:new("TowerHeal", "TowerHeal", 5, false, 0.3, 0.001, 5, 0, -1)
    id = addTemplateAbility(a)
    a = AuraPas:new("HealAura", 1, id, 200, UnitForce.kAlly + UnitForce.kSelf, false)
    taid = addTemplateAbility(a)
    
    u = createUnit(UL.kArcane)
    u:setMaxHp(u:getMaxHp() * 5)
    u:setBaseArmor(ArmorValue.kWall, 52)
    u:setForceByIndex(1)
    u:setPosition(100, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    u:addPassiveAbility(AL.kAutoHeal:getId())
    --u:addPassiveAbility(DamageBackPas:new(1.2, 0))
    a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, PL.kArcaneRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0, 0)
    u:addPassiveAbility(a)
    tower1 = u:getId()

    u = createUnit(UL.kTesla)
    u:setMaxHp(u:getMaxHp() * 5)
    u:setBaseArmor(ArmorValue.kWall, 52)
    u:setForceByIndex(0)
    u:setPosition(1900, 500)
    u:addPassiveAbility(taid)
    u:addPassiveAbility(taid2)
    u:addPassiveAbility(AL.kAutoHeal:getId())
    --u:addPassiveAbility(DamageBackPas:new(1.0, 0))
    a = DamageBuff:new("dmg", AttackValue.kMagical, 350.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    a = TransitiveLinkBuff:new("TransitiveLink", 0.2, 300, 8, 100, UnitForce.kEnemy, PL.kTeslaRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    a = AttackBuffMakerPas:new("RayLink", 0.20, id, false, 1.0, 0.0, 0)
    u:addPassiveAbility(a)
    tower2 = u:getId()
    
end

function Stage02:onTick(dt)
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
                
                u = spawnSoldier(4, 0)
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
                
                u = spawnSoldier(23, 0)
                u:setMaxHp(200 + lvl * 63)
                atk = u:getAttackAbility()
                atk:setExAttackValue(1.0 + lvl * 0.05, lvl * 2.3)
            end
            if true or not hero then
                local u = spawnHero()
                a = u:getAttackAbility()
                if a:getCastRange() < 150 and math.random(0, 100) < 50 then
                    spawnHero(4)
                    hero:addActiveAbility(AL.kCutter:getId())
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

Stage02:new():run()
