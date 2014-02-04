require("os");

luadir = require("luadir");

for fname in luadir.open("./") do
    print(fname);
end
for i=1,10000 do
    for fname in luadir.open("./") do
    end
end

print("---------------------");
d = luadir.dir("./");
for k,v in pairs(d) do
    print(v);
end

--[[
os.execute("sleep 2");
for fname in luadir.open("./") do
    print(fname);
end
os.execute("sleep 2");
for fname in luadir.open("./") do
    print(fname);
end
--]]

