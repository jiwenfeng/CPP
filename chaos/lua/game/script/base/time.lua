--$Id$
local TIME_BASE = 1072886400 --2004年1月1日 0:00 星期四

local EXCEL_DATE_BASE_VALUE = 37986 --2004年1月0日在Excel中的数值表示

-- 从Excel数值形式的日期表示，转化为相对于2004年1月1日为第一天开始算起的的第几天
function ExcelDateValue2RelayDayNo(ExcelValue)
	local value = ExcelValue - EXCEL_DATE_BASE_VALUE
	if value < 0 then
		return 0
	else
		return value
	end
end

-- 从2004-1-1起的相对天数转化为Excel数值形式的日期表示。
function RelayDayNo2ExcelDateValue(RelayNo)
	return RelayNo + EXCEL_DATE_BASE_VALUE
end

function GetDayNumOfMonth(Year, Month) -- 返回Year Month有多少天
	local CurTime = os.date("*t")
	Year = Year or CurTime.year
	Month = Month or CurTime.month

	local StartTime = { year = Year, month = Month, day = 1, hour = 0, min = 0, sec = 0, }

	local EndTime = nil
	if Month + 1 == 13 then
		EndTime = { year = Year + 1, month = 1, day = 1, hour = 0, min = 0, sec = 0, }
	else
		EndTime = { year = Year, month = Month + 1, day = 1, hour = 0, min = 0, sec = 0, }
	end

	local StartSecs = os.time(StartTime)
	local EndSecs = os.time(EndTime)

	local SubSecs = EndSecs - StartSecs
	assert(SubSecs > 0)

	return math.floor((SubSecs / 86400)) -- 理论上不会有小数
end

function RelayDayNo2Sec(DayNo)
	return TIME_BASE + DayNo * 3600 * 24
end

-- 返回所给时间是相对于2004年1月1日为第一天开始算起的的第几天
function GetRelaDayNo(Time)
	local TotalDay = 0
	local Standard = TIME_BASE		--2004年1月1日 00:00
	if not Time then
		Time = os.time()
	end
	if Time > Standard then
		TotalDay = (Time - Standard)/3600/24
	else
		TotalDay = (Standard - Time )/3600/24
	end
	return math.floor(TotalDay) + 1
end

-- 返回所给时间是相对于2004年1月1日为第一星期开始算起的的第几周，分割点是周一凌晨0:00
function GetRelaWeekNo(Time)
	local TotalWeek = 0
	local Standard = TIME_BASE + 3600*24*4		--因为2004年1月1日 0:00是星期四早上0:00，要改成星期日24:00
	Time = Time or os.time()
	if Time > Standard then
		TotalWeek = (Time - Standard)/3600/24/7
	else
		TotalWeek = (Standard - Time )/3600/24/7
	end
	return math.floor(TotalWeek + 1)
end

-- 返回所给时间是相对于2004年1月的第几个月，调用者保证 Time > TIME_BASE
function GetRelaMonthNo(Time)
	Time = Time or os.time()
	local Base = os.date("*t", TIME_BASE)
	local BaseYear, BaseMonth = Base.year, Base.month
	local Current = os.date("*t", Time)
	local CurYear, CurMonth = Current.year, Current.month

	return ( (CurYear-BaseYear) * 12 + CurMonth )
end

--根据当前日期时间，得到需要的以秒计算的单位
--时区为北京时间
function MkTime(year, mon, day, hour, min, sec)
	mon = mon - 2
	if 0 >= mon then   --1..12 -> 11,12,1..10
		mon = mon + 12 -- Puts Feb last since it has leap day
		year = year - 1
	end

	return (((
		(math.floor(year/4) - math.floor(year/100) + math.floor(year/400) + math.floor(367*mon/12) + day) +
			year*365 - 719499
	    )*24 + hour --/* now have hours */
	  )*60 + min    --/* now have minutes */
	)*60 + sec      --/* finally seconds */
	- 28800	    --/*调整时区*/
end

--将"2005-06-01 00:00:00"格式的日期转换为time table形式
function sDate2Table(sDateTime)
	if type(sDateTime) ~= "string" or
		string.match(sDateTime, "^%d+%-%d+%-%d+ %d+:%d+:%d+$") == nil then
		return nil
	end

	local MatchTable = {}

	for item in string.gmatch(sDateTime, "%d+") do
		table.insert(MatchTable, item)
	end

	local TimeTable = {}
	TimeTable.year = MatchTable[1]
	TimeTable.month = MatchTable[2]
	TimeTable.day = MatchTable[3]
	TimeTable.hour = MatchTable[4]
	TimeTable.min = MatchTable[5]
	TimeTable.sec = MatchTable[6]
	return TimeTable
end

--将"2006-06-01 10:00:00"这样的时间转换为秒
function sDate2Sec(sDateTime)
	return os.time(sDate2Table(sDateTime))
end

--将秒数转成字符串 "2009-01-03 22:10:53"
function sSec2DateTime( Sec )
	return os.date("%Y-%m-%d %H:%M:%S", Sec)
end

function sSec2DateTimeCn( Sec, OnlyDate )
	if OnlyDate then
		return os.date("%Y年%m月%d日", Sec)
	else
		return os.date("%Y年%m月%d日 %H时%M分%S秒", Sec)
	end
end

--获取当前的时间，格式为"2006-06-01 10:00:00"
function GetuNow()
	return os.date("%Y-%m-%d %H:%M:%S", os.time()) end

--获取unix格式的当前日期"2006-06-01"
function GetuDate()
	return os.date("%Y-%m-%d", os.time())
end

local M = {1,3,5,7,8,10,12}
function ValidDate (y,m,d)
	local nMaxDay = 30
	if m == 2 then
		if (y %4 == 0) and (y % 100 ~= 0) then
			nMaxDay = 29
		elseif y % 400 == 0 then
			nMaxDay = 29
		else
			nMaxDay = 28
		end
	end
	if table.member_key (M, m) then
		nMaxDay = 31
	end

	if d < 1 or d > nMaxDay then
		return false
	end
	if m < 1 or m > 12 then
		return false
	end

	return true
end

--参数和os.time的参数一样。
--直接使用数据转换的方法，比较简单一点~
function ValidDateTime(TimeTbl)
	if not TimeTbl.year or
		not TimeTbl.month or
		not TimeTbl.day then
		return false
	end
	local Time = os.time(TimeTbl)
	local NewTimeTbl = {year = os.date("%Y", Time),
			month = os.date("%m", Time),
			day = os.date("%d", Time),
			hour = os.date("%H", Time),
			min = os.date("%M", Time),
			sec = os.date("%S", Time), }
	for k,v in pairs(TimeTbl) do
		if tonumber(v) ~= tonumber(NewTimeTbl[k]) then
			return false
		end
	end
	return true
end

function SecToDes( Diff )
        local Day = math.floor(Diff / (3600*24))
        Diff = Diff % (3600*24)
        local Hour = math.floor(Diff / 3600)
        Diff = Diff % 3600
        local Min = math.floor(Diff / 60)
        Diff = Diff % 60
        local Sec = Diff % 60

        local Ret = ""
        if Day > 0 then
                Ret = Ret..Day.."天"
        end
        if Hour > 0 then
                Ret = Ret..Hour.."小时"
        end
        if Min > 0 then
                Ret = Ret..Min.."分"
        end
        if Sec > 0 then
                Ret = Ret..Sec.."秒"
        end

        return Ret
end

function SecToDesMin( Diff )
	local Day = math.floor(Diff / (3600*24))
	Diff = Diff % (3600*24)
	local Hour = math.floor(Diff / 3600)
	Diff = Diff % 3600
	local Min = math.floor(Diff / 60)

	local Ret = ""
	if Day > 0 then
		Ret = Ret..Day.."天"
	end
	if Hour > 0 then
		Ret = Ret..Hour.."小时"
	end
	if Min > 0 then
		Ret = Ret..Min.."分"
	end

	return Ret
end

function TimeDiffDes(Time1, Time2)
	return SecToDes( math.abs(Time1-Time2) )
end


---------------------------
function AdjustWDay(WDay)
	WDay = WDay - 1
	if WDay <= 0 then
		WDay = 7
	end
	return WDay
end

local function CalDiff(SrcHour, SrcMin, SrcSec, DstHour, DstMin, DstSec)
	local SrcTimeSecs = SrcHour * 3600 + SrcMin * 60 + SrcSec
	local DstTimeSecs = DstHour * 3600 + DstMin * 60 + DstSec

	return (DstTimeSecs - SrcTimeSecs)
end

function GetTimeSecsByWeek(Date, DstWDay, DstHour, DstMin, DstSec) -- 返回第一个Date那个周的WeekDay Hour Min Sec 的 Date
	local SrcTimeSecs = os.time(Date)
	local SrcWDay = AdjustWDay(Date.wday)
	local SrcHour = Date.hour
	local SrcMin = Date.min
	local SrcSec = Date.sec

	return SrcTimeSecs + ((DstWDay - SrcWDay) * 3600 * 24 + CalDiff(SrcHour, SrcMin, SrcSec, DstHour, DstMin, DstSec))
end

function GetTimePointDesc(Sec)
	return string.format("%s",os.date("%Y年%m月%d日 %H时%M分%S秒", Sec))
end

local Wday2CnDescTbl = {
	[2] = "一",
	[3] = "二",
	[4] = "三",
	[5] = "四",
	[6] = "五",
	[7] = "六",
	[1] = "日", --fck
}
function Wday2CnDesc( Wday )
	return Wday2CnDescTbl[Wday]
end

-- 返回按自然日算的天数差别
-- "2010-04-25 23:30:00"  "2010-04-26 09:10:00"   差1天
function YDayDiff( CurSec, PastSec )
	return GetRelaDayNo(CurSec) - GetRelaDayNo(PastSec)
end

function GetWDayRelaMin()	--取得今天的wday 和 今天到目前为止的总分钟数
	local CurTime = os.date( "*t", os.time() )
	local CurWDay = CurTime["wday"]
	local CurHour = CurTime["hour"]
	local CurMin  = CurTime["min"]
	local CurSec  = CurTime["sec"]

	local RelativeMin = CurHour * 60 + CurMin
	local RelativeSec = RelativeMin * 60 + CurSec

	return CurWDay, RelativeMin, RelativeSec
end

--两个时间区间有无重叠
function IsTimeOverlap(TimeBegin1, TimeEnd1, TimeBegin2, TimeEnd2)
	if TimeBegin1 > TimeEnd2 or TimeEnd1 < TimeBegin2 then
		return false
	else
		return true
	end
end

function GetDayBeginTime(Time)
	Time = Time or os.time()
	local t = os.date("*t", Time)
	t.hour = 0
	t.min = 0
	t.sec = 0
	return os.time(t)
end
