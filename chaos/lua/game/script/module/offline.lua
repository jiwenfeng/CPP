-- 离线操作处理模块
-- 该模块用于记录玩家离线阶段，对玩家产生影响的各种操作。在玩家登录时，所有的离线操作将被按时间顺序依次处理。
-- 登录时的处理阶段发生在玩家数据读取完毕之后，在 AllOnlineUser 表中登记之前，即此时玩家数据依然是不在线状态。
-- 由于读取数据库需要一定时间，这段时间内可能会有新的离线操作产生，而这些操作应该发生在数据库中的操作记录之前，因此用 LoginProcessTbl 将新操作存储起来，
--	   并保证放在旧操作之后（从尾部插入），而旧操作被读取完毕按时间顺序插入 LoginProcessTbl 表的头部（按时间由大到小，从头部插入），以确保操作顺序无误。


local LoginProcessTbl = {}			--离线操作记录表
local path_offline = "chaos.offline"

local ProcessTbl = {
	["gold"] = "SetGold",				--修改金币
	["voucher"] = "SetVoucher",			--修改礼券
	["nimbus"] = "SetNimbus",			--修改灵气
	["ingot"] = "SetIngot",				--修改元宝
	["item"] = "AddItemById",			--增加物品
	["exp"] = "SetExp",					--修改经验
	["heroState"] = "SetHeroState",		--武将状态
	["heroExp"] = "SetHeroExp",			--修改武将经验
	["indent"] = "SetIndentCount",		--修改订单次数
	["clearFavor"] = "ClearFriendFavor",--清除好感度
	["modFavor"] = "ModFriendFavor",	--清除好感度
	["friendApply"] = "AddFriendApply",	--添加好友申请
	["setUnionInfo"] = "SetUnionInfo", 	--联盟信息
	["quitUnion"] = "QuitUnion",		--退出联盟
--	["addPrize"] = "AddPrize",			--添加奖励
	["retreatChall"] = "RetreatChallWin",	--修仙塔挑战胜利
	["setCity"] = "SetCity",			--存储所在城池
	["quitCity"] = "QuitCity",			--离开城池所在城池
	["addMine"] = "AddWildMine",			--添加野矿
	["abandonMine"] = "AbandonWildMine",	--丢弃野矿
	["spar"] = "SetSpar",					--修改晶石
	["workInfo"] = "SetWorkInfo",		--玩家状态
	["pillagePro"] = "SetPillageProtectTime",	--玩家被掠夺保护时间
	["official"] = "SetOfficialInfo",	--玩家官职信息
	["fightHp"] = "ProHpAfterFight",		--恢复出战队伍血量
	["addBuffer"] = "AddOneBuffer",		--添加buffer
	["teamCopyNum"] = "SetDailyTeamCopyNum",		--组队副本次数
	["title"] = "SetTitleById",
	["tempCity"] = "SetTempCity",		--临时城主
	["phyleFeat"] = "SetPhyleFeatGot",		--势力军功
	["addStruct"] = "AddStruct",		--新增部落建筑
	["delStruct"] = "DelStruct",		--移除部落建筑
	["phyleFight"] = "SetPhyleFightStats",		--势力战统计
	["achvGot"] = "SetAchvGot",			--成就达成
	["petPrompt"] = "SetPetPrompt",		--本命精灵提示
	["initVigour"] = "InitVigour",		--初始化精力
	["finishMission"] = "FinishAllAcpMission",	--完成全部已接任务
	["setLocate"] = "SetLastLocateInfo",		--设置位置	
	["attachToMaster"] = "AttachToMainCity",	--依附回主城
	["setTeamExp"] = "SetTeamExp",
	["setCopyPassed"] = "SetCopyPassed",
}
function GetValidProcessTbl()
	return ProcessTbl
end

--记录离线操作
function SetOfflineData(roleId, type, ...)
	if not USER.GetUserNickById(roleId) or not ProcessTbl[type] then
		return DATA.log(string.format("无效的离线记录: roleId = %s, type = %s.", roleId or "nil", type or "nil"), "offline.log")
	end
	local param = table.deepcopy(arg)
	param.n = nil
	if LoginProcessTbl[roleId] then			--玩家已经请求登录，正在读取数据库中的离线操作，此时应将新的离线操作加入到 LoginProcessTbl 中用户记录的尾部
		table.insert(LoginProcessTbl[roleId], {type = type, param = param})		-- 插入表的尾部
		return
	end
	local id = STATS.GenId("offline")
	local buff = {type = type, roleId = roleId}
	buff.param = param
	buff._id = id
	buff.time = os.time()
	if not DATA.UpdateData(path_offline, {_id = id}, buff) then
		DATA.log(string.format("Save %s offline data failed: connect server Saved error!", roleId), "offline.log")
	end
end

--清理离线操作记录
function ClearOfflineData(list)
	for _, v in ipairs(list) do
		DATA.DBRemove(path_offline, {_id = v._id})
	end
end

function ProcessOfflineData(user)
	local roleId = user:GetRoleId()
	if not LoginProcessTbl[roleId] then
		return
	end
	for _, v in ipairs(LoginProcessTbl[roleId]) do
		USERBASE.clsUser[ProcessTbl[v.type]](user, unpack(v.param))
	end
end


--登录时处理离线操作
function LoginProOffline(user, AfterPro)
	local roleId = user:GetRoleId()
	local function callback(Data)		
		Data = UnSerialize(Data)
		if not Data then
			return AfterPro(false)
		end
		table.sort(Data, function(a, b) return tonumber(a._id) < tonumber(b._id) end)
		for _, v in ipairs(Data) do
			table.insert(LoginProcessTbl[roleId], {type = v.type, param = v.param})
		end
		ProcessOfflineData(user)			--开始处理所有的离线操作
		AfterPro(true)						--回到登录处理阶段，玩家数据将被登记在线
		LoginProcessTbl[roleId] = nil
		ClearOfflineData(Data)
	end
	LoginProcessTbl[roleId] = {}
	DATA.Read(path_offline, {roleId = roleId}, callback)
end


function __init__()

end
