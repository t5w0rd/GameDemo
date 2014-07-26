if __COMMON__ then return end
__COMMON__ = true


-- Unit Library
UL = {}
UL.BASE_ID = 0x1
UL.kMalik = UL.BASE_ID + 0
UL.kMage = UL.BASE_ID + 1
UL.kBarracks = UL.BASE_ID + 2
UL.kArcher = UL.BASE_ID + 3
UL.kFrost = UL.BASE_ID + 4
UL.kArtillery = UL.BASE_ID + 5
UL.kElemental = UL.BASE_ID + 6
UL.kOni = UL.BASE_ID + 7
UL.kThor = UL.BASE_ID + 8
UL.kViking = UL.BASE_ID + 9
UL.kLumberjack = UL.BASE_ID + 10
UL.kAlric = UL.BASE_ID + 11
UL.kAlien = UL.BASE_ID + 12
UL.kBeastMaster = UL.BASE_ID + 13
UL.kCrabman = UL.BASE_ID + 14
UL.kGiant = UL.BASE_ID + 15
UL.kMinotaur = UL.BASE_ID + 16
UL.kMirage = UL.BASE_ID + 17
UL.kPirate = UL.BASE_ID + 18
UL.kPriest = UL.BASE_ID + 19
UL.kVhelsing = UL.BASE_ID + 20
UL.kSoldier = UL.BASE_ID + 21
UL.kTemplar = UL.BASE_ID + 22
UL.kBarrack = UL.BASE_ID + 23
UL.kTemplars = UL.BASE_ID + 24
UL.kArcane = UL.BASE_ID + 25
UL.kTesla = UL.BASE_ID + 26
UL.kWizard = UL.BASE_ID + 27

-- Projectile Library
PL = {}
PL.BASE_ID = 0x1
PL.kMageBolt = PL.BASE_ID + 0
PL.kFrostBolt = PL.BASE_ID + 1
PL.kArcherArrow = PL.BASE_ID + 2
PL.kLightning = PL.BASE_ID + 3
PL.kThorHammer = PL.BASE_ID + 4
PL.kLumberjackProy = PL.BASE_ID + 5
PL.kAlienProy = PL.BASE_ID + 6
PL.kVoodooProy = PL.BASE_ID + 7
PL.kGiantProy = PL.BASE_ID + 8
PL.kMirageProy = PL.BASE_ID + 9
PL.kKungFuProy = PL.BASE_ID + 10
PL.kPirateProy = PL.BASE_ID + 11
PL.kPriestBolt = PL.BASE_ID + 12
PL.kWizardBolt = PL.BASE_ID + 13
PL.kWizardLightning = PL.BASE_ID + 14
PL.kWizardProy = PL.BASE_ID + 15
PL.kArcaneRay = PL.BASE_ID + 16
PL.kArcaneRay2 = PL.BASE_ID + 17
PL.kTeslaRay = PL.BASE_ID + 18
PL.kArcherArrow2 = PL.BASE_ID + 19


function table.copy(st)
    local tab = {}
    st = st or {}
    for k, v in pairs(st) do
        if type(v) ~= "table" then
            tab[k] = v
        else
            tab[k] = table.copy(v)
        end
    end
    setmetatable(tab, getmetatable(st))
    return tab
end

function table.changevalue(t, key, delta)
	if not t[key] then
		t[key] = delta
	else
		t[key] = t[key] + delta
	end
end

function getDirectionPoint(x0, y0, a, dis)
    return x0 + math.cos(-a) * dis, y0 + math.sin(a) * dis
end

function getForwardPoint(x0, y0, x1, y1, dis)
    local a = math.atan2(y1 - y0, x1 - x0)
    return getDirectionPoint(x0, y0, a, dis)
end



INFINITE = -1

