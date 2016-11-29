--用于统计数据

OnlineTimeTbl = {}

--记录注册玩家数量
function AllRegistedNum()
	local filename = "registedNum"
	local function Record(data, errmsg)
		if not data then
			DATA.log(string.format("获取玩家/商家注册数量失败：%s", errmsg), "stats.log")
			return
		end
		data = UnSerialize(data)
		DATA.Stats(string.format("%s %s:%d", os.date("%Y-%m-%d %H:%M"), data.name, data.num), filename)
	end
	DATA.Count("chaos.user", Record)
--	CALLOUT.Daily(getfenv(2), 0, 0, AllRegistedNum)
end

--记录在线玩家数量
function OnlineUserNum()
	local filename = "userOnline/" .. os.date("%Y%m%d")
	DATA.Stats(string.format("%d:%d", os.date("%H"), LOGIN.GetOnlineUserNum()), filename)
	CALLOUT.CallOut(OnlineUserNum, 3600)
end


function UpdateOnlineTime(roleId, nHour)
	OnlineTimeTbl[roleId] = (OnlineTimeTbl[roleId] or 0) + nHour
end




local CountTbl = {}		--各个表的记录计数器
local path_countTbl = "chaos.count"

function GenId(type)
	CountTbl[type] = (CountTbl[type] or 0) + 1
	if type ~= "npc" then
		DATA.SaveData(path_countTbl, CountTbl, type)
	end
	if type == "user" then
		return string.format("%d%04d%s", CountTbl[type], SERVER_ID, USER_ID_ENDING)
	elseif type == "npc" then
		return string.format("%d%04d%s", CountTbl[type], SERVER_ID, NPC_ID_ENDING)
	elseif type == "hero" then
		return string.format("%d%04d%s", CountTbl[type], SERVER_ID, HERO_ID_ENDING)
	elseif type == "godHero" then
		return string.format("%d%04d%s", CountTbl[type], SERVER_ID, GODHERO_ID_ENDING)
	elseif type == "pet" then
		return string.format("%d%04d%s", CountTbl[type], SERVER_ID, PET_ID_ENDING)
	else
		return string.format("%d%04d", CountTbl[type], SERVER_ID)
	end
end


function SaveLoginHistory(user)
	local sData = {}
	sData.ip = user:GetLoginIp()
	sData.timestamp = user:GetLoginTime()
	sData.nick = user:GetName()
	sData.roleId = user:GetRoleId()
	DATA.Insert("chaos.LoginHistory", sData)
end

function SaveUpgradeHistory(user)
	local sData = {}
	sData.phyle = user:GetPhyle()
	sData.Lv = user:GetLv()
	sData.timestamp = os.time()
	sData.roleId = user:GetRoleId()
	sData.nick = user:GetName()
	sData.timelong = os.time() - user:GetRegTime()
	DATA.Insert("chaos.UpgradeHistory", sData)
end

function SaveItemUseHistory(id, num)
	local sData = {}
	sData.goodsId = id
	sData.num = num
	sData.timestamp = os.time()
	DATA.Insert("chaos.ItemUseHistory", sData)
end

function SaveItemBuyHistory(id, num)
	local sData = {}
	sData.goodsId = id
	sData.num = num
	sData.timestamp = os.time()
	DATA.Insert("chaos.ItemBuyHistory", sData)
end

--type 进入方式
--1 扫荡 0 手动进入
--result 通关结果
--1 通关 0 未通关
function SaveCopyHistory(copyId, roleId, type, result)
	local sData = {}
	sData.copyId = copyId
	sData.roleId = roleId
	sData.nick = USER.GetUserNickInfo(roleId, "nick")
	sData.Lv = USER.GetUserNickInfo(roleId, "Lv")
	sData.type = type
	sData.timestamp = os.time()
	sData.result = result
	DATA.Insert("chaos.CopyHistory", sData)
end

function SaveWordBossHistory(id, num, state, killer, top)
	local sData = {}
	local info = CLSWORLDBOSS.GetWorldBossInfoById(id) 
	sData.timestamp = os.time()
	sData.id = id
	sData.state = state
	if killer then
		sData.killer = USER.GetUserNickInfo(killer, "nick")
	end
	if top then
		sData.top = USER.GetUserNickInfo(top, "nick")
	end
	sData.name = info.name
	sData.num = num
	DATA.Insert("chaos.WordBossHistory", sData)
end

function SavePhyleFightHistory(start, over, phoenix, dragon, tiger, win, prizenum)
	local sData = {}
	sData.timestamp = os.time()
	sData.start = start
	sData.over = over
	sData.phoenix = phoenix or 0
	sData.dragon = dragon or 0
	sData.tiger = tiger or 0
	sData.win = win
	sData.prizenum = prizenum or 0
	DATA.Insert("chaos.PhyleFightHistory", sData)
end

function __init__()
	DATA.Restore(path_countTbl, CountTbl)

end
