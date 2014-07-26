if __WORLD__ then return end
__WORLD__ = true

include("Common.lua")
include("lib/LevelUpdate.lua")


World = class()
function World:ctor()
end

function World:onHeroInit()
	local hero = createUnit(UD.heroes[1].id)
	hero:setLevelUpdate(HeroLevelUpdate:new())
	hero:setMaxLevel(20)
	hero:addExp(UD.heroes[1].exp)
	hero:setForceByIndex(0)
	hero:setPosition(1024, 768)
	setPortrait(1, hero)
	setCtrlUnit(hero)
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
	return hero
end

function World:onInit()
end

function World:onTick(dt)
end

function World:onUnitDead(u)
end

World.DEFAULT = World:new()
World.running = World.DEFAULT

function World:run()
	World.running = self
	
	onWorldInit = function()
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
