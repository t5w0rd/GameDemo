if __WORLD_COMMON__ then return end
__WORLD_COMMON__ = true

include("Common.lua")
include("Extension.lua")
include("lib/World.lua")
include("lib/Rush.lua")
include("lib/AbilityLibrary.lua")
include("lib/AI.lua")
include("lib/UnitGroup.lua")
include("lib/UnitCreator.lua")



function addAbilitiesForUnit(u, id)
	if id == UL.kMage then
		u:addActiveAbility(AL.kGravitySurf)
		u:addActiveAbility(AL.kMageRain, 3)
		u:addActiveAbility(AL.kKnockBack)
		
	elseif id == UL.kFrost then
		u:addActiveAbility(AL.kSnowStorm)
		u:addActiveAbility(AL.kKnockBack)
		u:addPassiveAbility(AL.kSlowDownAttack)
		
	elseif id == UL.kArcher then
		u:addActiveAbility(AL.kArrowWind)
		u:addActiveAbility(AL.kSpeedUp2)
		u:addPassiveAbility(AL.kCriticalAttack, 1)
		
	elseif id == UL.kElemental then
		u:addActiveAbility(AL.kSerialExplode)
		u:addActiveAbility(AL.kBouncingFireBall)
		u:addPassiveAbility(AL.kArmorBreakAttack)
		u:addPassiveAbility(AL.kAttackedSpeedUp)
		
	elseif id == ULx.kPriest then
		u:addActiveAbility(AL.kHealWind)
		u:addActiveAbility(AL.kSweetDew)
		
	elseif id == UL.kArtillery then
		u:addActiveAbility(AL.kCutter)
		u:addActiveAbility(AL.kShotGun)
		u:addPassiveAbility(AL.kKnockBackAttack)
		u:addPassiveAbility(AL.kChangeAttributeAttack)
		
	elseif id == UL.kLumberjack then
		u:addActiveAbility(AL.kMissileStorm)
		u:addPassiveAbility(AL.kStrikeBack)
		u:addPassiveAbility(AL.kTransitiveAttack)
		
	elseif id == UL.kThor then
		u:addActiveAbility(AL.kThunderHammer2)
		u:addPassiveAbility(AL.kLightningAttack)
		u:addPassiveAbility(AL.kStunAttack)
		
	elseif id == UL.kAlric then
		u:addActiveAbility(AL.kMultiSlash, 3)
		u:addActiveAbility(AL.kFastMoveToBack)
		
	elseif id == UL.kMalik then
		u:addActiveAbility(AL.kThunderCap)
		u:addPassiveAbility(AL.kSpeedUpAttack)
		u:addPassiveAbility(AL.kDoubleAttack)
		u:addPassiveAbility(AL.kStunAttack)
		u:addPassiveAbility(AL.kThunderCapAttack)
		
	elseif id == UL.kBarracks then
		u:addActiveAbility(AL.kShieldBash)
		u:addActiveAbility(AL.kArmorUpCry)
		u:addActiveAbility(AL.kSpeedUpCry)
		u:addPassiveAbility(AL.kEvadeHeal)
		
	elseif id == UL.kViking then
		u:addActiveAbility(AL.kSpeedUp)
		u:addActiveAbility(AL.kChargeJump)
		u:addPassiveAbility(AL.kBerserkerBlood)
		u:addPassiveAbility(AL.kVampireAttack)
		
	elseif id == UL.kOni then
		u:addActiveAbility(AL.kSwordWave)
		u:addPassiveAbility(AL.kDamageIncreaceAttack)
		u:addPassiveAbility(AL.kBleedAttack)
		u:addPassiveAbility(AL.kSplashAttack)
		
	end
end

