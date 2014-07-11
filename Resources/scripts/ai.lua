LuaAI = class(UnitAI)
function LuaAI:ctor()
    self:sctor()
    self.acts = {}

    self.targetId = nil
    self.canAttack = true
end

function LuaAI:tryCastSpell(unit, name, condFunc, params)
    local a = unit:getActiveAbility(name)
    if not a or a:isDoingCastingAction() or a:isCoolingDown() then
        return false
    end
    
	local castType = a:getCastTarget()
    if castType == CommandTarget.kNoTarget then
        if condFunc(unit, a, params) then
            unit:castSpellWithoutTarget(a)
            return true
        end
    elseif castType == CommandTarget.kUnitTarget then
        local t = condFunc(unit, a, params)
        if t then
            unit:castSpellWithUnitTarget(a, t)
            return true
        end
    elseif castType == CommandTarget.kPointTarget then
        local x, y = condFunc(unit, a, params)
        if x and y then
            unit:castSpellWithPointTarget(a, x, y)
            return true
        end
    end
    return false
end

function LuaAI:onUnitTick(unit, dt)
    local atk = unit:getAttackAbility()
    local t = unit:getAttackingTarget()
    if t then
        self.targetId = t:getId()
    end
    
    if t and atk and (atk:isCoolingDown()) then
		--攻击cd中
        local x0, y0 = t:getPosition()
        local x1, y1 = unit:getPosition()
        local dis = atk:getCastRange()
        if x1 > 1500 or x1 < 500 then
            dis = -400
        end

        local x, y = getForwardPoint(x0, y0, x1, y1, dis)
        unit:move(x, y + math.random(-50, 50))
        self.canAttack = false
    end
    
	if not self.canAttack then
		--如果不攻击
		if self.targetId and not getUnit(self.targetId) then
			-- 当前目标已不存在
			unit:stop()
			self.targetId = nil
		end
	end
	
    
    
    if not t and atk and not atk:isCoolingDown() then
        --如果不能攻击，但是 存在可攻击目标 且攻击没有cd
		unit:castSpellWithTargetUnit(atk, self.targetId)
        self.canAttack = true
    end
    
    if unit:isDoingOr(Unit.kObstinate) then
        return
    end
    
    if not t then
        return
    end
end

function LuaAI:onUnitAbilityReady(unit, ability)
    if ability:getId() ~= unit:getAttackAbility():getId() then
        unit:addBattleTip(ability:getName(), "fonts/Comic Book.fnt", 32, 0, 0, 0)
    end
end
