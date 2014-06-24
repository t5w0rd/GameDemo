function hpDelta(hero, target)
	local minDps = hero.dps * (1 - hero.r / 2)
	local maxDps = hero.dps * (1 + hero.r / 2)
	
	local res = {}
	res.dur = target.hp / hero.dps
	res.minDur = target.hp / maxDps
	res.maxDur = target.hp / minDps
	
	local x = (hero.l + target.l) / (hero.l + hero.l)
	--local x = 1
	res.hpDelta = -math.ceil(res.dur) * target.dps * x
	res.minHpDelta = -math.ceil(res.minDur) * target.dps * x
	res.maxHpDelta = -math.ceil(res.maxDur) * target.dps * x
	
	return res
end

function hpDeltaStr(hero, target)
	local res = hpDelta(hero, target)
	return string.format("%.2f (%.2f ~ %.2f) | %.2f%% (%.2f%% ~ %.2f%%) | %.2fs (%.2fs ~ %.2fs)", res.hpDelta, res.minHpDelta, res.maxHpDelta, res.hpDelta * 100 / hero.hp, res.minHpDelta * 100 / hero.hp, res.maxHpDelta * 100 / hero.hp, res.dur, res.minDur, res.maxDur)
end

function createHeroTable(heroBase, endLvl)
	local hero = {heroBase}
	for i = 2, endLvl do
		hero[i] = {}
		hero[i].l = hero[i - 1].l + 1
		hero[i].dps = hero[i - 1].dps + 3.6
		hero[i].hp = hero[i - 1].hp + 13
		hero[i].r = hero[i - 1].r
	end
	return hero
end

heroBase = {l = 1, dps = 10, hp = 50, r = 0.2}
heroLT = createHeroTable(heroBase, 10)

targetT = {
	{name = "Goblin", l = 1, dps = 5, hp = 22},
	{name = "Orc", l = 2, dps = 8, hp = 25},
	{name = "Boar", l = 3, dps = 10, hp = 30},
	{name = "Wolf", l = 4, dps = 15, hp = 40}
}


function printResult(heroTable, heroLevel, targetTable)
	for _, target in ipairs(targetTable) do
		print(string.format("Hero(Lv%d) vs %s(Lv%d) | %s", heroLevel, target.name, target.l, hpDeltaStr(heroTable[heroLevel], target)))
	end
end

printResult(heroLT, 1, targetT)
printResult(heroLT, 2, targetT)
printResult(heroLT, 3, targetT)
