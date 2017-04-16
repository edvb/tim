math.randomseed(os.time())
math.random(); math.random(); math.random()

local m8ball = {
	"It is certain",
	"It is decidedly so",
	"Without a doubt",
	"Yes, definitely",
	"You may rely on it",
	"As I see it, yes",
	"Most likely",
	"Outlook good",
	"Yes",
	"Signs point to yes",
	"Reply hazy try again",
	"Ask again later",
	"Better not tell you now",
	"Cannot predict now",
	"Concentrate and ask again",
	"Don't count on it",
	"My reply is no",
	"My sources say no",
	"Outlook not so good",
	"Very doubtful"
}

if args:find("coin") then
	return math.random(0,1) == 1 and "heads" or "tails"
elseif args:find("from %d to %d") then
	num1, num2 = args:match("number from (.-) to (.-)$")
	return math.random(num1, num2)
elseif args:find("between %d and %d") then
	num1, num2 = args:match("between (.-) and (.-)$")
	return math.random(num1, num2)
elseif args:find("should") then
	return m8ball[math.random(1,#m8ball)]
end
