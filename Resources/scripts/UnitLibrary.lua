if __UNIT_LIBRARY__ then return end
__UNIT_LIBRARY__ = true

include("GameCommon.lua")

--do return end

local sp, p, u, atk
-- static projectile library
SPL = {}
SPL.STEP = 1
SPL.id = SPL.STEP
function SPL.add(proj)
	local id = SPL.id
	addTemplateProjectile(SPL.id, proj)
	SPL.id = SPL.id + SPL.STEP
	return id
end

-- WizardProy
loadAnimations("WizardProy", {move = 0.1, die = 0.1})
sp = Sprite:new("WizardProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
p = Projectile:new(sp, "WizardProy")
p:setMoveSpeed(300.0)
p:setMaxHeightDelta(5.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
p:addFireSound("sounds/Effects/MageShot.mp3")
SPL.kWizardProy = SPL.add(p)

-- PriestBolt
loadAnimations("PriestBolt", {move = 0.1, die = 0.1})
sp = Sprite:new("PriestBolt")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
p = Projectile:new(sp, "PriestBolt")
p:setMoveSpeed(400.0)
p:setMaxHeightDelta(50.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
p:addFireSound("sounds/Effects/MageShot.mp3")
SPL.kPriestBolt = SPL.add(p)

-- VoodooProy
loadAnimations("VoodooProy", {move = 0.1, die = 0.1})
sp = Sprite:new("VoodooProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
p = Projectile:new(sp, "VoodooProy")
p:setMoveSpeed(500.0)
p:setMaxHeightDelta(0.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
p:addFireSound("sounds/Effects/MageShot.mp3")
SPL.kVoodooProy = SPL.add(p)

-- GreenWave
loadAnimations("GreenWave", {})
sp = Sprite:new("GreenWave")
sp:prepareFrame(Sprite.kFrmDefault, "default")
p = Projectile:new(sp, "GreenWave")
p:setMoveSpeed(500.0)
p:setMaxHeightDelta(0.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
--p:addFireSound("sounds/Effects/MageShot.mp3")
SPL.kGreenWave = SPL.add(p)

-- FireBall
loadAnimations("FireBall", {move = 0.1})
sp = Sprite:new("FireBall")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
p = Projectile:new(sp, "FireBall")
p:setMoveSpeed(400.0)
p:setMaxHeightDelta(0.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
--p:addFireSound("sounds/Effects/MageShot.mp3")
SPL.kFireBall = SPL.add(p)

-- Missile
loadAnimations("Missile", {move = 0.1})
sp = Sprite:new("Missile")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
p = Projectile:new(sp, "Missile")
p:setMoveSpeed(600.0)
p:setMaxHeightDelta(50.0)
p:setPenaltyFlags(Projectile.kOnDying)
p:setFireType(Projectile.kFireFollow)
--p:addFireSound("sounds/Effects/Sound_RocketLaunt.mp3")
SPL.kMissile = SPL.add(p)

-- static unit library
SUL = {}
SUL.STEP = 1
SUL.id = SUL.STEP
function SUL.add(unit)
	local id = SUL.id
	addTemplateUnit(SUL.id, unit)
	SUL.id = SUL.id + SUL.STEP
	return id
end

-- None
loadAnimations("None", {move = 1.0, die = 1.0})
sp = Sprite:new("None")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "None")
u:setGeometry(1.0, 1.0, 0.0, 0.0, 0.0, 0.0)
u:setMaxHp(1.0)
u:setGhost(true)
u:setBaseArmor(ArmorValue.kHoly, 0.0)
u:setBaseMoveSpeed(0.0)
u:setHostilityRange(0.0)
SUL.kNone = SUL.add(u)

-- Priest
loadAnimations("Priest", {move = 0.1, die = 0.1, act1 = 0.08, act2 = 0.08, act3 = 0.08, act4 = 0.08, act5 = 0.08, act6 = 0.08, act7 = 0.08})
sp = Sprite:new("Priest")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0, "sounds/Sprites/Priest/die/00.mp3")
sp:prepareAnimation(Sprite.kAniAct1, "act1", 6)
sp:prepareAnimation(Sprite.kAniAct2, "act2", 5)
sp:prepareAnimation(Sprite.kAniAct3, "act3", 8)
sp:prepareAnimation(Sprite.kAniAct4, "act4", 6)
sp:prepareAnimation(Sprite.kAniAct5, "act5", 7)
sp:prepareAnimation(Sprite.kAniAct6, "act6", 14)
sp:prepareAnimation(Sprite.kAniAct7, "act7", 7)
u = Unit:new(sp, "Priest")
u:setGeometry(23.0, 27.0, 78.0 / 156, 48.0 / 268, -18.0, 65.0)
u:addCtrlSound("sounds/Sprites/Priest/move/00.mp3", 2.011)
u:addCtrlSound("sounds/Sprites/Priest/move/01.mp3", 2.455)
u:addCtrlSound("sounds/Sprites/Priest/move/02.mp3", 1.880)
u:addCtrlSound("sounds/Sprites/Priest/move/03.mp3", 1.906)
u:addCtrlSound("sounds/Sprites/Priest/move/04.mp3", 1.410)
u:setMaxHp(500.0)
u:setBaseArmor(ArmorValue.kCrystal, 0.0)
atk = AttackAct:new("¹¥»÷", 1.75, AttackValue.kMagical, 20.0, 0.15, 0.0, 200.0, false, Sprite.kAniAct1)
atk:setTemplateProjectile(SPL.kPriestBolt)
u:addActiveAbility(atk)
u:setBaseMoveSpeed(50.0)
u:setHostilityRange(300.0)
SUL.kPriest = SUL.add(u)

-- Orb
sp = Sprite:new("VoodooProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "Orb")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
SUL.kOrb = SUL.add(u)

-- Orb2
loadAnimations("AlienProy", {move = 0.1, die = 0.1})
sp = Sprite:new("AlienProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "Orb2")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
SUL.kOrb2 = SUL.add(u)

-- BlackHole
loadAnimations("BlackHole", {move = 0.1, die = 0.1})
sp = Sprite:new("BlackHole")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "BlackHole")
u:setGeometry(0.0, 0.0, 0.5, 0.5, 0, 0);
u:setFixed()
SUL.kBlackHole = SUL.add(u)



