--if __ABILITYLIBRARY__ then return end
__ABILITYLIBRARY__ = true

function createAbilityLibrary00()
    local lib = {}
    local a, id
    
    -- ActiveAbilitys
    --  ThrowHammer
    a = StunBuff:new("Stun", "Stun", 2.0, false)
    id = addTemplateAbility(a)
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 50.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
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
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
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
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 10.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
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
    
    a = BuffMakerAct:new("Curse", 25.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(200.0)
    a:setCastTargetRadius(100.0)
    a:addCastAnimation(Sprite.kAniAct4)
    a:setImageName("UI/Ability/AbilityCurse.png")
    a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
    lib.Curse = addTemplateAbility(a)

    -- GravitySurf
    a = SummonUnitAct:new("GravitySurf", 18.0, CommandTarget.kPointTarget, 10002, 10.0)
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
    a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
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
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
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
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 1.0, false, 0.0, 0.0, Ability.kNoMasked, true)
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
    a = DamageBuff:new("dmg", AttackValue.kPhysical, 75.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    
    a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 10, 0, Sprite.kAniAct1, Sprite.kAniAct2)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("MultiSlash", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(300)
    a:setImageName("UI/Ability/Ability02.png")
    lib.MultiSlash = addTemplateAbility(a)
    
    -- ChargeJump
    a = ChargeJumpBuff:new("ChargeJump", 2.0, 2.0, 0.0, 1.0, 0)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("ChargeJump", 18.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:setImageName("UI/Ability/ChargeJump.png")
    lib.ChargeJump = addTemplateAbility(a)
	
	-- MagicalRain
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 0.75, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
	
	a = RainAct:new("MagicalRain", 2.0, RainAct.kBolt, 5.0, 3.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy)
	a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:setImageName("UI/Ability/Ability03.png")
    lib.MagicalRain = addTemplateAbility(a)
	
	-- BuffMaker
	a = StunBuff:new("Stun", "Stun", 0.5, false)
    id = addTemplateAbility(a)
	
	a = WhenDamagedBuff:new("Buff", "Buff", 10.0, false, 0.02, 0.0, id)
	id = addTemplateAbility(a)
	
	a = BuffMakerAct:new("BuffMaker", 1.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(400.0)
    a:addCastAnimation(Sprite.kAniAct3)
    a:setTemplateProjectile(PL.kLightning)
    a:addEffectSound("sounds/Effects/LightningLink.mp3")
    a:setImageName("UI/Ability/ThrowHammer.png")
    lib.BuffMaker = addTemplateAbility(a)
    
    -- PassiveAbilitys
    
    -- Rebirth
    a = RebirthPas:new("Rebirth", 120.0, 0.4, 0.0)
    lib.Rebirth = addTemplateAbility(a)
    
    -- BerserkerBlood
    a = BerserkerBloodPas:new("BerserkerBlood", 0.05, 0.0, 0.05, 0.0, 0.05, 0.0, 0.05, 0.0)
    lib.BerserkerBlood = addTemplateAbility(a)
    
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
    a = AttackBuffMakerPas:new("CriticalAttack", 0.15, 0, true, 4.0, 0.0)
    lib.CriticalAttack = addTemplateAbility(a)
	
	-- DeadlyAttack
    a = AttackBuffMakerPas:new("DeadlyAttack", 0.05, 0, true, 50.0, 0.0)
    lib.DeadlyAttack = addTemplateAbility(a)
    
    -- FastReflect
    a = ReflectBuff:new("FastReflectBuff", "FastReflect", 0.2)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("FastReflect", 2.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:setImageName("UI/Ability/ThunderCap.png")
    a:addCastAnimation(Sprite.kAniAct2)
    a:addEffectSound("sounds/Effects/KidnapGrab.mp3")
    lib.FastReflect = addTemplateAbility(a)
    
    -- Evade
    a = EvadePas:new("Evade", 0.20, 0)
    lib.Evade = addTemplateAbility(a)
    
    -- AutoHeal
    a = ChangeHpPas:new("AutoHeal", 0.2, 0.0, 1.5, 0.0, 0.0)
    lib.AutoHeal = addTemplateAbility(a)
    
    -- DoubleAttack
    a = DoubleAttackPas:new("DA", 0.2, 0.5, 0.0)
    lib.DoubleAttack = addTemplateAbility(a)

    -- ThunderCapAttack
    a = SpeedBuff:new("ThunderCapBuff", "ThunderCap", 3.0, false, -0.5, 0.0, -0.5, 0.0)
    id = addTemplateAbility(a)
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 30.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("ThunderCap", 2.0, CommandTarget.kNoTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct3)
    a:addEffectSound("sounds/Effects/ThunderCap.mp3")
    id = addTemplateAbility(a)

    a = AttackBuffMakerPas:new("ThunderCapAttack", 0.08, 0, false, 1.0, 10.0, id)
    lib.ThunderCapAttack = addTemplateAbility(a)
    
    -- ThrowHammerAttack
    a = StunBuff:new("StunBuff", "Stun", 5.0, false)
    id = addTemplateAbility(a)    
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)

    a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 4, 1, UnitForce.kEnemy, PL.kThorHammer)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("ThrowHammerEx", 1.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(400.0)
    a:addCastAnimation(Sprite.kAniAct3)
    a:setTemplateProjectile(PL.kThorHammer)
    a:addEffectSound("sounds/Effects/LightningLink.mp3")
    a:setImageName("UI/Ability/ThrowHammer.png")
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("ThrowHammerAttack", 0.08, 0, false, 1.0, 10.0, id)
    lib.ThrowHammerAttack = addTemplateAbility(a)
    
    -- CutterAttack
    a = SpeedBuff:new("SlowDownBuff", "SlowDown", 3.0, false, -0.5, 0.0, -0.5, 0.0)
    id = addTemplateAbility(a)
    
    a = ChangeHpBuff:new("Poison", "Poison", 3.0, false, 0.2, -0.01, 0.0, 0.0, -1.0)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = DamageBuff:new("dmg", AttackValue.kMagical, 10.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("Cutter", 1.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(150.0)
    a:addCastAnimation(Sprite.kAniAct2)
    a:setImageName("UI/Ability/AbilityFireBall.png")
    a:setTemplateProjectile(PL.kPirateProy)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("CutterAttack", 0.08, 0, false, 1.0, 0.0, id)
    lib.CutterAttack = addTemplateAbility(a)
    
    -- PressureBombAttack
    a = PressureBombBuff:new("PressureBomb", 15.0, 750, AttackValue.kMagical, 360.0)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("PressureBombAttack", 1.0, id, false, 1.0, 0.0)
    lib.PressureBombAttack = addTemplateAbility(a)
    
    -- DamageIncreaceAttack
    a = DamageIncreaceBuff:new("DamageIncreaceBuff", "DamageIncreace", 6.0, AttackValue.kMagical, 30.0)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("DamageIncreaceAttack", 1.0, id, false, 1.0, 0.0)
    lib.DamageIncreaceAttack = addTemplateAbility(a)
    
    -- LightningAttack
    a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 0.75, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    
    a = TransitiveLinkBuff:new("LightningAttack", 0.2, 150, 5, 100, UnitForce.kEnemy, PL.kTeslaRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("LightningAttack", 0.20, id, false, 1.0, 0.0, 0)
    lib.LightningAttack = addTemplateAbility(a)
	
	-- StrikeBack
	a = StunBuff:new("StunBuff", "Stun", 1, false)
    id = addTemplateAbility(a)
	
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 1.00, 0.0, Ability.kOnDamagedDoneTrigger, true)
	a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = StrikeBackPas:new("StrikeBack", 100, 0.2, id)
	lib.StrikeBack = addTemplateAbility(a)
	
	-- TransitiveAttack
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 1.00, 0.0, Ability.kOnAttackTargetTrigger, true)
    id = addTemplateAbility(a)
	
	a = TransitiveLinkBuff:new("TransitiveAttack", 0.0, 200.0, 9, 1, UnitForce.kEnemy, PL.kArcherArrow)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = AttackBuffMakerPas:new("TransitiveAttack", 1.00, id, false, 1.0, 0.0, 0)
    lib.TransitiveAttack = addTemplateAbility(a)
	
	-- ChangeAttributeAttack
	a = ChangeAttributeBuff:new("ChangeAttributeAttackBuff", "ChangeAttributeAttack", 0.5, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0)
	id = addTemplateAbility(a)
	
	a = EvadeBuff:new("EvadeBuff", "Evade", 0.5, false, 1.00)
	a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = AttackBuffMakerPas:new("ChangeAttributeAttack", 0.20, id, true, 1.0, 0.0, 0)
	lib.ChangeAttributeAttack = addTemplateAbility(a)
    
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
