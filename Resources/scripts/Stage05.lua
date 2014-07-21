include("Init.lua")


Stage05 = class(Battle)
function Stage05:onInit()
	local stages = loadValue("StageMap.tbl")
	print(1)
end

function Stage05:onTick(dt)
end

Stage05:new():run()
