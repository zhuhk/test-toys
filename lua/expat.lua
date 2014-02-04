require("luaexpat");

s = { count=0, cstart=0, cend=0, cdata=0, cattr=0, };
function clearT(T)
    for k,v in pairs(T) do
	T[k]=0
    end
end

function f_start1( parser, tagname)
    io.write("+ ", string.rep(" ",s.count), tagname);
    io.write("\n");

    s.count = s.count + 1;
    s.cstart = s.cstart + 1;
end
function f_start( parser, tagname, attrs)
    io.write(string.rep("  ",s.count), "+ ", tagname);
    if attrs then
	for k,v in pairs(attrs) do
	    io.write(" ", k, "=", v);
	    s.cattr = s.cattr + 1;
	end
    end
    
    io.write("\n");

    s.count = s.count + 1;
    s.cstart = s.cstart + 1;
end

function f_end(parser, tagname)
    s.count = s.count -1;
    s.cend = s.cend +1;
    io.write(string.rep("  ", s.count), "- ", tagname, "\n");
end


callbacks = {
    StartElement = f_start,
    EndElement = f_end,
    CharacterData = function(parser, data)
	if string.find(data,"%w+") then
	    data = string.gsub(data,"^%s+","");
	    data = string.gsub(data,"%s+$","");
	    io.write(string.rep("  ", s.count), "* ", data, "\n");
	    s.cdata = s.cdata+1;
	end
    end
}

lines = {
    '<doc>',
    '<to a1="v1" a2="v2">  aa  data </to>',
    '<to><yes/></to>',
    '<to><yes/></to>',
    '<to a1="v1" a2="v2"> data </to>',
    '<to a3="1"> <yes>Y</yes> </to>',
    '</doc>'
}

local lxp = require("luaexpat");

p = lxp.new(callbacks);
for i=1,#lines do
    p:parse(lines[i]);
end
p:parse();
p:close();

print();
for k,v in pairs(s) do
 print(k,v);
end

print("---------------------\n");
clearT(s);
p = lxp.new(callbacks);
for i=1,#lines do
    p.parse(p,lines[i])
end
p.parse(p);
p.close(p);

print();
for k,v in pairs(s) do
 print(k,v);
end
