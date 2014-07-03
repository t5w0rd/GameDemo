if __ABILITY_LIBRARY__ then return end
__ABILITY_LIBRARY__ = true

include("Ability.lua")

function loadAbilityLibrary()
    local a, id
    
    -- ActiveAbilities
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
    addTemplateAbility(a)
	AL.kThrowHammer = a
    
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
	addTemplateAbility(a)
	AL.kThunderCap = a
    
    -- SpeedUp
    a = SpeedBuff:new("SpeedUp", "SpeedUp", 5.0, false, 0.8, 0.0, 0.8, 0.0)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("SpeedUp", 10.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
	addTemplateAbility(a)
    AL.kSpeedUp = a
    
    -- Reflect
    a = ReflectBuff:new("Reflect", "Reflect", 5.0)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("Reflect", 15.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct4)
    a:addEffectSound("sounds/Effects/LevelUp2.mp3")
	addTemplateAbility(a)
    AL.kReflect = a
    
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
	addTemplateAbility(a)
    AL.kCutter = a

    -- Curse
    a = CurseBuff:new(13, false, 20, 4, 40 / 100)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("Curse", 25.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(200.0)
    a:setCastTargetRadius(100.0)
    a:addCastAnimation(Sprite.kAniAct4)
    a:setImageName("UI/Ability/AbilityCurse.png")
    a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
	addTemplateAbility(a)
    AL.kCurse = a

    -- GravitySurf
    a = SummonUnitAct:new("GravitySurf", 18.0, CommandTarget.kPointTarget, 10002, 10.0)
    a:setCastRange(200.0)
    a:setCastTargetRadius(150.0)
    a:addCastAnimation(Sprite.kAniAct4)
    a:setImageName("UI/Ability/GravitySurf.png")
    a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
	addTemplateAbility(a)
    AL.kGravitySurf = a
    
    -- SpeedUp2
    a = SpeedBuff:new("SpeedUp2", "SpeedUp2", 2.0, false, 3.0, 0.0, 3.0, 0.0)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("SpeedUp2", 10.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
	addTemplateAbility(a)
    AL.kSpeedUp2 = a
    
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
	addTemplateAbility(a)
    AL.kKnockBack = a
    
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
	addTemplateAbility(a)
    AL.kKnockBackEx = a
    
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
	addTemplateAbility(a)
    AL.kThrowHammerEx = a
    
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
	addTemplateAbility(a)
    AL.kWarCry = a
    
    -- MultiSlash
    a = DamageBuff:new("dmg", AttackValue.kPhysical, 75.0, 1.0, false, 0.0, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    
    a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 10, 0, Sprite.kAniAct1, Sprite.kAniAct2)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("MultiSlash", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, 1.0, id)
    a:setCastRange(300)
    a:setImageName("UI/Ability/Ability02.png")
	addTemplateAbility(a)
    AL.kMultiSlash = a
    
    -- ChargeJump
    a = ChargeJumpBuff:new("ChargeJump", 2.0, 2.0, 0.0, 1.0, 0)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("ChargeJump", 18.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:setImageName("UI/Ability/ChargeJump.png")
	addTemplateAbility(a)
    AL.kChargeJump = a
	
	-- MagicalRain
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 0.75, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
	
	a = RainAct:new("MagicalRain", 2.0, RainAct.kBolt, 5.0, 3.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy)
	a:addCastAnimation(Sprite.kAniAct5)
    a:addEffectSound("sounds/Effects/LevelUp.mp3")
    a:setImageName("UI/Ability/Ability03.png")
	addTemplateAbility(a)
    AL.kMagicalRain = a
	
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
	addTemplateAbility(a)
    AL.kBuffMaker = a
    
    -- PassiveAbilities
    
    -- Rebirth
    a = RebirthPas:new("Rebirth", 120.0, 0.4, 0.0)
	addTemplateAbility(a)
    AL.kRebirth = a
    
    -- BerserkerBlood
    a = BerserkerBloodPas:new("BerserkerBlood", 0.05, 0.0, 0.05, 0.0, 0.05, 0.0, 0.05, 0.0)
	addTemplateAbility(a)
    AL.kBerserkerBlood = a
    
    -- StunAttack
    a = StunBuff:new("StunBuff", "Stun", 1, false)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("StunAttack", 0.30, id, false, 1.0, 10.0)
	addTemplateAbility(a)
    AL.kStunAttack = a
    
    -- ArmorBreakAttack
    a = ArmorBuff:new("ArmorBreakBuff", "ArmorBreak", 12, false, -0.0, -4.0)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("ArmorBreakAttack", 1.0, id, false, 1.0, 0.0)
	addTemplateAbility(a)
    AL.kArmorBreakAttack = a
    
    -- VampireAttack
    a = VampirePas:new("VampireAttack", 0.3)
	addTemplateAbility(a)
    AL.kVampireAttack = a
    
    -- CriticalAttack
    a = AttackBuffMakerPas:new("CriticalAttack", 0.15, 0, true, 4.0, 0.0)
	addTemplateAbility(a)
    AL.kCriticalAttack = a
	
	-- DeadlyAttack
    a = AttackBuffMakerPas:new("DeadlyAttack", 0.05, 0, true, 50.0, 0.0)
	addTemplateAbility(a)
    AL.kDeadlyAttack = a
    
    -- FastReflect
    a = ReflectBuff:new("FastReflectBuff", "FastReflect", 0.2)
    id = addTemplateAbility(a)
    
    a = BuffMakerAct:new("FastReflect", 2.0, CommandTarget.kNoTarget, UnitForce.kSelf, 1.0, id)
    a:setImageName("UI/Ability/ThunderCap.png")
    a:addCastAnimation(Sprite.kAniAct2)
    a:addEffectSound("sounds/Effects/KidnapGrab.mp3")
	addTemplateAbility(a)
    AL.kFastReflect = a
    
    -- Evade
    a = EvadePas:new("Evade", 0.20, 0)
	addTemplateAbility(a)
    AL.kEvade = a
    
    -- AutoHeal
    a = ChangeHpPas:new("AutoHeal", 0.2, 0.0, 1.5, 0.0, 0.0)
	addTemplateAbility(a)
    AL.kAutoHeal = a
    
    -- DoubleAttack
    a = DoubleAttackPas:new("DA", 0.2, 0.5, 0.0)
	addTemplateAbility(a)
    AL.kDoubleAttack = a

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
	addTemplateAbility(a)
    AL.kThunderCapAttack = a
    
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
	addTemplateAbility(a)
    AL.kThrowHammerAttack = a
    
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
	addTemplateAbility(a)
    AL.kCutterAttack = a
    
    -- PressureBombAttack
    a = PressureBombBuff:new("PressureBomb", 15.0, 750, AttackValue.kMagical, 360.0)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("PressureBombAttack", 1.0, id, false, 1.0, 0.0)
	addTemplateAbility(a)
    AL.kPressureBombAttack = a
    
    -- DamageIncreaceAttack
    a = DamageIncreaceBuff:new("DamageIncreaceBuff", "DamageIncreace", 6.0, AttackValue.kMagical, 30.0)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("DamageIncreaceAttack", 1.0, id, false, 1.0, 0.0)
	addTemplateAbility(a)
    AL.kDamageIncreaceAttack = a
    
    -- LightningAttack
    a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 0.75, 0.0, Ability.kMaskActiveTrigger)
    id = addTemplateAbility(a)
    
    a = TransitiveLinkBuff:new("LightningAttack", 0.2, 150, 5, 100, UnitForce.kEnemy, PL.kTeslaRay)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
    
    a = AttackBuffMakerPas:new("LightningAttack", 0.20, id, false, 1.0, 0.0, 0)
	addTemplateAbility(a)
    AL.kLightningAttack = a
	
	-- StrikeBack
	a = StunBuff:new("StunBuff", "Stun", 1, false)
    id = addTemplateAbility(a)
	
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 1.00, 0.0, Ability.kOnDamagedDoneTrigger, true)
	a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = StrikeBackPas:new("StrikeBack", 100, 0.1, id)
	addTemplateAbility(a)
	AL.kStrikeBack = a
	
	-- TransitiveAttack
	a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.0, false, 1.00, 0.0, Ability.kOnAttackTargetTrigger, true)
    id = addTemplateAbility(a)
	
	a = TransitiveLinkBuff:new("TransitiveAttack", 0.2, 200.0, 9, 1, UnitForce.kEnemy, PL.kLightning)
    a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = AttackBuffMakerPas:new("TransitiveAttack", 1.00, id, false, 1.0, 0.0, 0)
	addTemplateAbility(a)
    AL.kTransitiveAttack = a
	
	-- ChangeAttributeAttack
	a = ChangeAttributeBuff:new("ChangeAttributeAttackBuff", "ChangeAttributeAttack", 0.5, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0)
	id = addTemplateAbility(a)
	
	a = EvadeBuff:new("EvadeBuff", "Evade", 0.5, false, 1.00)
	a:setAppendBuff(id)
    id = addTemplateAbility(a)
	
	a = AttackBuffMakerPas:new("ChangeAttributeAttack", 0.02, id, true, 1.0, 0.0, 0)
	addTemplateAbility(a)
	AL.kChangeAttributeAttack = a

end

function createAbilityLibrary01()
    local lib = {}
    local a, id
    
    return lib
end

SAL = {}
SAL.STEP = 100
SAL.id = SAL.STEP
function SAL.add(ability)
	local res = addTemplateAbility(SAL.id, ability)
	SAL.id = SAL.id + SAL.STEP
	return res
end

function SAL.addi(ability, index)
	return addTemplateAbility(SAL.id + index, ability)
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
a:setMaxLevel(3)
id = SAL.addi(a, 1)

a = BuffMakerAct:new("群体诅咒", 5.0, CommandTarget.kPointTarget, UnitForce.kEnemy, 1.0, id)
a:setCastRange(200.0)
a:setCastTargetRadius(100.0)
a:addCastAnimation(Sprite.kAniAct4)
a:setImageName("UI/Ability/AbilityCurse.png")
a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
a:setMaxLevel(3)
a:setLevel(0)
a:setDescribe("诅咒一片区域，受诅咒的英雄受到20点/秒的伤害，持续13秒，每隔4秒每损失100点的生命值就会受到40点的额外伤害")
--a:setDescribe("Curses a region cursed hero 20 points of damage per second, 13 seconds every 4 seconds per 100 hit points of damage are 40 extra damage")
a:setLevelInfo(1, 0, "每损失100的生命时所受到的伤害提高40点")
--a:setLevelInfo(1, 0, "Injuries received when the loss of 100 lives per 40 points")
a:setLevelInfo(2, 1, "同时降低单位15%的移动速度")
--a:setLevelInfo(2, 1, "While reducing speed by 15%")
a:setLevelInfo(3, 2, "诅咒效果持续17秒")
--a:setLevelInfo(3, 2, "Curse lasts 17 seconds")
a:setGrade(Ability.kEpic)
a:setCost(3)
a:setLevelUpdate(CurseUpdate)
id = SAL.add(a)

addAbilityToUserData(100, 1)
