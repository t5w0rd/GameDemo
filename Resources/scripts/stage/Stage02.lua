include("WorldCommon.lua")


-- 出场单位数据表
local ucs = {}

uc = UnitCreator:new()
uc.id = UL.kSoldier
uc.nm = "Soldier"
uc.hp = 4000
uc.ms = 80
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 200.0
uc.atk.i = 1.5
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 20.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal}
table.insert(ucs, uc)


Stage02 = class(World)
function Stage02:onInit()
	local me = getHero()
	me:addExp(5000)
	
	self.mss = Mission:new()
	
	local pth = UnitPath:new()
	pth:addPoint(100, 500)
	pth:addPoint(1900, 500)
	retainMRObj(pth)
	
	self.mss:addPath(pth)
	
	local r = self.mss:addNewRound()
	
	local rush = Rush:new(1)
	rush:addUnit(ucs[1], 5, 2)
	
	self.mss:addRush(r, rush)
	
end

function Stage02:onTick(dt)
	local n = self.mss:rushCount()
	for i = 1, n do
		local uc, code = self.mss:step(i, dt)
		if not uc then
			return
		end
		local u = uc:createUnit()
		local pth = self.mss:pathOfRush(i)
		u:setPosition(pth:getFirstPoint())
		u:moveAlongPath(pth, false)
	end
	
	
	do return end
	if not getUnit(tower1) then
		stageVictory(math.random(1, 3))
	end
	
	if not getUnit(tower2) then
		stageDefeat()
	end
end

Stage02:new():run()
