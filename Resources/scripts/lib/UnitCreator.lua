if __UNIT_CREATOR__ then return end
__UNIT_CREATOR__ = true

include("Common.lua")


UnitCreator = class()
function UnitCreator:ctor()
	
	-- id
	self.id = UL.kSoldier
	
	-- name
	self.nm = "Unnamed"
	
	-- level
	self.l = 1
	
	-- max hp
	self.hp = 10
	
	-- move speed
	self.ms = 50
	
	-- hostility range
	self.hr = 400
	
	-- force
	self.force = {--[[index: force index; ally: ally mask]]}
	self.force.index = 0
	self.force.ally = 0
	
	-- attack
	self.atk = {--[[t: type; v: value; i: interval]]}
	self.atk.t = AttackValue.kPhysical
	self.atk.v = 10.0
	self.atk.i = 1.5
	self.atk.r = 0.15
	
	-- armor
	self.amr = {--[[t: type; v: value]]}
	self.amr.t = ArmorValue.kHeavy
	self.amr.v = 0.0
	
	-- ability
	self.act = {--[[array of active ability]]}
	self.pas = {--[[array of passive ability]]}
	
	-- reward
	self.reward = {--[[g: gold; e: exp]]}
	self.reward.g = 10
	self.reward.e = 10
	
	-- AI(Optional)
	self.ai = nil
	
	-- LevelUpdate(Optional)
	self.lu = nil
end

function UnitCreator:createUnit()
	local u = createUnit(self.id)
	
	u:setName(self.nm)
	
	u:setMaxLevel(self.l)
	u:setLevel(self.l)
	
	u:setMaxHp(self.hp)

	u:setBaseMoveSpeed(self.ms)

	u:setHostilityRange(self.hr)

	u:setForceByIndex(self.force.index)
	u:setAlly(self.force.ally)
	
	local atk = u:getAttackAbility()
    atk:setBaseAttack(self.atk.t, self.atk.v)
    atk:setBaseAttackInterval(self.atk.i)
	atk:setAttackValueRandomRange(self.atk.r)
	
	u:setBaseArmor(self.amr.t, self.amr.v)
	
	for _, act in ipairs(self.act) do
		u:addActiveAbility(act)
	end
	
	for _, pas in ipairs(self.pas) do
		u:addPassiveAbility(pas)
	end
	
	u:setRewardGold(self.reward.g)
	u:setRewardExp(self.reward.e)
	
	if self.ai then
		u:setAI(self.ai)
	end
	
	if self.lu then
		u:setLevelUpdate(self.lu)
	end
	
	return u
end
