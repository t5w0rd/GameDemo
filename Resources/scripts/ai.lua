if __AI__ then return end
__AI__ = true

-- ConditionFunctions
CF = {}

function CF.anytime(u, a)
	return true
end

function CF.checkEffectiveUnits(u, a)
	local r = a:getCastTargetRadius()
	local eff = a:getEffectiveTypeFlags()	
	local t = u:getNearestEffectiveUnitInRange(r, eff)
	
	return t ~= nil
end

function CF.checkRangeEnemies(u, a)
	local r = a:getCastTargetRadius()
	if r == 0 then
		local atk = u:getAttackAbility()
		r = atk:getCastRange() + 50
	end
	local t = u:getNearestEffectiveUnitInRange(r, UnitForce.kEnemy)
	
	return t ~= nil
end

function CF.getAttackingTargetUnit(u, a)
	return u:getAttackingTarget()
end

function CF.checkAttackingTargetUnit(u, a)
	local t = u:getAttackingTarget()
	if not t then
		return nil
	end
	
	if u:getTouchDistance(t) > a:getCastRange() then
		return nil
	end
	
	return t
end

function CF.getAttackingTargetUnitPoint(u, a)
	local t = u:getAttackingTarget()
	if not t then
		return nil, nil
	end
	
	return t:getPosition()
end

function CF.getNearestEffectiveUnit(u, a)
	local r = a:getCastRange()
	local eff = a:getEffectiveTypeFlags()
	local t = u:getNearestEffectiveUnitInRange(r, eff)
	
	return t
end

function CF.getNearestAttackRangeEnemy(u, a)
	local atk = u:getAttackAbility()
	local r = atk:getCastRange() + 50
	local eff = UnitForce.kEnemy
	local t = u:getNearestEffectiveUnitInRange(r, eff)
	
	return t
end

function CF.getNearestEffectiveUnitExceptSelf(u, a)
	local r = a:getCastRange()
	local eff = a:getEffectiveTypeFlags()
	local self = bit32.band(eff, UnitForce.kSelf)
	eff = bit32.band(eff, bit32.bnot(UnitForce.kSelf))
	
	local t = u:getNearestEffectiveUnitInRange(r, eff)
	if not t then
		if self then
			return u
		end
		return nil
	end
	return t
end

function CF.getNearestEffectiveUnitPoint(u, a)
	local r = a:getCastRange()
	local eff = a:getEffectiveTypeFlags()
	local t = u:getNearestEffectiveUnitInRange(r, eff)
	if not t then
		return nil, nil
	end
	
	return t:getPosition()
end

function CF.getNearestEffectiveInjuredUnit(u, a)
	return u:getNearestEffectiveInjuredUnitInRange(a:getCastRange(), a:getEffectiveTypeFlags(), 0.75, 0.0)
end

function CF.getNearestEffectiveInjuredUnitExceptSelf(u, a)
	local r = a:getCastRange()
	local eff = a:getEffectiveTypeFlags()
	local self = bit32.band(eff, UnitForce.kSelf)
	if self then
		eff = bit32.band(eff, bit32.bnot(UnitForce.kSelf))
	end
	
	local t = u:getNearestEffectiveInjuredUnitInRange(a:getCastRange(), eff, 0.75, 0.0)
	if not t then
		if self and u:getHp() < u:getRealMaxHp() * 0.75 then
			return u
		end
		return nil
	end
	
	return t
end

ACF = {}

-- NoTarget
ACF["英勇疾跃"] = CF.anytime
ACF["狂暴"] = CF.anytime
ACF["击退"] = CF.checkEffectiveUnits
ACF["雷霆震地"] = CF.checkEffectiveUnits
ACF["圣愈之风"] = CF.checkEffectiveUnits
ACF["守护者呐喊"] = CF.checkRangeEnemies
ACF["疾速"] = CF.checkRangeEnemies
ACF["逐风式"] = CF.checkRangeEnemies
ACF["幽灵背刺"] = CF.checkRangeEnemies
ACF["速射散弹枪"] = CF.checkRangeEnemies

-- PointTarget
ACF["汽油弹"] = CF.getNearestEffectiveUnitPoint
ACF["引力虚空"] = CF.getNearestEffectiveUnitPoint
ACF["魔法箭雨"] = CF.getNearestEffectiveUnitPoint
ACF["冰锥术"] = CF.getNearestEffectiveUnitPoint
ACF["连环爆轰"] = CF.getNearestEffectiveUnitPoint
ACF["导弹风暴"] = CF.getNearestEffectiveUnitPoint
ACF["箭矢狂风"] = CF.getNearestEffectiveUnitPoint

-- UnitTarget
ACF["风暴战锤"] = CF.checkAttackingTargetUnit
ACF["雷神之锤"] = CF.checkAttackingTargetUnit
ACF["盾击"] = CF.checkAttackingTargetUnit
ACF["爆裂之球"] = CF.getNearestEffectiveUnit
ACF["幻影斩击"] = CF.getNearestEffectiveUnit
ACF["治疗甘露"] = CF.getNearestEffectiveInjuredUnitExceptSelf


LuaAI = class(UnitAI)
function LuaAI:ctor()
    self:sctor()
    self.acts = {}

    self.targetId = nil  -- 锁定了的攻击目标
    self.hitAndRun = false  -- hitAndRun模式
end

function LuaAI:tryCastSpell(unit, a)
	if not a or a:isCoolingDown() then
        return false
    end
    
	local castType = a:getCastTargetType()
	local nm = a:getName()
	local cf = ACF[nm]
	if not cf then
		return false
	end
	
    if castType == CommandTarget.kNoTarget then
        if cf(unit, a) then
            unit:castSpellWithoutTarget(a)
            return true
        end
    elseif castType == CommandTarget.kUnitTarget then
        local t = cf(unit, a)
        if t then
            unit:castSpellWithTargetUnit(a, t)
            return true
        end
    elseif castType == CommandTarget.kPointTarget then
        local x, y = cf(unit, a)
        if x and y then
            unit:castSpellWithTargetPoint(a, x, y)
            return true
        end
    end
    return false
end

function LuaAI:onUnitTick(unit, dt)
	local atk = unit:getAttackAbility()
	local a = unit:getCastingActiveAbility()
	local casting = unit:isDoingCastingAction()
	if casting and (not a or not atk or a:getId() ~= atk:getId()) then
		return
	end
	
	if a and unit:isDoingAnd(Unit.kCasting + Unit.kObstinate) and atk and a:getId() ~= atk:getId() then
		-- 如果当前正在强制施法中，且处于施法的技能不是攻击
    	return
    end
	
	-- 尝试释放所有技能
    for _, a in pairs(self.acts) do
    	if (self:tryCastSpell(unit, a) == true) then
			return
		end
    end
	
	if casting then
		-- 处于施法动作的时候不执行AI
		return
	end
    
	if not atk then
		return
	end
	
    local t = unit:getAttackingTarget()
    if t then
        self.targetId = t:getId()
    end
    
	if atk:getCastRange() > 150 then
		-- 是远程英雄
		if t and atk:isCoolingDown() then
			-- 不在hitAndRun模式中，当前正在攻击一个目标，且攻击cd中，将进入hitAndRun模式
			local x0, y0 = t:getPosition()
			local x1, y1 = unit:getPosition()
			local dis = atk:getCastRange()
			if x1 > 1500 or x1 < 500 then
				dis = -400
			end
			--logf("hitAndRun")
			local x, y = getForwardPoint(x0, y0, x1, y1, dis)
			if x < 100 or x > 1900 or y < 100 or y > 1400 then
				x, y = getForwardPoint(x0, y0, x1, y1, -dis)
			end
			x = math.min(2000, math.max(0, x))
			y = math.min(1500, math.max(0, y))
			unit:move(x, y + math.random(-50, 50))
			self.hitAndRun = true
		end
		
		if self.hitAndRun and self.targetId then
			-- 正处于hitAndRun模式中
			local lastTarget = getUnit(self.targetId)
			if lastTarget then
				if not atk:isCoolingDown() then
					-- 且攻击没有cd
					unit:castSpellWithTargetUnit(atk, self.targetId, false)
					self.hitAndRun = false
				end
			else
				-- 之前的目标已不存在
				unit:stop()
				self.targetId = nil
				self.hitAndRun = false
			end
		end
    end
    
end

function LuaAI:onUnitAddActiveAbility(unit, a)
	table.insert(self.acts, a)
end

function LuaAI:onUnitAbilityReady(unit, ability)
    if ability:getId() ~= unit:getAttackAbility():getId() then
        --unit:addBattleTip(ability:getName(), "fonts/Comic Book.fnt", 32, 0, 0, 0)
    end
end
