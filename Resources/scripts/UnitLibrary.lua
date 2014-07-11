if __UNIT_LIBRARY__ then return end
__UNIT_LIBRARY__ = true

include("GameCommon.lua")

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
loadAnimation("Sprites/WizardProy/move", "Sprites/WizardProy/move", 0.1)
loadAnimation("Sprites/WizardProy/die", "Sprites/WizardProy/die", 0.1)
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
loadAnimation("Sprites/PriestBolt/move", "Sprites/PriestBolt/move", 0.1)
loadAnimation("Sprites/PriestBolt/die", "Sprites/PriestBolt/die", 0.1)
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

-- Priest
loadAnimation("Sprites/Priest/move", "Sprites/Priest/move", 0.1)
loadAnimation("Sprites/Priest/die", "Sprites/Priest/die", 0.1)
loadAnimation("Sprites/Priest/act1", "Sprites/Priest/act1", 0.08)
loadAnimation("Sprites/Priest/act2", "Sprites/Priest/act2", 0.08)
loadAnimation("Sprites/Priest/act3", "Sprites/Priest/act3", 0.08)
loadAnimation("Sprites/Priest/act4", "Sprites/Priest/act4", 0.08)
loadAnimation("Sprites/Priest/act5", "Sprites/Priest/act5", 0.08)
loadAnimation("Sprites/Priest/act6", "Sprites/Priest/act6", 0.05)
loadAnimation("Sprites/Priest/act7", "Sprites/Priest/act7", 0.05)
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
atk = AttackAct:new("¹¥»÷", 1.75, AttackValue.kMagical, 50.0, 0.15, 0.0, 200.0, false, Sprite.kAniAct1)
atk:setTemplateProjectile(SPL.kPriestBolt)
u:addActiveAbility(atk)
u:setBaseMoveSpeed(50.0)
u:setHostilityRange(300.0)
SUL.kPriest = SUL.add(u)

-- Orb
loadAnimation("Sprites/VoodooProy/move", "Sprites/VoodooProy/move", 0.1)
loadAnimation("Sprites/VoodooProy/die", "Sprites/VoodooProy/die", 0.1)
sp = Sprite:new("VoodooProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "Orb")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
SUL.kOrb = SUL.add(u)

-- Orb2
loadAnimation("Sprites/AlienProy/move", "Sprites/AlienProy/move", 0.1)
loadAnimation("Sprites/AlienProy/die", "Sprites/AlienProy/die", 0.1)
sp = Sprite:new("AlienProy")
sp:prepareFrame(Sprite.kFrmDefault, "default")
sp:prepareAnimation(Sprite.kAniMove, "move", -1)
sp:prepareAnimation(Sprite.kAniDie, "die", 0)
u = Unit:new(sp, "Orb2")
u:setGeometry(16.0, 16.0, 0.5, 0.5, 0, 0);
u:setFixed()
SUL.kOrb2 = SUL.add(u)



