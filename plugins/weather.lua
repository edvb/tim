local http = require "socket.http"
local json = require "json"

if not args:find("weather") then
	return nil
end

function urlencode(str)
	if (str) then
		str = string.gsub(str, "\n", "\r\n")
		str = string.gsub(str, "([^%w ])", function (c) return string.format ("%%%02X", string.byte(c)) end)
		str = string.gsub(str, " ", "+")
	end
	return str
end

local f = io.open(get_path("data/memory.json"), "rb")
local js = f:read("*all")
f:close()
local t = json.decode(js)

units = t["units"]
location = t["location"]

local url = "https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22" .. urlencode(location) .. "%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys"

local js = http.request(url)
local t = json.decode(js)
local w = t["query"]["results"]["channel"]

local temp      = w["item"]["condition"]["temp"]
local tempunits = w["units"]["temperature"]
local city      = w["location"]["city"]
local region    = w["location"]["region"]
local country   = w["location"]["country"]

local place
if country == "United States" or country == "United Kingdom" then
	place = city .. region
else
	place = city .. ", " .. country
end

return "It is " .. temp .. " " .. tempunits .. " out today in " .. place
