include("WorldCommon.lua")


-- control unit
local hero = nil

local ucs = {}
uc = UnitCreator:new()
uc.id = UL.kMage
uc.nm = "Enemy"
uc.hp = 2000
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 10.0
uc.atk.i = 1.5
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 10.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kBarracks
uc.nm = "Enemy"
uc.hp = 2000
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 10.0
uc.atk.i = 1.5
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 10.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kThor
uc.nm = "Ally"
uc.hp = 2000
uc.force.index = 0
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 10.0
uc.atk.i = 1.5
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 10.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoDamaged}
table.insert(ucs, uc)


Stage04 = class(World)
function Stage04:onInit()
	hero = getHero()
	--hero:setLevel(20)
	hero:addExp(10000)
	
	self.id = {}
	
	uc = UnitCreator:new()
	uc.id = UL.kArcane
	uc.nm = "Tower"
	uc.hp = 20000
	uc.force.index = 1
	uc.atk.t = AttackValue.kHoly
	uc.atk.v = 500.0
	uc.atk.i = 1.5
	uc.amr.t = ArmorValue.kWall
	uc.amr.v = 100.0
	uc.reward.g = 500
	uc.reward.e = 500
	local u = uc:createUnit()
	u:setFixed(true)
	u:setPosition(1500, 1000)
end

function Stage04:onTick(dt)
	for i, uc in ipairs(ucs) do
		if not self.id[i] or not getUnit(self.id[i]) then
			local u = uc:createUnit()
			u:setPosition(math.random(500, 1000), math.random(200, 800))
			self.id[i] = u:getId()
		end
	end
end

Stage04:new():run()
