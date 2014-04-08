LuaAI = class(UnitAI)
function LuaAI:ctor()
    self:sctor()
end

function LuaAI:onUnitTick(unit, dt)
    a = unit:getActiveAbility("ThunderCap")
    if not a then
        return
    end
    
    if unit:isDoingOr(Unit.kObstinate) then
        return
    end
    
    t = unit:getAttackingTarget()
    if t and unit:getDistance(t) < 100 and not unit:isDoingCastingAction() and not a:isCoolingDown() then
        unit:setCastTarget()
        unit:castSpell(a)
    end
end

function LuaAI:onUnitAbilityReady(unit, ability)
    if ability:getId() ~= unit:getAttackAbility():getId() then
        unit:addBattleTip(ability:getName(), "", 32, 0, 0, 0)
    end
end
