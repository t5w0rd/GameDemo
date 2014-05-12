LuaAI = class(UnitAI)
function LuaAI:ctor()
    self:sctor()
    self.stunned = false
    self.t = nil
    self.atk = true
end

function LuaAI:onUnitTick(unit, dt)
    atk = unit:getAttackAbility()
    if unit:getBuffAbility("Stun") then
        self.stunned = true
    end
    
    su = unit:getBuffAbility("SpeedUp2")
    if su then
        --return
    end
    
    t = unit:getAttackingTarget()
    if t then
        tsu = t:getBuffAbility("SpeedUp")
        self.t = t:getId()
    end
    a = unit:getActiveAbility("Cutter")
    if t and a and not a:isCoolingDown() then
        local x0, y0 = t:getPosition()
        local x1, y1 = unit:getPosition()
        local x, y = getForwardPoint(x1, y1, x0, y0, 100)
        unit:castSpellWithTargetPoint(x, y, a)
    end
    
    a = unit:getActiveAbility("KnockBack")
    if t and a and unit:getDistance(t) < 100 and not a:isCoolingDown() then
        unit:castSpellWithoutTarget(a)
    end
    
    a = unit:getActiveAbility("Reflect")
    if a and not unit:isDoingCastingAction() and not a:isCoolingDown() then
        unit:castSpellWithoutTarget(a)
    end
    
    if t and atk and (atk:isCoolingDown()) then
        local x0, y0 = t:getPosition()
        local x1, y1 = unit:getPosition()
        local dis = atk:getCastRange()
        if x1 > 1500 or x1 < 500 then
            dis = -400
        end

        local x, y = getForwardPoint(x0, y0, x1, y1, dis)
        unit:move(x, y + math.random(-50, 50))
        self.atk = false
    end
    
    if not self.atk and (self.t and not getUnit(self.t)) then
        unit:stop()
        self.t = nil
    end
    
    if not self.atk and not t and atk and not atk:isCoolingDown() then
        unit:castSpellWithTargetUnit(self.t, atk)
        self.atk = true
    end
    
    if unit:isDoingOr(Unit.kObstinate) then
        return
    end
    
    if not t then
        return
    end
    
    a = unit:getActiveAbility("SpeedUp2")
    if a and not unit:isDoingCastingAction() and not a:isCoolingDown() and unit:getDistance(t) < 400 and self.stunned then
        unit:castSpellWithoutTarget(a)
    end
    
    if unit:getHp() / unit:getRealMaxHp() < 0.5 and not unit:getBuffAbility("TowerHeal") then
        a = unit:getActiveAbility("SpeedUp2")
        if a and not unit:isDoingCastingAction() and not a:isCoolingDown() then
            unit:castSpellWithoutTarget(a)
        end
        
        if not unit:isDoingCastingAction() then
            unit:move(100, 500)
        end
    end
end

function LuaAI:onUnitAbilityReady(unit, ability)
    if ability:getId() ~= unit:getAttackAbility():getId() then
        unit:addBattleTip(ability:getName(), "fonts/Comic Book.fnt", 32, 0, 0, 0)
    end
end
