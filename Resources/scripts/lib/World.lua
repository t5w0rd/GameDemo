if __WORLD__ then return end
__WORLD__ = true

include("Common.lua")
include("lib/LevelUpdate.lua")
include("lib/AI.lua")


World = class()
function World:ctor()
end

function World:onHeroInit()
	local useNum = UD.heroesUseNum
	local mHero = nil
	for i = 1, useNum do
		for _, h in ipairs(UD.heroes) do
			if i == h.order then
				local hero = createUnit(h.id)
				--hero:setLevelUpdate(HeroLevelUpdate:new())
				
				h.wid = hero:getId()
				if i == 1 then
					mHero = hero
					hero:setLevelUpdate(HeroLevelUpdate:new())
					setCtrlUnit(hero)
				else
					hero:setAI(LuaAI:new())
					addAbilitiesForUnit(hero, ULx.kPriest)
					hero:setLevelUpdate(HeroLevelUpdate2:new())
				end
				
				hero:setMaxLevel(20)
				hero:addExp(h.exp)
				hero:setForceByIndex(0)
				hero:setPosition(1024, 768)
				setPortrait(i, hero)
			end
		end
	end
	
	--hero:setAlly(2 ^ 0 + 2 ^ 2)
	
	--local a = TeleportAct:new("TP", 1.0, Sprite.kAniAct6, Sprite.kAniAct7)
	--a:setImageName("UI/Ability/Ability03.png")
	--hero:addActiveAbility(a)
	--hero:addActiveAbility(AL.kBuffMaker:getId())
	--hero:addActiveAbility(AL.kChangeAttack:getId())
	--hero:addActiveAbility(AL.kFastMoveToBack:getId())
	--hero:addActiveAbility(AL.kMissileStorm:getId())
	--hero:addActiveAbility(AL.kSwordWave:getId())
	--hero:addActiveAbility(AL.kBouncingFireBall:getId())
	--hero:addActiveAbility(AL.kArrowWind:getId())
	return mHero
end

function World:onInit()
end

function World:onTick(dt)
end

function World:onUnitDead(u)
	local us = getUnits(nil, true)
	local ru = {}  -- reward units
	for _, uu in ipairs(us) do
		if not uu:isDead() and uu:isMyEnemy(u) and uu:getDistance(u) < 400 then
			table.insert(ru, uu)
		end
	end
	
	local numReward = #ru
	if numReward == 0 then
		return
	end
	
	local rg = randValue(u:getRewardGold(), 0.5) * 1 / (numReward + 0)
	local re = randValue(u:getRewardExp(), 0.5) * 2 / (numReward + 1)
	
	for _, uu in ipairs(ru) do
		local killer = u:getKiller()
		if killer and killer:getId() == uu:getId() then
			uu:addGold(rg)
			if uu:getForce() == 2 ^ 0 then
				uu:addBattleTip(string.format("+%d Gold", rg), nil, 18, 255, 247, 53)
			end
		end
		
		if uu:canIncreaseExp() then
			uu:addExp(re)
		end
	end
end

function World:updateHeroUD()
	for _, h in ipairs(UD.heroes) do
		if h.order > 0 then
			local hero = getUnit(h.wid)
			if hero then
				h.exp = hero:getExp()
			end
		end
	end
end

World.DEFAULT = World:new()
World.running = World.DEFAULT

function World:run()
	World.running = self
	
	onWorldInit = function()
		UD = loadValue(F("save.dat"))
		showDebug(false)
		math.randomseed(os.time())
		setHero(World.running:onHeroInit())
		World.running:onInit()
	end
	
	onWorldTick = function(dt)
		World.running:onTick(dt)
	end
	
	onUnitDead = function(u)
		World.running:onUnitDead(u)
	end
end
