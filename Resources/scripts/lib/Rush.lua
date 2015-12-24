if __RUSH__ then return end
__RUSH__ = true

Rush = class()
Rush.kNoUnit = -3
Rush.kWaiting = -4

function Rush:ctor(pathIndex)
	self.pathIndex = pathIndex
	self.pos = 1
	self.arrPos = 1
	self.pass = 0.0
	self.unitInfos = {}
end

function Rush:addUnit(uc, num, delay)
	local unitInfo = {}
	unitInfo.uc = uc
	unitInfo.num = num
	unitInfo.delay = delay
	table.insert(self.unitInfos, unitInfo)
end

function Rush:step(dt)
	local n = #self.unitInfos
	if self.arrPos > n then
		return nil, Rush.kNoUnit
	end

	self.pass = self.pass + dt
	
	local unitInfo = self.unitInfos[self.arrPos]
	
	if self.pass > unitInfo.delay then
		self.pass = self.pass - unitInfo.delay
		self.pos = self.pos + 1
		if self.pos > unitInfo.num then
			self.pos = 1
		end
		
		if self.pos == 1 then
			self.arrPos = self.arrPos + 1
		end
		return unitInfo.uc
	end
	
	return nil, Rush.kWaiting
end


Mission = class()
Mission.kNoRound = -1
Mission.kInvalidIndex = -2

function Mission:ctor()
	self.rpos = 1
	self.rounds = {}
	self.paths = {}
end

function Mission:step(rushIndex, dt)
	local r = self:curRound()
	if r < 0 then
		return nil, Mission.kNoRound
	end
	
	if rushIndex > #self.rounds then
		return nil, Mission.kInvalidIndex
	end
	
	return self.rounds[r][rushIndex]:step(dt)
end

function Mission:curRound()
	if self.rpos > #self.rounds then
		return -1
	end
	
	return self.rpos
end

function Mission:nextRound()
	if self.rpos > #self.rounds then
		return false
	end
	
	return true
end

function Mission:roundCount()
	return #self.rounds
end

function Mission:rushCount()
	local r = self:curRound()
	if r < 0 then
		return -1
	end
	return #self.rounds[r]
end

function Mission:addPath(path)
	table.insert(self.paths, path)
	return #self.paths
end

function Mission:pathOfRush(rushIndex)
	local r = self:curRound()
	if r < 0 then
		return nil
	end
	
	if rushIndex <= #self.rounds[r] then
		local rush = self.rounds[r][rushIndex]
		if rush.pathIndex <= #self.paths then
			return self.paths[rush.pathIndex]
		end
	end
	
	return nil
end

function Mission:addNewRound()
	table.insert(self.rounds, {})
	return #self.rounds
end

function Mission:addRush(roundIndex, rush)
	if roundIndex <= #self.rounds then
		table.insert(self.rounds[roundIndex], rush)
	end
end

function Mission:isRoundEnd()
	return self.rpos > #self.rounds
end

function Mission:isLastRound()
	return self.rpos == #self.rounds
end