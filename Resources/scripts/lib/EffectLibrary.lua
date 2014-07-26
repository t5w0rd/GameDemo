if __EFFECT_LIBRARY__ then return end
__EFFECT_LIBRARY__ = true

include("lib/ObjectLibrary.lua")


EffectCreator = class()
function EffectCreator:ctor(epath, delay, ax, ay, sound)
	self.epath = epath
	self.delay = delay
	self.ax = ax
	self.ay = ay
	self.sound = sound
end

function EffectCreator:createEffectOnUnit(unit)
	local eff = Effect:new(self.epath, self.delay, self.ax, self.ay, self.sound, unit)
	local x, y = unit:getAnchorPointInPoints()
	eff:setPosition(x, y + unit:getHalfOfHeight())
	return eff
end

function EffectCreator:createEffectAtPosition(x, y, h)
	local eff = Effect:new(self.epath, self.delay, self.ax, self.ay, self.sound)
	eff:setLogicPosition(x, y)
	eff:setLogicHeight(h)
	return eff
end

EL = ObjectLibrary:new(1, function(self, id, obj)
	self.effs[id] = obj
end)
EL.effs = {}

function EL:createEffectOnUnit(id, unit)
	local ec = self.effs[id]
	if not ec then
		return nil
	end
	
	return ec:createEffectOnUnit(unit)
end

function EL:createEffectAtPosition(id, x, y, h)
	local ec = self.effs[id]
	if not ec then
		return nil
	end
	
	return ec:createEffectAtPosition(x, y, h)
end


local eff

eff = EffectCreator:new("Effects/Fire/Big", 0.1, 0.5, 50 / 92, nil)
EL.kFireBig = EL:add(eff)

eff = EffectCreator:new("Effects/Explosion/Normal", 0.05, 0.5, 42 / 168, "sounds/Effects/Sound_FireballHit.mp3")
EL.kExplosionNormal = EL:add(eff)

eff = EffectCreator:new("Effects/Explosion/Small", 0.08, 0.5, 10 / 92, nil)
EL.kExplosionSmall = EL:add(eff)

eff = EffectCreator:new("Effects/Explosion/Short", 0.08, 0.5, 40 / 132, "sounds/Effects/Sound_FireballHit.mp3")
EL.kExplosionShort = EL:add(eff)

eff = EffectCreator:new("Effects/ArmorBreak", 0.08, 0.5, 0.5, nil)
EL.kArmorBreak = EL:add(eff)

eff = EffectCreator:new("Effects/Burn", 0.05, 0.5, 22 / 92, nil)
EL.kBurn = EL:add(eff)

eff = EffectCreator:new("Effects/Rain/Bolt", 0.03, 0.5, 25 / 320, nil)
EL.kRainBolt = EL:add(eff)

eff = EffectCreator:new("Effects/Rain/Ice", 0.05, 0.5, 40 / 208, nil)
EL.kRainIce = EL:add(eff)

eff = EffectCreator:new("Effects/Rain/SharpIce", 0.05, 0.5, 40 / 172, nil)
EL.kRainSharpIce = EL:add(eff)

eff = EffectCreator:new("Effects/Bleeding/Red/Big", 0.05, 0.5, 60 / 116, nil)
EL.kBleedingRedBig = EL:add(eff)

eff = EffectCreator:new("Effects/EarthQuake", 0.05, 0.5, 25 / 132, nil)
EL.kEarthQuake = EL:add(eff)
