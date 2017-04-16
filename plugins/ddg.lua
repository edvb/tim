local http = require "socket.http"
local json = require "json"

function urlencode(str)
	if (str) then
		str = string.gsub(str, "\n", "\r\n")
		str = string.gsub(str, "([^%w ])", function (c) return string.format ("%%%02X", string.byte(c)) end)
		str = string.gsub(str, " ", "+")
	end
	return str
end

local urlq = urlencode(args)
local url = "http://api.duckduckgo.com/?q=" .. urlq .. "&format=json"

local js = http.request(url)

local t = json.decode(js)

tlist = {
	"Answer",
	"Definition",
	"AbstractText"
}

local ret = ""
for i,v in ipairs(tlist) do
	if (ret == "") then
		ret = t[v]
	end
end

if (ret == "") then
	local rett = {}
	for i,reltopic in ipairs(t["RelatedTopics"]) do
		if (reltopic["Topics"]) then
			for j,topic in ipairs(reltopic["Topics"]) do
				rett[#rett+1] = tostring(topic["Text"])
			end
		else
			rett[#rett+1] = tostring(reltopic["Text"])
		end
	end
	ret = table.concat(rett,"\n")
end

return ret
