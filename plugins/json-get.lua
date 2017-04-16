local json = require "json"

local myarg = { }
for s in string.gmatch(args, "%S+") do
   myarg[#myarg+1] = s
end
local file = myarg[1]
local var = myarg[2]

local f = io.open(file, "rb")
local js = f:read("*all")
f:close()
local t = json.decode(js)

return t[var]
