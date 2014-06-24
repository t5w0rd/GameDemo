--if __LEVELUPDATE__ then return end
__LEVELUPDATE__ = true

HeroLevelUpdate = class(LevelUpdate)
function HeroLevelUpdate:onChangeLevel(u, change)
    cast(u, Unit)

    u:addBuffAbility(ChangeHpBuff:new("LevelUpHeal", "LevelUpHeal", 5.0, false, 0.2, 0.02, 0.0))
    u:addBuffAbility(ReflectBuff:new("Reflect", "Reflect", 5.0))
    
    u:setMaxHp(u:getMaxHp() + 300)
    
    u:setBaseMoveSpeed(u:getBaseMoveSpeed() + 1.5)

    local atk = u:getAttackAbility()
    local a, b = atk:getExAttackSpeed()
    atk:setExAttackSpeed(a + 0.01, b)

    local t, v = atk:getBaseAttack()
    atk:setBaseAttack(t, v + 7.8)

    t, v = u:getBaseArmor()
    u:setBaseArmor(t, v + 1)
    
    local lvl = u:getLevel()
    
	if lvl == 10 then
        u:addPassiveAbility(lib.TransitiveAttack)
    elseif lvl == 11 then
        u:addPassiveAbility(lib.LightningAttack)
    elseif lvl == 12 then
        u:addPassiveAbility(lib.DamageIncreaceAttack)
    elseif lvl == 13 then
        u:addPassiveAbility(lib.PressureBombAttack)
    elseif lvl == 14 then
        u:addPassiveAbility(lib.Rebirth)
    elseif lvl == 15 then
        me:addActiveAbility(lib.ChargeJump)
    elseif lvl == 16 then
        u:addPassiveAbility(lib.CriticalAttack)
    elseif lvl == 17 then
        me:addPassiveAbility(lib.ThrowHammerAttack)
    elseif lvl == 18 then
        me:addPassiveAbility(lib.BerserkerBlood)
    elseif lvl == 19 then
        me:addPassiveAbility(lib.CutterAttack)
    elseif lvl == 20 then
        me:addActiveAbility(lib.MultiSlash)
    end

    saveUserData()
end
