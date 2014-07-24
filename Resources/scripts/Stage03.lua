include("Init.lua")


-- control unit
local hero = nil

local ucs = {}

uc = UnitCreator:new()
uc.id = UL.kBarracks
uc.nm = "Barracks"
uc.hp = 4000
uc.ms = 80
uc.force.index = 1
uc.atk.t = AttackValue.kPhysical
uc.atk.v = 200.0
uc.atk.i = 1.5
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kHeavy
uc.amr.v = 20.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
uc.pos = 100
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kArcher
uc.nm = "Archer"
uc.hp = 2500
uc.ms = 100
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
uc.pos = 300
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kThor
uc.nm = "Thor"
uc.hp = 4500
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
uc.pos = 80
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kMalik
uc.nm = "Malik"
uc.hp = 4300
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
uc.pos = 100
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kFrost
uc.nm = "Frost"
uc.hp = 2000
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
uc.pos = 200
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kMage
uc.nm = "Mage"
uc.hp = 2200
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
uc.pos = 180
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kArtillery
uc.nm = "Artillery"
uc.hp = 2700
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
uc.pos = 280
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kElemental
uc.nm = "Elemental"
uc.hp = 2000
uc.ms = 85
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 260.0
uc.atk.i = 1.4
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 50.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
uc.pos = 200
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kOni
uc.nm = "Oni"
uc.hp = 4500
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
uc.pos = 80
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kViking
uc.nm = "Viking"
uc.hp = 4000
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
uc.pos = 50
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kLumberjack
uc.nm = "Lumberjack"
uc.hp = 5000
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
uc.pos = 50
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = UL.kAlric
uc.nm = "Alric"
uc.hp = 3000
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
uc.pos = 100
table.insert(ucs, uc)

uc = UnitCreator:new()
uc.id = SUL.kPriest
uc.nm = "Priest"
uc.hp = 2000
uc.ms = 60
uc.force.index = 1
uc.atk.t = AttackValue.kMagical
uc.atk.v = 80.0
uc.atk.i = 2.0
uc.atk.r = 0.10
uc.amr.t = ArmorValue.kCrystal
uc.amr.v = 5.0
uc.reward.g = 10
uc.reward.e = 10
uc.pas = {AL.kAutoHeal:getId()}
uc.pos = 250
table.insert(ucs, uc)


Stage03 = class(Battle)

function Stage03:spawnTeam(ucs, number, template, control, x, y)
	local us = {}
	local id
	local force
	
	local maxAngle = 0
	local dtAngle = 0
	if number > 1 then
		maxAngle = math.pi / 2
		dtAngle = maxAngle / (number - 1)
	end
	
	for i = 1, number do
		repeat
			id = math.random(#ucs)
		until ( #template < i and not us[id]) or (#template >= i and template[i] == ucs[id].id)
		us[id] = true
		
		local uc = ucs[id]
		local u = uc:createUnit()
		u:setMaxLevel(20)
		u:setLevel(20)
		u:setAI(LuaAI:new(true))
		addAbilitiesForUnit(u, uc.id)
		u:setHostilityRange(200)
		
		local as = u:getActiveAbilities()
		for _, a in ipairs(as) do
			if a:getCoolDown() > 0 then
				a:coolDown(math.min(1.00, 20 / a:getCoolDown()))
			end
		end
		
		if control then
			u:setForceByIndex(0)
			u:setAlly(2 ^ 0 + 2 ^ 2)
			u:setPosition(getDirectionPoint(x, y, -maxAngle / 2 + dtAngle * (i - 1) + math.pi, uc.pos))
			--u:setPosition(x + uc.pos, y + math.random(-200, 200))
			table.insert(self.f0, uc.nm)
			setPortrait(i, u)
		else
			u:setForceByIndex(1)
			u:setAlly(2 ^ 1 + 2 ^ 2)
			u:setPosition(getDirectionPoint(x, y, -maxAngle / 2 + dtAngle * (i - 1), uc.pos))
			--u:setPosition(x - uc.pos, y + math.random(-200, 200))
			table.insert(self.f1, uc.nm)
		end
		
		table.changevalue(self.fight, uc.nm, 1)

	end
end

function Stage03:onHeroInit()
	return nil
end

function Stage03:onInit()
	--msgBox("test测试", "title标题")
	--setGameSpeed(2.0)
	showDebug(true)
	
	self.over = false
	self.fight = {}
	self.f0 = {}
	self.f1 = {}
	self.dur = 0.0
	self.timeout = 120.0
	
	local u, uc, us, id
	local n
	us = {}
	
	local tm = {}
	--tm[#tm + 1] = {UL.kViking, SUL.kPriest}
	--tm[#tm + 1] = {UL.kAlric}
	--tm[#tm + 1] = {UL.kMage, UL.kArtillery, UL.kLumberjack, UL.kFrost}
	--tm[#tm + 1] = {UL.kMage, UL.kViking, UL.kThor, UL.kFrost, UL.kAlric}
	--tm[#tm + 1] = {UL.kThor, UL.kElemental, UL.kViking, SUL.kPriest}
	--tm[#tm + 1] = {UL.kElemental, UL.kViking, UL.kMage, UL.kArtillery}
	--tm[#tm + 1] = {UL.kBarracks, UL.kMalik, UL.kViking, UL.kThor}
	
	if #tm > 0 then
		local rnd = math.random(1, #tm)
		team = tm[rnd]
	else
		team = {}
	end
	
	if #team == 0 then
		n = math.random(5)
	else
		n = #team
	end
	
	self:spawnTeam(ucs, n, team, true, 500, 700)
	self:spawnTeam(ucs, n, {}, false, 1000, 700)
	--self:spawnTeam(ucs, 1, {UL.kLumberjack}, true, 500, 700)
	--self:spawnTeam(ucs, 1, {UL.kFrost}, false, 1000, 700)
	
	self.res = loadValue(F("battle.tbl"))
	if not self.res then
		self.res = {}
		self.res.fight = {}
		self.res.win = {}
		self.res.survive = {}
		self.res.log = {}
	else
		-- 根据胜率排序
		local st = {}
		for nm, num in pairs(self.res.survive) do
			local ste = {}
			ste.nm = nm
			local win2 = self.res.win[nm]
			local fight2 = self.res.fight[nm]
			ste.st = self.res.win[nm] * 100.0 / self.res.fight[nm]
			table.insert(st, ste)
		end
		
		table.sort(st, function(el, el2)
			return el.st > el2.st
		end)
		
		local str = "\n============= Rank(Win) =============\n"
		for _, el in ipairs(st) do
			local strl = string.format("%s: %d/%d/%d win(%.3g%%) sur(%.3g%%)",
				el.nm,
				self.res.fight[el.nm],
				self.res.win[el.nm],
				self.res.survive[el.nm],
				self.res.win[el.nm] * 100.0 / self.res.fight[el.nm],
				self.res.survive[el.nm] * 100.0 / self.res.fight[el.nm])
			
			log("%s", strl)
			str = str .. strl .. "\n"
		end
		
		saveValue(F("rank.str"), str)
	end
end

function Stage03:onTick(dt)
	if self.over then
		return
	end
	
	self.dur = self.dur + dt
	
	if self.dur > self.timeout then
		self.over = true
		log("timeout!")
		restartStage()
		return
	end
	
	local us = getUnits(nil, true)
	local f0 = 0
	local f1 = 0
	for _, u in ipairs(us) do
		local f = u:getForce()
		if f == 2 ^ 0 then
			f0 = f0 + 1
		elseif f == 2 ^ 1 then
			f1 = f1 + 1
		end
	end
	
	if f0 == 0 or f1 == 0 then
		self.over = true
		
		if f0 > 0 or f1 > 0 then
			-- 统计胜场
			local win
			if f0 > 0 then
				win = self.f0
			else
				win = self.f1
			end
			
			for _, nm in ipairs(win) do
				table.changevalue(self.res.win, nm, 1)
			end
		end
		
		-- 统计存活
		for nm, num in pairs(self.fight) do
			table.changevalue(self.res.fight, nm, num)
		end
		
		local bl = {}
		us = getUnits()
	
		for _, u in ipairs(us) do
			local nm = u:getName()
			table.insert(bl, nm)
			table.changevalue(self.res.survive, nm, 1)
		end
		table.insert(self.res.log, bl)
		
		saveValue(F("battle.tbl"), self.res)
		
		log("the battle log has been updated! (%d)", #self.res.log)
		restartStage()
	end
end

Stage03:new():run()
