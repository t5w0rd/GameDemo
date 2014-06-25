include("Init.lua")


-- control unit
local hero = nil

local uc = nil
uc = UnitCreator:new()
uc.name = "T_T"
uc.hp = 220
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 5.0
uc.atk.i = 1.0
uc.arm.t = ArmorValue.kHeavy
uc.arm.v = 0.0
uc.reward.g = 5
uc.reward.e = 5


Stage03 = class(Battle)

function Stage03:onInit()
	hero = initForHero()
	--hero:setLevel(1)
	
	local u = uc:createUnit()
	u:setPosition(500, 800)
end

function Stage03:onTick(dt)
end

Stage03:new():run()
