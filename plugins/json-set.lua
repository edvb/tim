local json = require "json"

local myarg = { }
for s in string.gmatch(args, "%S+") do
   myarg[#myarg+1] = s
end
local file = myarg[1]
local var  = myarg[2]
local val  = myarg[3]

local f = io.open(file, "rb")
local js = f:read("*all")
f:close()
local t = json.decode(js)
t[var] = val

f = io.open(file, "w")
f:write(json.encode(t))
f:close()

return "Setting " .. var .. " to " .. val
