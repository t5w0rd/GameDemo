if __COMMON__ then return end
__COMMON__ = true

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

function getDirectionPoint(x0, y0, a, dis)
    return x0 + math.cos(-a) * dis, y0 + math.sin(a) * dis
end

function getForwardPoint(x0, y0, x1, y1, dis)
    local a = math.atan2(y1 - y0, x1 - x0)
    return getDirectionPoint(x0, y0, a, dis)
end



INFINITE = -1

