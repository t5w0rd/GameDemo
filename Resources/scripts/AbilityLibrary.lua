function createAbilityLibrary00()
    local lib = {}
    local a, id
    
	-- ActiveAbilitys
	--  ThrowHammer
    a = StunBuff:new("Stun", "Stun", 2.0, false)
    id = addTemplateAbility(a)
    
	a = DamageBuff:new("dmg", AttackValue.kMagical, 50.0, 1.0, false, 0.0, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("ThrowHammer", 18.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(300.0)
    a:addCastAnimation(Sprite.kAniAct2)
    a:setTemplateProjectile(PL.kThorHammer)
    a:addEffectSound("sounds/Effects/LightningLink.mp3")
    lib.ThrowHammer = addTemplateAbility(a)
    
    -- ThunderCap
    a = SpeedBuff:new("ThunderCap", "ThunderCap", 3.0, false, -0.5, 0.0, -0.5, 0.0)
    id = addTemplateAbility(a)
    
	a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("ThunderCap", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct3)
    a:addEffectSound("sounds/Effects/ThunderCap.mp3")
    lib.ThunderCap = addTemplateAbility(a)
    
    -- SpeedUp
    a = SpeedBuff:new("SpeedUp", "SpeedUp", 5.0, false, 0.8, 0.0, 0.8, 0.0)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("SpeedUp", 10.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    lib.SpeedUp = addTemplateAbility(a)
    
    -- Reflect
    a = ReflectBuff:new("Reflect", "Reflect", 5.0)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("Reflect", 15.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct4)
    a:addEffectSound("sounds/Effects/LevelUp2.mp3")
    lib.Reflect = addTemplateAbility(a)
    
    -- Cutter
    a = SpeedBuff:new("SlowDown", "SlowDown", 3.0, false, -0.5, 0.0, -0.5, 0.0)
    id = addTemplateAbility(a)
    
	a = ChangeHpBuff:new("Poison", "Poison", 3.0, false, 0.2, -0.01, 0.0, 0.0, -1.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = DamageBuff:new("dmg", AttackValue.kMagical, 10.0, 1.0, false, 0.0, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("Cutter", 10.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(600.0)
    a:addCastAnimation(Sprite.kAniAct2)
    a:setImageName("UI/Ability/AbilityFireBall.png")
    a:setTemplateProjectile(PL.kPirateProy)
    lib.Cutter = addTemplateAbility(a)

    -- Curse
    a = CurseBuff:new(13, false, 20, 4, 40 / 100)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("Curse", 5.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(200.0)
    a:setCastTargetRadius(100.0)
    a:addCastAnimation(Sprite.kAniAct4)
    a:setImageName("UI/Ability/AbilityCurse.png")
    a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
    lib.Curse = addTemplateAbility(a)

    -- GravitySurf
    a = SummonUnitAct:new("GravitySurf", 1.0, CommandTarget.kPointTarget, 10002, 10.0)
    a:setCastRange(200.0)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct4)
    a:setImageName("UI/Ability/GravitySurf.png")
    a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
    lib.GravitySurf = addTemplateAbility(a)
    
    -- SpeedUp2
    a = SpeedBuff:new("SpeedUp2", "SpeedUp2", 2.0, false, 3.0, 0.0, 3.0, 0.0)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("SpeedUp2", 10.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    lib.SpeedUp2 = addTemplateAbility(a)
    
    -- KnockBack
    a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0)
    id = addTemplateAbility(a)
    
	a = KnockBackBuff:new("KnockBack", "KnockBack", 0.5, 100)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("KnockBack", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct2)
    a:addEffectSound("sounds/Effects/KidnapGrab.mp3")
    lib.KnockBack = addTemplateAbility(a)
	
    -- KnockBackEx
    a = SpeedBuff:new("SlowDownBuff", "SlowDown", 8.0, false, -0.8, 0.0, -0.8, 0.0)
    id = addTemplateAbility(a)
    
    a = KnockBackBuff:new("KnockBackBuff", "KnockBack", 0.5, 40)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 1.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("KnockBackEx", 5.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct2)
    a:addEffectSound("sounds/Effects/ThunderCap.mp3")
    a:setImageName("UI/Ability/ThunderCap.png")
    lib.KnockBackEx = addTemplateAbility(a)
    
    -- ThrowHammerEx
    a = StunBuff:new("StunBuff", "Stun", 5.0, false)
	id = addTemplateAbility(a)	
	
	a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 1.0)
	a:setAppendBuff(id)
    id = addTemplateAbility(a)

    a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 4, 1, UnitForce.kEnemy, PL.kThorHammer)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("ThrowHammerEx", 12.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(400.0)
    a:addCastAnimation(Sprite.kAniAct3)
    a:setTemplateProjectile(PL.kThorHammer)
    a:addEffectSound("sounds/Effects/LightningLink.mp3")
    a:setImageName("UI/Ability/ThrowHammer.png")
    lib.ThrowHammerEx = addTemplateAbility(a)
    
    -- WarCry
    a = SpeedBuff:new("WarCrySpeedBuff", "WarCrySpeed", 10.0, false, 2.0, 0.0, 2.0, 0.0)
    id = addTemplateAbility(a)
    
    a = ChangeHpBuff:new("WarCryHealBuff", "WarCryHeal", 10.0, false, 0.2, 0.02, 0.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("WarCry", 15.0, CommandTarget.kNoTarget, UnitForce.kAlly, 1.0, id)
    a:setCastTargetRadius(200.0)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:setImageName("UI/Ability/WarCry.png")
    lib.WarCry = addTemplateAbility(a)
	
	-- MultiSlash
	a = DamageBuff:new("dmg", AttackValue.kPhysical, 75.0, 1.0)
    id = addTemplateAbility(a)
    
	a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 10, 0, Sprite.kAniAct1, Sprite.kAniAct2)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
	a = BuffMakerAct:new("MultiSlash", 1.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(500)
    a:setImageName("UI/Ability/Ability02.png")
    lib.MultiSlash = addTemplateAbility(a)
	
	-- PassiveAbilitys
	
	-- Rebirth
    a = RebirthPas:new("Rebirth", 120.0)
    lib.Rebirth = addTemplateAbility(a)
	
	-- BerserkerBlood
	a = BerserkerBloodPas:new("BerserkerBlood", 0.05, 0.0, 0.05, 0.0, 0.05, 0.0, 0.05, 0.0)
    lib.BerserkerBlood = addTemplateAbility(a)
	a:setLevel(3)
	a:onChangeLevel(2)
    
	-- StunAttack
	a = StunBuff:new("StunBuff", "Stun", 1, false)
    id = addTemplateAbility(a)
    
	a = AttackBuffMakerPas:new("StunAttack", 0.30, id, false, 1.0, 10.0)
    lib.StunAttack = addTemplateAbility(a)
	
	-- ArmorBreakAttack
	a = ArmorBuff:new("ArmorBreakBuff", "ArmorBreak", 12, false, -0.0, -4.0)
    id = addTemplateAbility(a)
    
	a = AttackBuffMakerPas:new("ArmorBreakAttack", 1.0, id, false, 1.0, 0.0)
    lib.ArmorBreakAttack = addTemplateAbility(a)
	
	-- VampireAttack
	a = VampirePas:new("VampireAttack", 0.3)
    lib.VampireAttack = addTemplateAbility(a)
	
	-- CriticalAttack
	a = AttackBuffMakerPas:new("CriticalAttack", 0.15, id, true, 4.0, 0.0)
    lib.CriticalAttack = addTemplateAbility(a)
	
	
    return lib
end

function createAbilityLibrary01()
	local lib = {}
    local a, id
	
	return lib
end

-- Curse
CurseUpdate = class(LevelUpdate)
function CurseUpdate:onChangeLevel(a, change)
	cast(a, ActiveAbility)
	local lvl = a:getLevel()
	if lvl == 1 then
		a:setCost(5)
	elseif lvl == 2 then
		a:setCost(7)
	elseif lvl == 3 then
		a:setCost(9)
	end
end
CurseUpdate = CurseUpdate:new()

a = CurseBuff:new(13, false, 20, 4, 40 / 100)
id = addTemplateAbility(a)

a = BuffMakerAct:new("群体诅咒", 5.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
a:setCastRange(200.0)
a:setCastTargetRadius(100.0)
a:addCastAnimation(Sprite.kAniAct4)
a:setImageName("UI/Ability/AbilityCurse.png")
a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
a:setMaxLevel(3)
a:setLevel(0)
a:setDescribe("诅咒一片区域，受诅咒的英雄受到20点/秒的伤害，持续13秒，每隔4秒每损失100点的生命值就会受到40点的额外伤害")
a:setLevelInfo(1, 0, "每损失100的生命时所受到的伤害提高40点")
a:setLevelInfo(2, 1, "同时降低单位15%的移动速度")
a:setLevelInfo(3, 2, "诅咒效果持续17秒")
a:setGrade(Ability.kEpic)
a:setCost(3)
a:setLevelUpdate(CurseUpdate)
addTemplateAbility(AL.kCurse, a)

addAbilityToUserData(AL.kCurse, 1)
