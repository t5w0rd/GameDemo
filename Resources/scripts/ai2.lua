
LuaSimpleAI = class(UnitAI)
function LuaSimpleAI:ctor()
	self:sctor()
end
function LuaSimpleAI:onUnitTick(unit, dt)
end
function LuaSimpleAI:tryCastSkill(unit, skill, target)
	if unit:isDoingOr(Unit.kObstinate) then
		return
	end
	--t = unit:getAttackingTarget()
	a = unit:getActiveAbility(skillName)
	if a and t and not a:isCoolingDown() and not unit:isDoingCastingAction() then
		unit:setCastTarget(target)
	end
end

function LuaSimpleAI:setHpAI(unit, count)
	if count > 5 then
		count = 0
		unit:setHp(unit:getMaxHp())
		unit:addBattleTip("Full~~~~", "", 32, 0, 0, 0)
	end
	return count
end

function LuaSimpleAI:hitAndRunAI(unit)
	t = unit:getAttackingTarget()
	
	if unit:getDistance(t) < 500 then
	end
	
end


LuaAIWarrior = class(LuaSimpleAI)
function LuaAIWarrior:ctor()
	self:sctor()
	self.timeCount = 0
end

function LuaAIWarrior:onUnitTick(unit, dt)
	--self.timeCount = self.timeCount + dt
	--log(string.format("%f",self.timeCount))
	
	--self.timeCount = self:setHpAI(unit, self.timeCount)
	
	t = unit:getAttackingTarget()
	a = unit:getActiveAbility("ThrowHammer")
	if not a then
		unit:addBattleTip("no throwhammer", "", 32, 0, 0, 0)
	end
	if a and t and not unit:isDoingCastingAction() and not a:isCoolingDown() then
		unit:setCastTarget(t:getId())
		unit:castSpell(a)
		unit:addBattleTip("ThrowHammer", "", 32, 0, 0, 0)
	end
	
	a = unit:getActiveAbility("ThunderCap")
	if a and t and unit:getDistance(t) < 100 and not unit:isDoingCastingAction() and not a:isCoolingDown() then
		unit:setCastTarget()
		unit:castSpell(a)
		unit:addBattleTip("Thunder Cap", "", 32, 0, 0, 0)
	end
	
	a = unit:getActiveAbility("SpeedUp")
	if not a then
		unit:addBattleTip("no speedup", "", 32, 0, 0, 0)
	end
	if a and t and not unit:isDoingCastingAction() and not a:isCoolingDown() then
		if unit:getDistance(t) < 100 then
			unit:setCastTarget()
			unit:castSpell(a)
			unit:addBattleTip("Speed Up", "", 32, 0, 0, 0)
		end
	end
	
end

