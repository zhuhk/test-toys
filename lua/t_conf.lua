print("white:", white.r, white.g, white.b);
print("red:", red.r, red.g, red.b);

--print(os.getenv("DISPLAY"));

if os.getenv("DISPLAY") == "0.0" then
    background = white;
--    width = 10;
--    height = 30;
else
    width = 3;

    height = 4;

    height = width *3 + 5;
    background = {r=127, b=110};
end

function f(x,y)
    return (x^2 * y);
end

