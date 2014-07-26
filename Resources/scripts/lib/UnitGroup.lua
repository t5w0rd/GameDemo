if __UNIT_GROUP__ then return end
__UNIT_GROUP__ = true


UnitGroup = class()
function UnitGroup:ctor()
	self.units = {}
end

function UnitGroup.getEffectiveUnitsInRange(x, y, unit, radius, forceFlags, func)
	local ug = UnitGroup:new()
	ug.units = getUnits(function (u)
		return unit:canEffect(u, forceFlags) and u:getTouchDistance(x, y) < radius and (not func or func(u))
	end)
	
	return ug
end

function UnitGroup:getRandomUnit()
	local num = #self.units
	if num == 0 then
		return nil
	end
	
	return self.units[math.random(num)]
end

function UnitGroup:addBuffAbility(buff, src, level)
	for _, u in ipairs(self.units) do
		u:addBuffAbility(buff, src, level)
	end
end

function UnitGroup:damaged(ad, src, mask)
	for _, u in ipairs(self.units) do
		u:damaged(ad, src, mask)
	end
end
