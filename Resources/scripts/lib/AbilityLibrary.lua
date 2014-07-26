if not DEBUG and __ABILITY_LIBRARY__ then return end
__ABILITY_LIBRARY__ = true

include("lib/Ability.lua")
include("lib/LevelUpdate.lua")
include("lib/ObjectLibrary.lua")


AL = ObjectLibrary:new(100, function(self, id, obj)
    addTemplateAbility(id, obj)
end)

-- CriticalAttack
Update = class(LevelUpdate)
function Update:onLevelChanged(a, change)
	cast(a, AttackBuffMakerPas)
	local lv = a:getLevel()
	if lv == 1 then
		a:setCost(5)
		a:setExAttackValue(3.0, 0.0)
	end
end

a = AttackBuffMakerPas:new("致命一击", 0.15, 0, true, 2.0, 0.0)
a:setImageName("UI/Ability/CriticalAttack.png")
a:setMaxLevel(1)
a:setLevel(0)
a:setCost(3)
a:setDescribe("攻击时有15%的概率对目标造成200%的伤害")
a:setLevelInfo(1, 0, "造成300%的伤害")
a:setGrade(Ability.kNormal)
a:setLevelUpdate(Update:new())
AL.kCriticalAttack = AL:add(a)
	
-- Curse
Update = class(LevelUpdate)
function Update:onLevelChanged(a, change)
	cast(a, BuffMakerAct)
	local lv = a:getLevel()
	if lv == 1 then
		a:setCost(5)
	elseif lv == 2 then
		a:setCost(7)
	elseif lv == 3 then
		a:setCost(9)
	end
end

a = SpeedBuff:new("SlowDown", "SlowDown", 3.0, false, -0.15, 0.0, 0.0, 0.0)
id = AL:add2(a)  -- 002

CurseBuffUpdate = class(LevelUpdate)
CurseBuffUpdate.id = id
function CurseBuffUpdate:onLevelChanged(a, change)
	cast(a, CurseBuff)
	local lv = a:getLevel()
	
	if lv >= 1 then
		a.multiply = 80 / 100
	end
	
	if lv >= 2 then
		a:setAppendBuff(self.id)
	end
	
	if lv >= 3 then
		a:setDuration(17.0)
	end
	
end
CurseBuffUpdate = CurseBuffUpdate:new()

a = CurseBuff:new(13, false, 20, 4, 40 / 100)
a:setLevelUpdate(CurseBuffUpdate)
a:setMaxLevel(3)
id = AL:add2(a)  -- 001

a = BuffMakerAct:new("群体诅咒", 30.0, CommandTarget.kPointTarget, UnitForce.kEnemy, id)
a:setCastRange(200.0)
a:setCastTargetRadius(100.0)
a:addCastAnimation(Sprite.kAniAct4)
a:setImageName("UI/Ability/AbilityCurse.png")
a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
a:setMaxLevel(3)
a:setLevel(0)
a:setCost(3)
a:setDescribe("诅咒一片区域，受诅咒的英雄受到20点/秒的伤害，持续13秒，每隔4秒每损失100点的生命值就会受到40点的额外伤害")
a:setLevelInfo(1, 0, "每损失100的生命时所受到的伤害提高40点")
a:setLevelInfo(2, 1, "同时降低单位15%的移动速度，持续3秒")
a:setLevelInfo(3, 2, "诅咒效果持续17秒")
a:setGrade(Ability.kEpic)
a:setLevelUpdate(Update:new())
AL.kMassCurse = AL:add(a)


-- MultiSlash
Update = class(LevelUpdate)
Update.buff = {}
function Update:onLevelChanged(a, change)
	cast(a, BuffMakerAct)
	local lv = a:getLevel()
	if lv == 1 then
		a:setTemplateBuff(self.buff[lv])
		a:setCost(7)
	elseif lv == 2 then
		a:setTemplateBuff(self.buff[lv])
		a:setCost(10)
	elseif lv == 3 then
		a:setTemplateBuff(self.buff[lv])
		a:setCost(16)
	end
end

-- Lv1
a = DamageBuff:new("dmg", AttackValue.kPhysical, 0.0, 1.0, 0.0, Ability.kMaskActiveTrigger, true)
id = AL:add2(a)  -- 002

a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 5, 0, Sprite.kAniAct1, Sprite.kAniAct2)
a:setAppendBuff(id)
Update.buff[1] = AL:add2(a)  -- 012

-- Lv2
a = DamageBuff:new("dmg", AttackValue.kPhysical, 0.0, 1.0, 0.0, Ability.kMaskActiveTrigger, true)
id = AL:add2(a)  -- 003

a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 9, 0, Sprite.kAniAct1, Sprite.kAniAct2)
a:setAppendBuff(id)
Update.buff[2] = AL:add2(a)  -- 013

-- Lv3
a = DamageBuff:new("dmg", AttackValue.kPhysical, 0.0, 1.0, 0.0, Ability.kNoMasked, true)
id = AL:add2(a)  -- 004

a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 9, 0, Sprite.kAniAct1, Sprite.kAniAct2)
a:setAppendBuff(id)
Update.buff[3] = AL:add2(a)  -- 014

-- Lv0
a = DamageBuff:new("dmg", AttackValue.kPhysical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger, true)
id = AL:add2(a)  -- 001

a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 5, 0, Sprite.kAniAct1, Sprite.kAniAct2)
a:setAppendBuff(id)
id = AL:add2(a)  -- 011

a = BuffMakerAct:new("幻影斩击", 30.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(300)
a:setImageName("UI/Ability/Slash.png")
a:setMaxLevel(3)
a:setLevel(0)
a:setCost(5)
a:setDescribe("快速跳跃式的随即斩击战场上的敌人，每次斩击继承75%攻击力，共斩击6次，享受攻击力加成")
a:setLevelInfo(1, 0, "每次斩击继承100%攻击力")
a:setLevelInfo(2, 1, "斩击次数提高至10次")
a:setLevelInfo(3, 2, "每次斩击可以触发其他技能效果")
a:setGrade(Ability.kLegend)

a:setLevelUpdate(Update:new())
AL.kMultiSlash = AL:add(a)


-- MagicalRain
Update = class(LevelUpdate)
function Update:onLevelChanged(a, change)
	cast(a, RainAct)
	local lv = a:getLevel()
	if lv == 1 then
		a:setCost(7)
		a:setCastTargetRadius(150)
	elseif lv == 2 then
		a:setCost(10)
		a:setCastTargetRadius(150)
		a.intensity = 8
	elseif lv == 3 then
		a:setCost(12)
		a:setCastTargetRadius(150)
		a.intensity = 8
		a.duration = 5
	end
end

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.20, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)  -- 001

a = RainAct:new("魔法箭雨", 15.0, EL.kRainBolt, 5.0, 3.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy)
a:addCastAnimation(Sprite.kAniAct3)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/TrebleBolt.png")
a:setCastRange(300)
a:setCastTargetRadius(100)
a:setMaxLevel(3)
a:setLevel(0)
a:setCost(5)
a:setDescribe("召唤魔法箭雨袭击指定区域内敌人，在半径100内每秒降下5支魔法箭，每支魔法箭造成20%攻击力的伤害，持续3秒")
a:setLevelInfo(1, 0, "降雨半径提升为150")
a:setLevelInfo(2, 1, "魔法箭密度提升至每秒8支")
a:setLevelInfo(3, 1, "降雨持续时间提升至5秒")
a:setGrade(Ability.kRare)

a:setLevelUpdate(Update:new())
AL.kMageRain = AL:add(a)


-- ThunderHammer
Update = class(LevelUpdate)
Update.stun = 0
Update.buff = {}
function Update:onLevelChanged(a, change)
	cast(a, BuffMakerAct)
	local lv = a:getLevel()
	if lv == 1 then
		a:setCost(10)
		a:setTemplateBuff(self.buff[lv])
	elseif lv == 2 then
		a:setCost(18)
		a:setTemplateBuff(self.buff[lv])
	end
end

a = StunBuff:new("StunBuff", "Stun", 5.0, false)
Update.stun = AL:add2(a)

-- Lv1
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.00, 0.0, Ability.kMaskActiveTrigger, true)
a:setAppendBuff(Update.stun)
id = AL:add2(a)

a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 4, 1, UnitForce.kEnemy, false, PL.kThorHammer)
a:setAppendBuff(id)
Update.buff[1] = AL:add2(a)

-- Lv2
a = DamageBuff:new("dmg", AttackValue.kHoly, 0.0, 1.00, 0.0, Ability.kMaskActiveTrigger, true, true)
a:setAppendBuff(Update.stun)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.00, 0.0, Ability.kMaskActiveTrigger, true)
a:setAppendBuff(id)
id = AL:add2(a)

a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 4, 1, UnitForce.kEnemy, false, PL.kThorHammer)
a:setAppendBuff(id)
Update.buff[2] = AL:add2(a)

-- Lv0
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.00, 0.0, Ability.kMaskActiveTrigger, true)
a:setAppendBuff(Update.stun)
id = AL:add2(a)

a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 0, 1, UnitForce.kEnemy, false, PL.kThorHammer)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("雷神之锤", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(400.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(PL.kThorHammer)
a:addEffectSound("sounds/Effects/LightningLink.mp3")
a:setImageName("UI/Ability/ThrowHammer.png")
a:setMaxLevel(2)
a:setLevel(0)
a:setCost(5)
a:setDescribe("投掷雷神之锤，造成100%攻击力的伤害，并且击晕目标5秒")
a:setLevelInfo(1, 0, "雷神之锤额外弹射4次")
a:setLevelInfo(2, 3, "附加10%攻击力的神圣伤害")
a:setGrade(Ability.kLegend)

a:setLevelUpdate(Update:new())
AL.kThunderHammer = AL:add(a)



addAbilityToUserData(100, 0)
addAbilityToUserData(200, 0)
addAbilityToUserData(300, 0)
addAbilityToUserData(400, 0)
addAbilityToUserData(500, 0)







-- ActiveAbilities
--  ThrowHammer
a = StunBuff:new("StunBuff", "Stun", 2.0, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 2.5, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("风暴战锤", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(300.0)
a:addCastAnimation(Sprite.kAniAct2)
a:setTemplateProjectile(PL.kThorHammer)
a:setImageName("UI/Ability/ThrowHammer.png")
a:addEffectSound("sounds/Effects/LightningLink.mp3")
AL.kThrowHammer = AL:add(a)

-- ThunderCap
a = EffectBuff:new(EL.kEarthQuake, false)
id = AL:add2(a)

a = SpeedBuff:new("ThunderCapBuff", "SlowDown", 5.0, true, -0.8, 0.0, -0.8, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = StunBuff:new("StunBuff", "Stun", 2.0, false)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.5, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("雷霆震地", 18.0, CommandTarget.kNoTarget, UnitForce.kEnemy, id)
a:setCastTargetRadius(150.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setImageName("UI/Ability/ThunderCap.png")
a:addEffectSound("sounds/Effects/ThunderCap.mp3")
AL.kThunderCap = AL:add(a)

-- ShieldBash
a = StunBuff:new("StunBuff", "Stun", 3.5, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 2.0, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("盾击", 15.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastHorizontal(true)
a:setCastRange(50.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setImageName("UI/Ability/GravitySurf.png")
a:addEffectSound("sounds/Effects/ThunderCap.mp3")
AL.kShieldBash = AL:add(a)

-- SpeedUp
a = SplashPas:new("Splash", 25, 1.00, 0.0, 50, 0.50, 0.0)
id = AL:add2(a)

a = LimitedPasBuff:new("LimitedPas", 8.0, id)
id = AL:add2(a)

a = ChangeAttributeBuff:new("RageBuff", "Rage", 8.0, true, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, -0.50, -5.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("狂暴", 25.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct5)
a:setImageName("UI/Ability/PowerUp.png")
a:addEffectSound("sounds/Effects/LevelUp.mp3")
AL.kSpeedUp = AL:add(a)

-- Reflect
a = ReflectBuff:new("Reflect", "Reflect", 5.0)
id = AL:add2(a)

a = BuffMakerAct:new("Reflect", 15.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp2.mp3")
AL.kReflect = AL:add(a)

-- Cutter
a = EffectBuff:new(EL.kFireBig, true)
id = AL:add2(a)

a = SpeedBuff:new("SlowDown", "SlowDown", 3.0, false, -0.5, 0.0, -0.5, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = ChangeHpBuff:new("FireBuff", "Fire", 3.0, false, 0.2, -0.003, -10.0, 0.0, -1.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.5, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("汽油弹", 10.0, CommandTarget.kPointTarget, UnitForce.kEnemy, id)
a:setCastRange(600.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setImageName("UI/Ability/Fire.png")
a:setTemplateProjectile(PL.kPirateProy)
AL.kCutter = AL:add(a)

-- Curse
a = CurseBuff:new(13, false, 20, 4, 40 / 100)
id = AL:add2(a)

a = BuffMakerAct:new("Curse", 25.0, CommandTarget.kPointTarget, UnitForce.kEnemy, id)
a:setCastRange(200.0)
a:setCastTargetRadius(100.0)
a:addCastAnimation(Sprite.kAniAct4)
a:setImageName("UI/Ability/AbilityCurse.png")
a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
AL.kCurse = AL:add(a)

-- GravitySurf
a = SummonUnitAct:new("引力虚空", 40.0, CommandTarget.kPointTarget, ULx.kBlackHole, 5.0)
a:setEffectiveTypeFlags(UnitForce.kEnemy)
a:setCastRange(100.0)
a:setCastTargetRadius(150.0)
a:addCastAnimation(Sprite.kAniAct4)
a:setImageName("UI/Ability/GravitySurf.png")
a:addEffectSound("sounds/Effects/KRF_sfx_vodoo_kamikazelanza.mp3")
AL.kGravitySurf = AL:add(a)

-- SpeedUp2
a = EvadeBuff:new("SU.EB", "闪避", 3.0, false, 1.00)
id = AL:add2(a)

a = SpeedBuff:new("SU.SB", "SpeedUp", 3.0, false, 4.0, 0.0, 4.0, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("疾速", 15.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct5)
a:setImageName("UI/Ability/SpeedUp.png")
a:addEffectSound("sounds/Effects/LevelUp.mp3")
AL.kSpeedUp2 = AL:add(a)

-- KnockBack
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)

a = ChangeAttributeBuff:new("KB.CAB", "SlowDown", 1.0, false, 0.0, 0.0, -0.50, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = KnockBackBuff:new("KnockBack", 0.5, 25)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("击退", 8.0, CommandTarget.kNoTarget, UnitForce.kEnemy, id)
a:setCastTargetRadius(50.0)
a:addCastAnimation(Sprite.kAniAct2)
a:setImageName("UI/Ability/Wave.png")
a:addEffectSound("sounds/Effects/KidnapGrab.mp3")
AL.kKnockBack = AL:add(a)

-- KnockBackEx
a = SpeedBuff:new("SlowDownBuff", "SlowDown", 8.0, false, -0.8, 0.0, -0.8, 0.0)
id = AL:add2(a)

a = KnockBackBuff:new("KnockBack", 0.5, 40)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("KnockBackEx", 5.0, CommandTarget.kNoTarget, UnitForce.kEnemy, id)
a:setCastTargetRadius(150.0)
a:addCastAnimation(Sprite.kAniAct2)
a:addEffectSound("sounds/Effects/ThunderCap.mp3")
a:setImageName("UI/Ability/ThunderCap.png")
AL.kKnockBackEx = AL:add(a)

-- kThunderHammer
a = StunBuff:new("StunBuff", "Stun", 2.0, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 2.0, 0.0, Ability.kNoMasked, true)
a:setAppendBuff(id)
id = AL:add2(a)

a = TransitiveLinkBuff:new("Chain", 0.1, 100.0, 3, 1, UnitForce.kEnemy, false, PL.kThorHammer)
a:setAppendBuff(id)
id = AL:add2(a)

a = StunBuff:new("StunBuff", "Stun", 3.0, false)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kHoly, 0.0, 2.00, 0.0, Ability.kMaskActiveTrigger, true, true)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("雷神之锤", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(400.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(PL.kThorHammer)
a:addEffectSound("sounds/Effects/LightningLink.mp3")
a:setImageName("UI/Ability/ThrowHammer.png")
AL.kThunderHammer2 = AL:add(a)

-- HealWind
a = ChangeAttributeBuff:new("HW.CAB", "HealWind", 5.0, false, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 999.0)
id = AL:add2(a)

a = ChangeHpBuff:new("HW.CHB", "HealWind", 5.0, false, 0.1, 0.005, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("圣愈之风", 40.0, CommandTarget.kNoTarget, UnitForce.kSelf + UnitForce.kAlly, id)
a:setCastTargetRadius(200.0)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/HealWind.png")
AL.kHealWind = AL:add(a)

-- kArmorUpCry
a = ChangeAttributeBuff:new("AUC.CAB", "ChangeAttributeAttack", 5.0, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 10.0)
id = AL:add2(a)

a = BuffMakerAct:new("守护咆哮", 20.0, CommandTarget.kNoTarget, UnitForce.kSelf + UnitForce.kAlly, id)
a:setCastTargetRadius(200.0)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/ArmorUp.png")
AL.kArmorUpCry = AL:add(a)

-- kSpeedUpCry
a = ChangeAttributeBuff:new("SUC.CAB", "ChangeAttributeAttack", 5.0, false, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = BuffMakerAct:new("狂风咆哮", 20.0, CommandTarget.kNoTarget, UnitForce.kSelf + UnitForce.kAlly, id)
a:setCastTargetRadius(200.0)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/AttackValue.png")
AL.kSpeedUpCry = AL:add(a)

-- MultiSlash
a = DamageBuff:new("dmg", AttackValue.kPhysical, 75.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)

a = TransitiveBlinkBuff:new("TransitiveBlink", 200, 10, 0, Sprite.kAniAct1, Sprite.kAniAct2)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("MultiSlash", 20.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(300)
a:setImageName("UI/Ability/Slash.png")
AL.kMultiSlash2 = AL:add(a)

-- ChargeJump
a = ChargeJumpBuff:new("ChargeJump", 2.0, 2.0, 0.0, 1.0, 0)
id = AL:add2(a)

a = BuffMakerAct:new("英勇疾跃", 20.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct5)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/ChargeJump.png")
AL.kChargeJump = AL:add(a)

-- MagicalRain
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)

a = RainAct:new("MagicalRain", 2.0, RainAct.kBolt, 5.0, 3.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy)
a:addCastAnimation(Sprite.kAniAct5)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/TrebleBolt.png")
AL.kMagicalRain = AL:add(a)

-- MissileStorm
a = EffectBuff:new(EL.kExplosionSmall, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = ProjectileRainAct:new("导弹风暴", 40.0, PLx.kMissile, 10.0, 5.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy, "sounds/Effects/Sound_RocketLaunt.mp3", 2.0)
a:addCastAnimation(Sprite.kAniAct3)
--a:addEffectSound("sounds/Effects/AV_ULT.mp3")
a:setImageName("UI/Ability/FireSmoke.png")
a:setCastTargetRadius(250)
AL.kMissileStorm = AL:add(a)

-- SweetDew
a = ChangeAttributeBuff:new("SD.CAB", "SweetDew", 5.0, false, 0.2, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = ChangeHpBuff:new("SD.CHB", "SweetDew", 2.0, true, 0.05, 0.002, 0.0, 0.0, -1.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("治疗甘露", 3.0, CommandTarget.kUnitTarget, UnitForce.kSelf + UnitForce.kAlly, id)
a:setCastRange(300.0)
--a:setCastTargetRadius(75.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(PLx.kWizardProy)
a:addEffectSound("sounds/Effects/sound_eff_impact_heal.mp3")
a:setImageName("UI/Ability/SweetDew.png")
AL.kSweetDew = AL:add(a)

-- SnowStorm
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)

a = SpeedBuff:new("SnowStormBuff", "SlowDown", 2.0, false, -0.5, 0.0, -0.5, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = RainAct:new("冰锥术", 15.0, EL.kRainSharpIce, 10.0, 1.0, id, CommandTarget.kPointTarget, UnitForce.kEnemy)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/BluePower.png")
a:setCastRange(300)
a:setCastTargetRadius(75)
AL.kSnowStorm = AL:add(a)

-- SerialExplode
a = DamageIncreaceBuff:new("SE.DI", "DamageIncreace", 1.0, AttackValue.kMagical, 0.0, 1.50, 0.0)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.50, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = SerialExplodeAct:new("连环爆轰", 30.0, 100, 5, 0.2, 50, id)
a:addCastAnimation(Sprite.kAniAct2)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/SerialExplode.png")
--a:setCastRange(500)
AL.kSerialExplode = AL:add(a)

-- FastMoveToBack
a = EffectBuff:new(EL.kBleedingRedBig, true)
id = AL:add2(a)

a = ArmorBuff:new("FMTB.AB", "ArmorBreak", 0.0, true, -0.00, -15.0)
a:setAppendBuff(id)
id = AL:add2(a)

--a = StunBuff:new("StunBuff", "Stun", 1, false)
--a:setAppendBuff(id)
--id = AL:add2(a)

a = ChangeHpBuff:new("FMTB.BleedingBuff", "Bleeding", 5.0, false, 0.1, -0.0002, -10.0, 0.0, -1.0)
a:setAppendBuff(id)
id2 = AL:add2(a)

a = BackStabBuff:new("FMTB.BackStabBuff", "BackStab", 0.5, false, 0, id2)
id = AL:add2(a)
a.buffForSelf = id

a = ChangeAttributeBuff:new("FMTB.CAB", "ChangeAttributeAttack", 1.0, false, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.50, 0.0, 0.0, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = FastMoveToBackBuff:new("FastMoveToBack", 12, 2.0, 500.0, id)
id = AL:add2(a)

a = BuffMakerAct:new("幽灵背刺", 26.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct4)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/CriticalAttack.png")
AL.kFastMoveToBack = AL:add(a)

-- SwordWave
a = ChangeAttributeBuff:new("CA.CAB", "ChangeAttributeAttack", 8.0, false, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, -0.99, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = ChangeAttackBuff:new("ChangeAttack", 8.0, AttackValue.kMagical, 0.5, nil, 200.0, false, PLx.kGreenWave)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("逐风式", 35.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct2)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/AttackValue.png")
AL.kSwordWave = AL:add(a)

-- ShotGun
a = SplashPas:new("Splash", 25, 1.00, 0.0, 50, 0.50, 0.0)
id = AL:add2(a)

a = LimitedPasBuff:new("LimitedPas", 10.0, id)
id = AL:add2(a)

a = ChangeAttributeBuff:new("SG.CAB", "ChangeAttributeAttack", 10.0, false, 0.0, 0.0, 0.80, 0.0, 0.80, 0.0, -0.50, 0.0, 0.0, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = ChangeAttackBuff:new("ChangeAttack", 10.0, AttackValue.kPhysical, 0.2, nil, 100.0, false, nil)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("速射散弹枪", 30.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:addCastAnimation(Sprite.kAniAct2)
a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/Shot.png")
AL.kShotGun = AL:add(a)

-- BouncingFireBall
a = EffectBuff:new(EL.kExplosionShort, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.50, 0.0, Ability.kMaskActiveTrigger, false, true, 80)
a:setAppendBuff(id)
id = AL:add2(a)

a = TransitiveLinkBuff:new("FireBall.TLB", 0.0, 200.0, 3, 1, UnitForce.kEnemy, true, PLx.kFireBall)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("爆裂之球", 30.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(400.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(PLx.kFireBall)
--a:addEffectSound("sounds/Effects/sound_eff_impact_heal.mp3")
a:setImageName("UI/Ability/FireBall.png")
AL.kBouncingFireBall = AL:add(a)

-- ArrowWind
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)

a = ProjectileWindAct:new("箭矢狂风", 40.0, PL.kArcherArrow2, 30.0, 500, 500, 5.0, id, UnitForce.kEnemy)
a:addCastAnimation(Sprite.kAniAct4)
--a:addEffectSound("sounds/Effects/LevelUp.mp3")
a:setImageName("UI/Ability/TrebleBolt.png")
AL.kArrowWind = AL:add(a)

-- BuffMaker
--a = StunBuff:new("StunBuff", "Stun", 0.5, false)
--id = AL:add2(a)

--a = WhenDamagedBuff:new("Buff", "Buff", 10.0, false, 0.02, 0.0, id)
--id = AL:add2(a)

a = ChangeHpBuff:new("Heal", "Heal", 1.0, true, 0.05, 0.0125, 0.0, 0.0, -1.0)
id = AL:add2(a)

a = BuffMakerAct:new("BuffMaker", 1.0, CommandTarget.kUnitTarget, UnitForce.kSelf + UnitForce.kAlly, id)
a:setCastRange(400.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(1)
a:addEffectSound("sounds/Effects/sound_eff_impact_heal.mp3")
a:setImageName("UI/Ability/Book.png")
AL.kBuffMaker = AL:add(a)

-- TestAct
a = TestAct:new("Test", 1.0, CommandTarget.kUnitTarget, UnitForce.kSelf + UnitForce.kAlly)
a:setCastRange(400.0)
a:setImageName("UI/Ability/Book.png")
AL.kTest = AL:add(a)

-- PassiveAbilities

-- Rebirth
a = RebirthPas:new("Rebirth", 120.0, 0.4, 0.0)
AL.kRebirth = AL:add(a)

-- BerserkerBlood
a = BerserkerBloodPas:new("狂战士之血", 0.05, 0.0, 0.05, 0.0, 0.05, 0.0, 0.05, 0.0)
AL.kBerserkerBlood = AL:add(a)

-- StunAttack
a = StunBuff:new("StunBuff", "Stun", 1, false)
id = AL:add2(a)

a = AttackBuffMakerPas:new("StunAttack", 0.30, id, false, 1.0, 10.0)
AL.kStunAttack = AL:add(a)

-- ArmorBreakAttack
a = ArmorBuff:new("ArmorBreakBuff", "ArmorBreak", 5, true, -0.0, -4.0)
id = AL:add2(a)

a = AttackBuffMakerPas:new("ArmorBreakAttack", 0.2, id, false, 1.0, 0.0)
AL.kArmorBreakAttack = AL:add(a)

-- VampireAttack
a = VampirePas:new("VampireAttack", 0.3)
AL.kVampireAttack = AL:add(a)

-- CriticalAttack
a = AttackBuffMakerPas:new("CriticalAttack", 0.15, 0, true, 4.0, 0.0)
AL.kCriticalAttack2 = AL:add(a)

-- SlowDownAttack
a = SpeedBuff:new("SlowDownAttackBuff", "SlowDown", 3, false, -0.3, 0.0, -0.3, 0.0)
id = AL:add2(a)
a = AttackBuffMakerPas:new("SlowDownAttack", 1.0, id, false, 1.0, 0.0, 0)
AL.kSlowDownAttack = AL:add(a)

-- KnockBackAttack
a = DamageBuff:new("dmg", AttackValue.kPhysical, 0.0, 1.5, 0.0, Ability.kMaskActiveTrigger)
id = AL:add2(a)
a = KnockBackBuff:new("KnockBack", 0.5, 30)
a:setAppendBuff(id)
id = AL:add2(a)
a = AttackBuffMakerPas:new("KnockBackAttack", 0.3, id, false, 1.5, 0.0, 0)
AL.kKnockBackAttack = AL:add(a)

-- DeadlyAttack
a = AttackBuffMakerPas:new("DeadlyAttack", 0.05, 0, true, 50.0, 0.0)
AL.kDeadlyAttack = AL:add(a)

-- FastReflect
a = ReflectBuff:new("FastReflectBuff", "FastReflect", 0.2)
id = AL:add2(a)

a = BuffMakerAct:new("FastReflect", 2.0, CommandTarget.kNoTarget, UnitForce.kSelf, id)
a:setImageName("UI/Ability/ThunderCap.png")
a:addCastAnimation(Sprite.kAniAct2)
a:addEffectSound("sounds/Effects/KidnapGrab.mp3")
AL.kFastReflect = AL:add(a)

-- Evade
a = EvadePas:new("Evade", 0.20, 0)
AL.kEvade = AL:add(a)

-- EvadeHeal
a = ChangeHpBuff:new("EvadeHealBuff", "EvadeHeal", 3, true, 0.2, 0.0002, 1, 0.0, 0.0)
id = AL:add2(a)
a = EvadePas:new("EvadeHeal", 0.20, id)
AL.kEvadeHeal = AL:add(a)

-- AutoHeal
a = ChangeHpPas:new("AutoHeal", 0.2, 0.0, 1.5, 0.0, 0.0)
AL.kAutoHeal = AL:add(a)

-- AutoDamaged
a = ChangeHpPas:new("AutoDamaged", 0.2, -0.005, 0.0, 0.2, 0.0)
AL.kAutoDamaged = AL:add(a)

-- kSplashAttack
a = SplashPas:new("SplashAttack", 25, 1.00, 0.0, 50, 0.50, 0.0)
AL.kSplashAttack = AL:add(a)

-- DoubleAttack
a = DoubleAttackPas:new("DA", 0.2, 0.5, 0.0)
AL.kDoubleAttack = AL:add(a)

-- ThunderCapAttack
a = EffectBuff:new(EL.kEarthQuake, true)
id = AL:add2(a)

a = SpeedBuff:new("ThunderCapBuff", "ThunderCap", 3.0, true, -0.2, 0.0, -0.2, 0.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.5, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("ThunderCap", 1.0, CommandTarget.kNoTarget, UnitForce.kEnemy, id)
a:setCastTargetRadius(100.0)
a:addCastAnimation(Sprite.kAniAct3)
a:addEffectSound("sounds/Effects/ThunderCap.mp3")
id = AL:add2(a)

a = AttackBuffMakerPas:new("ThunderCapAttack", 0.30, 0, false, 1.0, 10.0, id)
AL.kThunderCapAttack = AL:add(a)

-- ThrowHammerAttack
a = StunBuff:new("StunBuff", "Stun", 5.0, false)
id = AL:add2(a)	

a = DamageBuff:new("dmg", AttackValue.kMagical, 100.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = TransitiveLinkBuff:new("Chain", 0.1, 150.0, 4, 1, UnitForce.kEnemy, false, PL.kThorHammer)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("ThrowHammerEx", 1.0, CommandTarget.kUnitTarget, UnitForce.kEnemy, id)
a:setCastRange(400.0)
a:addCastAnimation(Sprite.kAniAct3)
a:setTemplateProjectile(PL.kThorHammer)
a:addEffectSound("sounds/Effects/LightningLink.mp3")
a:setImageName("UI/Ability/ThrowHammer.png")
id = AL:add2(a)

a = AttackBuffMakerPas:new("ThrowHammerAttack", 0.08, 0, false, 1.0, 10.0, id)
AL.kThrowHammerAttack = AL:add(a)

-- CutterAttack
a = SpeedBuff:new("SlowDownBuff", "SlowDown", 3.0, false, -0.5, 0.0, -0.5, 0.0)
id = AL:add2(a)

a = ChangeHpBuff:new("Poison", "Poison", 3.0, false, 0.2, -0.001, -10.0, 0.0, -1.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 10.0, 0.0, 0.0, Ability.kMaskActiveTrigger)
a:setAppendBuff(id)
id = AL:add2(a)

a = BuffMakerAct:new("Cutter", 1.0, CommandTarget.kPointTarget, UnitForce.kEnemy, id)
a:setCastRange(150.0)
a:addCastAnimation(Sprite.kAniAct2)
a:setImageName("UI/Ability/FireBall.png")
a:setTemplateProjectile(PL.kPirateProy)
id = AL:add2(a)

a = AttackBuffMakerPas:new("CutterAttack", 0.03, 0, false, 1.0, 0.0, id)
AL.kCutterAttack = AL:add(a)

-- PressureBombAttack
a = PressureBombBuff:new("PressureBomb", 15.0, 800, AttackValue.kMagical, 300.0, 50)
id = AL:add2(a)

a = AttackBuffMakerPas:new("PressureBombAttack", 1.0, id, false, 1.0, 0.0)
AL.kPressureBombAttack = AL:add(a)

-- DamageIncreaceAttack
a = DamageIncreaceBuff:new("DamageIncreaceBuff", "DamageIncreace", 6.0, AttackValue.kMagical, 30.0)
id = AL:add2(a)

a = AttackBuffMakerPas:new("DamageIncreaceAttack", 1.0, id, false, 1.0, 0.0)
AL.kDamageIncreaceAttack = AL:add(a)

-- LightningAttack
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.75, 0.0, Ability.kOnAttackTargetTrigger, true, false)
id = AL:add2(a)

a = TransitiveLinkBuff:new("LightningAttack", 0.2, 150, 8, 1, UnitForce.kEnemy, false, PL.kTeslaRay)
a:setAppendBuff(id)
id = AL:add2(a)

a = AttackBuffMakerPas:new("LightningAttack", 0.20, id, false, 1.0, 0.0, 0)
AL.kLightningAttack = AL:add(a)

-- StrikeBack
a = StunBuff:new("StunBuff", "Stun", 1, false)
id = AL:add2(a)

a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 1.00, 0.0, Ability.kOnDamagedDoneTrigger, true)
a:setAppendBuff(id)
id = AL:add2(a)

a = StrikeBackPas:new("StrikeBack", 100, 0.10, id)
AL.kStrikeBack = AL:add(a)

-- TransitiveAttack
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.50, 0.0, Ability.kOnAttackTargetTrigger, true)
id = AL:add2(a)

a = TransitiveLinkBuff:new("TransitiveAttack", 0.0, 200.0, 5, 150, UnitForce.kEnemy, false, PL.kLumberjackProy)
a:setAppendBuff(id)
id = AL:add2(a)

a = AttackBuffMakerPas:new("TransitiveAttack", 0.50, id, false, 1.0, 0.0, 0)
AL.kTransitiveAttack = AL:add(a)

-- ChangeAttributeAttack
a = ChangeAttributeBuff:new("ChangeAttributeAttackBuff", "ChangeAttributeAttack", 0.5, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = AttackBuffMakerPas:new("ChangeAttributeAttack", 0.05, id, true, 1.0, 0.0, 0)
AL.kChangeAttributeAttack = AL:add(a)

-- SpeedUpAttack
a = ChangeAttributeBuff:new("SpeedUpAttackBuff", "SpeedUpAttack", 10.0, true, 0.0, 0.0, 0.2, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = AttackBuffMakerPas:new("SpeedUpAttack", 0.5, id, true, 1.0, 0.0, 0)
AL.kSpeedUpAttack = AL:add(a)

-- BleedAttack
a = EffectBuff:new(EL.kBleedingRedBig, true)
id = AL:add2(a)

a = ChangeHpBuff:new("BA.BleedingBuff", "Bleeding", 5.0, true, 0.2, -0.001, -10.0, 0.0, -1.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = AttackBuffMakerPas:new("BleedAttack", 0.20, id, false, 1.0, 0.0, 0)
AL.kBleedAttack = AL:add(a)

-- AttractAura
a = DamageBuff:new("dmg", AttackValue.kMagical, 0.0, 0.5, 0.0)
id = AL:add2(a)

a = AttractBuff:new("Attract", 1, 10.0)
a:setAppendBuff(id)
id = AL:add2(a)

a = AuraPas:new("AttractAura", 0.5, id, 150, UnitForce.kEnemy, false)
AL.kAttractAura = AL:add(a)

-- AttackedSpeedUp
a = ChangeAttributeBuff:new("ASU.CAB", "ChangeAttribute", 15.0, true, 0.0, 0.0, 0.01, 0.0, 0.01, 0.0, 0.10, 0.0, 0.0, 0.0)
id = AL:add2(a)

a = AttackedBuffMakerPas:new(id, true, 1.00)
AL.kAttackedSpeedUp = AL:add(a)