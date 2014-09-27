if __LOAD_GAME__ then return end
__LOAD_GAME__ = true

include("Common.lua")


SAVE_VERSION = 1
-- Global UserData
UD = nil

function onLoadingUserData()
	UD = loadValue(F("save.dat"))
	if not UD then
		UD = {}
		UD.version = SAVE_VERSION
		
		UD.stages = {}
		
		UD.heroesUseNum = 0
		UD.heroes = {}
		UD.heroes[1] = {}
		UD.heroes[1].id = UL.kBarracks
		UD.heroes[1].exp = 0
		UD.heroes[1].order = 0
		UD.heroes[2] = {}
		UD.heroes[2].id = 0x101
		UD.heroes[2].exp = 0
		UD.heroes[2].order = 0
		
		
		-- for test
		local sm = loadValue(F("StageMap.tbl"))
		for i, _ in ipairs(sm) do
			local stage = {}
			stage.grade = math.random(1, 3)
			UD.stages[i] = stage
			setStageGrade(i, stage.grade)
		end
		
		saveGame()
	else
		for i, stage in ipairs(UD.stages) do
			setStageGrade(i, stage.grade)
		end
	end
end

function saveGame()
	saveValue(F("save.dat"), UD)
end

function stageVictory(grade)
	local idx = getCurStageIndex()
	local real = setStageGrade(idx, grade)
	if real then
		UD.stages[idx] = {}
		UD.stages[idx].grade = real
	end
	
	saveGame()
	endWithVictory(grade)
end

function stageDefeat()
	saveGame()
	endWithDefeat()
end

function F(name)
	return getWritablePath() .. "/" .. name
end

