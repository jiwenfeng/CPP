
--$Id: call_out.lua 106563 2010-01-12 02:50:51Z tony@NETEASE.COM $

--每个节点存储的内容为{Func, TimeOut, arg, next=?}
--目前采用单链表实现，RunCallOut很快，但insert, remove都很慢，将来可以考虑用二叉树来实现
local NORMAL = 0
local FREQUENCY = 1
TIME_OUT = "200722184107"


local function Log(...)
	local tbl = {...}
	local f = io.open("callout.log", "a")
	f:write(table.concat(tbl, ","))
	f:close()
end

function Trace(Obj, Func, StartTime)
	local EndTime = os.time()
	local Last = EndTime - StartTime
	if Last > 2 then
		Log("long run callout:", tostring(Obj.__FILE__), StartTime, Last, tostring(Func), debug.traceback())
	end
end


--用函数的upvalue来记录相关信息。
--返回timer的自增索引，而不是timer本身，可以防止同一个object内部重复删除/添加timer时可能造成的丢失情况。
local function CallOutByType(Obj, Func, TimeOut, CallType, ...)
	Obj.__AllTimers = rawget(Obj, "__AllTimers") or {}
	local Index
	local function RealFunc()
		if CallType ~= FREQUENCY then
			Obj.__AllTimers[Index] = nil --remove.
		end
		local StartTime = os.time()
		if type(Func) == "string" then
			assert(Obj[Func], Func)
			Obj[Func](unpack(arg))
		else
			Func(unpack(arg))
		end
		Trace(Obj, Func, StartTime)
	end

	TimeOut = TimeOut < 1 and 1 or TimeOut

	if CallType == FREQUENCY then
		Index = lengine.call_multi(RealFunc, TimeOut)
	else
		Index = lengine.call_once(RealFunc, TimeOut)
	end

	Obj.__AllTimers[Index] = true
	return Index
end

--删除指定timer.
function RemoveCallOut(Obj, TimerIndex)
	assert(Obj and TimerIndex)
	Obj.__AllTimers = rawget(Obj, "__AllTimers") or {}
	if not Obj.__AllTimers[TimerIndex] then
		return
	end
	Obj.__AllTimers[TimerIndex] = nil

	return lengine.rm_call(TimerIndex)
end

--删除指定对象的所有timer.
function RemoveAll(Obj)
	assert(Obj)
	local AllTimers = rawget(Obj, "__AllTimers")
	if not AllTimers then return end
	for TimerIndex, _ in pairs(AllTimers) do
		lengine.rm_call(TimerIndex)
	end

	Obj.__AllTimers = {}
end

--以某频率(second)定时运行
function ObjCallFre(Obj, Func, Freqency, ...)
	local TimerIndex = CallOutByType(Obj, Func, Freqency, FREQUENCY, ...)
	return TimerIndex
end
function CallFre(Func, Freqency, ...)
	local TimerIndex = CallOutByType(getfenv(2), Func, Freqency, FREQUENCY, ...)
	return TimerIndex
end

--延时呼叫
function CallOut(Func, TimeOut, ...)
	local TimerIndex = CallOutByType(getfenv(2), Func, TimeOut, NORMAL, ...)
	return TimerIndex
end
function ObjCallOut(Object, Func, TimeOut, ...)
	local TimerIndex= CallOutByType(Object, Func, TimeOut, NORMAL, ...)
	return TimerIndex
end
--在 Begin ～ End时段每隔Timeout 调用一次Fun
function CallOutInRange(Fun, Begin, End, Timeout, ...)
	local env = getfenv(2)
	local function _DoFun(...)
		Fun(...)
		if os.time() + Timeout < End then
			CallOutByType(env, _DoFun, Timeout, NORMAL, ...)
		end
	end
	local Now = os.time()
	if Now > Begin then
		if Now < End then
			_DoFun(...)
		end
	else
		CallOutByType(env, _DoFun, Begin - Now, NORMAL, ...)
	end
end

--指定某时间运行
function ObjCallAt(Obj, Func, DateTime, ...)
	local Time = DateTime
	if type(DateTime) == "string" then
		Time = TIME.sDate2Sec(DateTime)
	end
	local diff = Time - os.time()
	diff = diff < 1 and 1 or diff
	if diff >= 0 then --小于零的CallAt意味著一种错误。
		return ObjCallOut(Obj, Func, diff, ...)
	end
end
function CallAt(Func, DateTime, ...)
	return ObjCallAt(getfenv(2), Func, DateTime, ...)
end
function CallAtTime(Func, hour, min, sec, ...)
	local year, month, day = os.date("%Y"), os.date("%m"), os.date("%d")
	local tbl = {year = year, month = month, day = day, hour = hour, min = min, sec = sec}
	local time = os.time(tbl)
	if time >= os.time() then
		CallAt(Func, time, ...)
	else
		CallAt(Func, time + 24 * 60 * 60, ...)  --过了今天加24小时
	end
end
--等待一定时间后resume一个coroutine
function ResumeLater(Seconds)
	local CoObj = coroutine.running()
	assert(CoObj, "Not in Coroutine!!")
	local function RunCo(Co)
		if coroutine.status(Co) == "suspended" then
			coroutine.resume(Co, TIME_OUT)
		end
	end
	CallOut(RunCo, Seconds, CoObj)
	return coroutine.yield(CoObj)
end

--[[
hour    11
min     34
wday    3
day     23
month   1
year    2007
sec     35
yday    23
]]
local DAY = 60*60*24
local WEEK = DAY * 7
function RunAt (env, Period, func, ...)
	assert(env)
	--注意，这样可能会导致精度不够，误差积累到一定程度就会出现明显的时钟偏移
	--之所以要CallOut而不是直接run func 是因为我不想受func的调用机制的干扰,
	--要不就由CallOut提供一个RunNow的函数。
	ObjCallOut (env, func, 0, ...)
	local TimerIndex = ObjCallFre (env, func, Period, ...)
	return TimerIndex
end

--返回:当天相差的秒数，和相差的天数
function GetDiff (hour, min, wday)
	local t = os.date("*t")
	t.hour = hour or 0
	t.min = min or 0
	t.sec = 0
	wday = wday or 1 -- 1 == sunday
	return os.time(t) - os.time(), wday - t.wday
end

local function Round (x, mod)
	return x >= 0 and x or mod + x
end

--run at hour:min every day
function Daily (env, hour, min, func, ...)
	assert(env)
	local TimerIndex = ObjCallOut (env, RunAt, Round (GetDiff(hour,min), DAY), env, DAY, func, ...)
	return TimerIndex
end

--run at wday:hour:min every week
--sunday = 1,
function Weekly (env, wday, hour, min, func, ...)
	assert(env)
	local diffsec, diffday  = GetDiff (hour, min, wday)
	local at = Round (Round(diffday, 7) * DAY + diffsec, WEEK)
	local TimerIndex = ObjCallOut (env, RunAt, at, env, WEEK, func, ...)
	return TimerIndex
end



function __test_sfunc(...)
	print(os.time(), "testing callout:sfunction",...)
end

local function __test_func(...)
	print(os.time(), "testing callout:function",...)
end

function __test__()
	local TimerIndex1 = CallOut("__test_sfunc", 1, 1)
	local TimerIndex2 = CallOut(__test_func, 1, 2)

	local Obj = getfenv(1)
	assert(Obj.__AllTimers[TimerIndex1])
	assert(Obj.__AllTimers[TimerIndex2])

	RemoveAll(Obj)

	assert(not Obj.__AllTimers[TimerIndex1])
	assert(not Obj.__AllTimers[TimerIndex2])

	local TimerIndex1 = CallOut("__test_sfunc", 1, 1)
	local TimerIndex2 = CallOut(__test_func, 1, 2)

	assert(Obj.__AllTimers[TimerIndex1])
	assert(Obj.__AllTimers[TimerIndex2])
	RemoveCallOut(Obj, TimerIndex1)

	assert(not Obj.__AllTimers[TimerIndex1])
	assert(Obj.__AllTimers[TimerIndex2])

	RemoveCallOut(Obj, TimerIndex1)
	RemoveCallOut(Obj, TimerIndex2)
	assert(not Obj.__AllTimers[TimerIndex1])
	assert(not Obj.__AllTimers[TimerIndex2])

	local TimerIndex1 = CallOut("__test_sfunc", 0, 1)
	--local TimerIndex2 = CallOut(__test_func, 1, 2)

	local TimerIndex3 = CallFre(__test_sfunc,1,"fre test1, only twice")
	assert(Obj.__AllTimers[TimerIndex3])
	CallOut(function()
	print("removing timer3", TimerIndex3)
	RemoveCallOut(Obj, TimerIndex3)
	print(sys.repr(Obj.__AllTimers))
	assert(not Obj.__AllTimers[TimerIndex3])
	end, 3)
	local timer = CallFre(__test_sfunc,3,"fre test2")
	CallOut(function()
		RemoveAll(Obj)
		assert(table.size(Obj.__AllTimers) == 0)
		end, 5)
end

