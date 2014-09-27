if __LEVEL_UPDATE__ then return end
__LEVEL_UPDATE__ = true

include("Common.lua")
include("Extension.lua")


HeroLevelUpdate = class(LevelUpdate)

function HeroLevelUpdate:calcExp(lv)
    if lv == 0 then
        return 0
    end

    return lv * lv * 9 + lv * 3 + 8;
end

function HeroLevelUpdate:calcMaxHp(lv)
	--return 50 + (lv - 1) * 15
	return 50 + (lv - 1) * 300
end

function HeroLevelUpdate:calcMoveSpeed(lv)
	return 70.0 + (lv - 1) * 0.8
end

function HeroLevelUpdate:calcAttackValue(lv)
	--return 10.0 + (lv - 1) * 3.6
	return 10.0 + (lv - 1) * 7.8
end

function HeroLevelUpdate:calcAttackSpeed(lv)
	if lv == 0 then
		return 0.0
	end
	return 0.0 + (lv - 1) * 0.01
end

function HeroLevelUpdate:calcArmorValue(lv)
	--return 2.0 + (lv - 1) * 0.5
	return 2.0 + (lv - 1) * 1.0
end

function HeroLevelUpdate:onLevelChanged(u, change)
    cast(u, Unit)
	
	local lv = u:getLevel()

    u:addBuffAbility(ChangeHpBuff:new("LevelUpHeal", "LevelUpHeal", 5.0, false, 0.2, 0.02, 0.0))
    u:addBuffAbility(ReflectBuff:new("Reflect", "Reflect", 5.0))
    
    u:setMaxHp(self:calcMaxHp(lv))
    
    u:setBaseMoveSpeed(self:calcMoveSpeed(lv))

    local atk = u:getAttackAbility()
    local a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + self:calcAttackSpeed(lv) - self:calcAttackSpeed(lv - 1), b)

    local t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, self:calcAttackValue(lv))

    t, v = u:getBaseArmor()
    u:setBaseArmor(t, self:calcArmorValue(lv))
    
	--[[]]
	if lv == 10 then
        --u:addPassiveAbility(AL.kTransitiveAttack)
    elseif lv == 11 then
        u:addPassiveAbility(AL.kLightningAttack)
    elseif lv == 12 then
        u:addPassiveAbility(AL.kDamageIncreaceAttack)
    elseif lv == 13 then
        --u:addPassiveAbility(AL.kPressureBombAttack)
    elseif lv == 14 then
        u:addPassiveAbility(AL.kRebirth)
    elseif lv == 15 then
        u:addPassiveAbility(AL.kChangeAttributeAttack)
    elseif lv == 16 then
        u:addPassiveAbility(AL.kCriticalAttack)
    elseif lv == 17 then
        u:addPassiveAbility(AL.kThrowHammerAttack)
    elseif lv == 18 then
        u:addPassiveAbility(AL.kBerserkerBlood)
    elseif lv == 19 then
        u:addPassiveAbility(AL.kCutterAttack)
    elseif lv == 20 then
        u:addPassiveAbility(AL.kStrikeBack)
	elseif lv == 50 then
		u:addPassiveAbility(AL.kPressureBombAttack)
    end
	--[[]]
	World.running:updateHeroUD()
	saveGame()
end


HeroLevelUpdate2 = class(LevelUpdate)

function HeroLevelUpdate2:calcExp(lv)
    if lv == 0 then
        return 0
    end

    return lv * lv * 9 + lv * 3 + 8;
end

function HeroLevelUpdate2:calcMaxHp(lv)
	--return 50 + (lv - 1) * 15
	return 50 + (lv - 1) * 300
end

function HeroLevelUpdate2:calcMoveSpeed(lv)
	return 70.0 + (lv - 1) * 0.8
end

function HeroLevelUpdate2:calcAttackValue(lv)
	--return 10.0 + (lv - 1) * 3.6
	return 10.0 + (lv - 1) * 7.8
end

function HeroLevelUpdate2:calcAttackSpeed(lv)
	if lv == 0 then
		return 0.0
	end
	return 0.0 + (lv - 1) * 0.01
end

function HeroLevelUpdate2:calcArmorValue(lv)
	--return 2.0 + (lv - 1) * 0.5
	return 2.0 + (lv - 1) * 1.0
end

function HeroLevelUpdate2:onLevelChanged(u, change)
    cast(u, Unit)
	
	local lv = u:getLevel()

    u:addBuffAbility(ChangeHpBuff:new("LevelUpHeal", "LevelUpHeal", 5.0, false, 0.2, 0.02, 0.0))
    u:addBuffAbility(ReflectBuff:new("Reflect", "Reflect", 5.0))
    
    u:setMaxHp(self:calcMaxHp(lv))
    
    u:setBaseMoveSpeed(self:calcMoveSpeed(lv))

    local atk = u:getAttackAbility()
    local a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + self:calcAttackSpeed(lv) - self:calcAttackSpeed(lv - 1), b)

    local t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, self:calcAttackValue(lv))

    t, v = u:getBaseArmor()
    u:setBaseArmor(t, self:calcArmorValue(lv))
    
	World.running:updateHeroUD()
	saveGame()
end
