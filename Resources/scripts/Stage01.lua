include("Init.lua")


-- control unit
local hero = nil

local uc = nil
uc = UnitCreator:new()
uc.nm = "T_T"
uc.hp = 220
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 5.0
uc.atk.i = 1.0
uc.arm.t = ArmorValue.kHeavy
uc.arm.v = 0.0
uc.reward.g = 5
uc.reward.e = 5


uc = UnitCreator:new()
uc.id = SUL.kPriest
uc.nm = "T_T"
uc.hp = 2200
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 205.0
uc.atk.i = 1.0
uc.arm.t = ArmorValue.kHeavy
uc.arm.v = 100.0
uc.reward.g = 500
uc.reward.e = 500


Stage01 = class(Battle)

function Stage01:onInit()
	hero = initForHero()
	--hero:setLevel(1)
	hero:addPassiveAbility(AL.kChangeAttributeAttack:getId())
	
	self.u = uc:createUnit()
	self.u:addPassiveAbility(AL.kRebirth:getId())
	self.u:setPosition(500, 800)
	
	self.u = uc:createUnit()
	self.u:addPassiveAbility(AL.kRebirth:getId())
	self.u:setPosition(500, 500)
end

function Stage01:onTick(dt)
	if self.u and self.u:isDead() then
		endWithVictory(math.random(1, 3))
	end
end

Stage01:new():run()
