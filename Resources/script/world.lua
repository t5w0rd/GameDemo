function table.copy(st)
    local tab = {}
    st = st or {}
    for k, v in pairs(st) do
        if type(v) ~= "table" then
            tab[k] = v
        else
            tab[k] = copyTab(v)
        end
    end
    setmetatable(tab, getmetatable(st))
    return tab
end

CurseBuff = class(BuffAbility)
function CurseBuff:ctor(duration, stackable, damage, interval, multiply)
    self:sctor("curse", "Curse", duration, stackable)
    self.damage = damage
    self.interval = interval
    self.pass = 0
    self.multiply = multiply
    self.hp = 0
    self:setInterval(0.5)
end

function CurseBuff:onUnitAddAbility()
    o = self:getOwner()
    self.hp = o:getHp()
    o:addBattleTip("Curse", "Arial", 18, 255, 0, 255)
end

function CurseBuff:onUnitInterval()
    o = self:getOwner()
    local damage = 0
    self.pass = self.pass + 0.5
    if self.pass > self.interval then
        self.pass = 0
        damage = (self.hp - o:getHp()) * self.multiply
    end
    
    if damage < 0 then
        damage = 0
    end
    damage = damage + self.damage
    o:addBattleTip(string.format("-%d", damage), "Arial", 18, 255, 0, 0)
    o:setHp(o:getHp() - damage)
end

DefPas = class(PassiveAbility)
function DefPas:ctor(buff)
    self:sctor("root", "name", 0)
    self.buff = buff
    self:setTriggerFlags(Ability.kOnAttackTargetTrigger)
end

function DefPas:onUnitAddAbility()
    print("DefPas:onUnitAddAbility()")
end

function DefPas:onUnitAttackTarget(ad, target)
    o = self:getOwner()
    cclog("lua ad: %s", ad._p)
    --ad:setAttackValue(ad:getAttackValue() * 10)
    ad:addAttackBuff(self.buff, 1)
    o:addBattleTip(math.ceil(ad:getAttackValue()), "Arial", 18, 0, 0, 0)
    return ad
end

function onWorldInit()
	

    c = getControlUnit()
    local x, y = c:getPosition()
	cclog("p(%.1f, %.1f)", x, y);
    c:setPosition(x + 200, y)
	x, y = c:getPosition()
	cclog("p(%.1f, %.1f)", x, y);
    c:setMaxHp(2000)

	do return end
    
    s = Sprite:new("Malik")
    s:prepareFrame(Sprite.kFrmDefault, "default")
    s:prepareAnimation(Sprite.kAniMove, "move", -1)
    s:prepareAnimation(Sprite.kAniDie, "die", -1)
    s:prepareAnimation(Sprite.kAniAct1, "act1", 4)
    s:prepareAnimation(Sprite.kAniAct2, "act2", 4)
    s:setGeometry(7.0, 10.0, 0.5, 0.1125, 10.0, 10.0)
    u = Unit:new(s, "MK")

    u:setForceByIndex(3)
    u:setMaxHp(1375.0)
    u:setBaseArmor(0, 9.0)
    u:setBaseMoveSpeed(80.0)
    u:setPosition(500, 500)

    setControlUnit(u)

    a = StatusShowPas:new()
    u:addPassiveAbility(a)

    a = AttackAct:new("NormalAttack", 1.5, 0, 10, 0.15,   -3, 20, true, Sprite.kAniAct1, Sprite.kAniAct2)
    u:addActiveAbility(a)

    --do return end
    
    u:setExMoveSpeed(2, 0)
    
    a = CurseBuff:new(13, false, 10, 4, 20 / 100)
    id = addTemplateAbility(a)
    
    a = DefPas:new(id)
    u:addPassiveAbility(a) 
    
end

