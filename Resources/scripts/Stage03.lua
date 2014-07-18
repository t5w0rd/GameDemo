include("Init.lua")


-- control unit
local hero = nil

local ucs = {}

uc = UnitCreator:new()
uc.id = UL.kBarracks
uc.nm = "Barracks"
uc.hp = 5000
uc.ms = 80
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 200.0
uc.atk.i = 1.5
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 15.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kArcher
uc.nm = "Archer"
uc.hp = 3500
uc.ms = 90
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 180.0
uc.atk.i = 1.4
uc.atk.r = 0.15
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 10.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kThor
uc.nm = "Thor"
uc.hp = 5500
uc.ms = 75
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 240.0
uc.atk.i = 1.8
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 12.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kMalik
uc.nm = "Malik"
uc.hp = 5300
uc.ms = 85
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 200.0
uc.atk.i = 1.6
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 10.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kFrost
uc.nm = "Frost"
uc.hp = 3000
uc.ms = 70
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 180.0
uc.atk.i = 2.0
uc.atk.r = 0.20
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 26.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kMage
uc.nm = "Mage"
uc.hp = 3200
uc.ms = 75
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 170.0
uc.atk.i = 1.8
uc.atk.r = 0.20
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 24.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kArtillery
uc.nm = "Artillery"
uc.hp = 3400
uc.ms = 85
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 230.0
uc.atk.i = 1.6
uc.atk.r = 0.50
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 9.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kElemental
uc.nm = "Elemental"
uc.hp = 2500
uc.ms = 85
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 260.0
uc.atk.i = 1.4
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 38.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kOni
uc.nm = "Oni"
uc.hp = 5500
uc.ms = 75
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 240.0
uc.atk.i = 1.6
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 12.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kViking
uc.nm = "Viking"
uc.hp = 5800
uc.ms = 85
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 230.0
uc.atk.i = 1.5
uc.atk.r = 0.50
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 5.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kLumberjack
uc.nm = "Lumberjack"
uc.hp = 6000
uc.ms = 60
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 150.0
uc.atk.i = 2.0
uc.atk.r = 0.50
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 35.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kAlric
uc.nm = "Alric"
uc.hp = 4000
uc.ms = 88
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 220.0
uc.atk.i = 1.4
uc.atk.r = 0.20
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 11.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = SUL.kPriest
uc.nm = "Priest"
uc.hp = 3000
uc.ms = 60
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 100.0
uc.atk.i = 2.0
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 5.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
table.insert(ucs, uc)


Stage03 = class(Battle)
function Stage03:onInit()
	local hero = initForHero()
	--hero:setLevel(20)
	hero:addExp(10000)
	hero:setGhost(true)
	hero:setAlly(2 ^ 0 + 2 ^ 1 + 2 ^ 2)
	
	local u, uc, us, id
	local n = math.random(5)
	
	us = {}
	for i = 1, n do
		repeat
			id = math.random(#ucs)
		until not us[id]
		us[id] = true
		
		uc = ucs[id]
		u = uc:createUnit()
		u:setAI(LuaAI:new())
		addAbilitiesForUnit(u, uc.id)
		u:setHostilityRange(1000)
		u:setForceByIndex(0)
		u:setAlly(2 ^ 0 + 2 ^ 2)
		u:setPosition(300 + math.random(-50, 50), 500 + math.random(-50, 50))
	end
	
	us = {}
	for i = 1, n do
		repeat
			id = math.random(#ucs)
		until not us[id]
		us[id] = true
		
		uc = ucs[id]
		u = uc:createUnit()
		u:setAI(LuaAI:new())
		addAbilitiesForUnit(u, uc.id)
		u:setHostilityRange(1000)
		u:setForceByIndex(1)
		u:setAlly(2 ^ 1 + 2 ^ 2)
		u:setPosition(1200 + math.random(-50, 50), 500 + math.random(-50, 50))
	end
	setControlUnit(u)
end

function Stage03:onTick(dt)
	
end

Stage03:new():run()
