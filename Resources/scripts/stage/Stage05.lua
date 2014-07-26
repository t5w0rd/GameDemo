include("WorldCommon.lua")


Stage05 = class(World)
function Stage05:onHeroInit()
	return nil -- 取消默认的主角英雄
end

function Stage05:onInit()
	local me = createUnit(UL.kViking)  -- 创建一个单位，模型采用UL.kViking
	me:setPosition(800, 600)  -- 设置单位的位置
	me:setName("sw0rd")  -- 设置名字
	me:sayf("Hey, guys! I'm %s!", me:getName())  -- 说一句欠揍的话

	setPortrait(1, me)  -- 将头像设置到1号位上，当然嫌长得丑也可以不设置头像。。
	setCtrlUnit(me)  -- 现在你可以操控这个单位了
	
end

function Stage05:onTick(dt)
end

Stage05:new():run()
