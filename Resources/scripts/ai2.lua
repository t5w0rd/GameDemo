
LuaSimpleAI = class(UnitAI)
function LuaSimpleAI:ctor()
    self:sctor()
end
function LuaSimpleAI:onUnitTick(unit, dt)
end
function LuaSimpleAI:tryCastSkill(unit, skillName, castTarget, ...)   --distance, timeCount
    if unit:isDoingOr(Unit.kObstinate) then
        return
    end
    a = unit:getActiveAbility(skillName)
    if not a then
        return
    end
    argvs = {...}
    if #argvs == 0 then
        self:trySkill(unit, a, castTarget)
        return
    end
    if #argvs == 1 then
        distance = argvs[1]
        t = unit:getAttackingTarget()
        if  #argvs == 1 and t and distance and unit:getDistance(t) < distance then
            self:trySkill(unit, a, castTarget)
            return
        end
    end
end

function LuaSimpleAI:trySkill(unit, skill, castTarget)
    if skill and not skill:isCoolingDown() and not unit:isDoingCastingAction() then
        if castTarget then
            --logf("castTarget %d", castTarget)
            unit:castSpellWithTargetUnit(skill, castTarget)
        else
            unit:castSpellWithoutTarget(skill)
        end
        --unit:sayf("Taste my %s!", skill:getName())
    end
end

function LuaSimpleAI:setHpAI(unit, count)
    if count > 5 then
        count = 0
        unit:setHp(unit:getMaxHp())
        unit:addBattleTip("Full~~~~", "fonts/Comic Book.fnt", 32, 0, 0, 0)
    end
    return count
end

function LuaSimpleAI:hitAndRun(unit, runDistance)
    t = unit:getAttackingTarget()
    if t and unit:getDistance(t) < runDistance then
        x,y = unit:getPosition()
        unit:move(x + math.random(-100, 100), y + math.random(-100, 100))
    end
end


LuaAIWarrior = class(LuaSimpleAI)
function LuaAIWarrior:ctor()
    self:sctor()
    self.timeCount = 0
end

function LuaAIWarrior:onUnitTick(unit, dt)
    t = unit:getAttackingTarget()
    if t and t:getDistance(unit) < 200 then
        self:tryCastSkill(unit, "ThrowHammer", t)
    end
	if t and t:getHp() > 60 then
		self:tryCastSkill(unit, "ThunderCap", nil, 100)
		self:tryCastSkill(unit, "SpeedUp", nil, 100)
	end
    
    if not unit:isDoingCastingAction() and unit:getHp() / unit:getRealMaxHp() < 0.5 and not unit:getBuffAbility("TowerHeal") then
        unit:move(100, 500)
    end
    --self:hitAndRun(unit, 200)
    --self.timeCount = self.timeCount + dt
    --self.timeCount = self:setHpAI(unit, self.timeCount)
end

function LuaAIWarrior:onUnitDamagedDone(unit, dmg, src)
    --[[
	a = unit:getActiveAbility("ThunderCap")
    if a and not a:isCoolingDown() and src and unit:getDistance(src) < 100 then
        unit:castSpellWithoutTarget(a)
    end
	]]
end

