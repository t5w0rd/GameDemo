data = loadTableFromFile("save.dat")
x = data.heroes_num + 5
x = 0
y = data.hero.def[1]
z = data.hero.def["1"]

data[1] = 2
data[2] = 5
data[3] = false
--saveTableToFile(data, "ttt.dat")

ttt = loadTableFromFile("ttt.dat")

o = nil
