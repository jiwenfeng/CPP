--测试npc

local menuList = {
	[1] = {name = "我想要非常富有！", subMenu = {
			[1] = {name = "万能的主啊，请赐我【财如粪土】！"},
			[2] = {name = "万能的主啊，请赐我【珠光宝气】！"},
			[3] = {name = "万能的主啊，请赐我【钟灵毓秀】！"},
			[4] = {name = "万能的主啊，请赐我【位高权重】！"},
		},
	},
	[2] = {name = "我想要更加强壮！", subMenu = {
			[1] = {name = "万能的主啊，请赐我【精神焕发】！"},
			[2] = {name = "万能的主啊，请赐我【森罗万象】！"},
			[3] = {name = "万能的主啊，请赐我【曾经沧海】！"},
		},
	},
	[3] = {name = "我想要征服世界！", subMenu = {
			[1] = {name = "万能的主啊，请赐我【兵精粮足】！"},
			[2] = {name = "万能的主啊，请赐我【突飞猛进】！"},
			[3] = {name = "万能的主啊，请赐我【鸡犬升天】！"},
		},
	},
}
---------------------------------------------------
clsAdminNpc = CLSNPC.clsNpc:Inherit()

function clsAdminNpc:__init__()
	Super(clsAdminNpc).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsAdminNpc:initsavedata()	
end

function clsAdminNpc:inittempdata()
	self.__temp.id = nil
	self.__temp.name = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
end


function clsAdminNpc:OnTalk(user)
	MISSION.CheckMissionEvent(user, "dialog", self:GetId(), 1)
	local menu, msg = self:GetMenu(user, menuList)
	local index = self:SendMenu(user, msg or "凡人，说出你的愿望。", menu)
	local info = menu[index]
	if info and not info.missionId then
		if index == 1 then
			local how = self:SendMenu(user, "虔诚祈祷，你会富有的。", self:GetMenu(user, menuList[1].subMenu, true))
			local sData = {content = "如你所愿！\n你获得了"}
			if how == 1 then
				user:SetGold(500000)
				sData.content = sData.content .. "500000钱币。"
			elseif how == 2 then
				user:SetIngot(100000)
				sData.content = sData.content .. "100000元宝。"
			elseif how == 3 then
				user:SetNimbus(500000)
				sData.content = sData.content .. "500000灵气。"	
			elseif how == 4 then
				user:SetTotalRecharge(5000)
				sData.content = sData.content .. "500 RMB的充值效果。"
			end
			user:SendProtocol("c_npc_click", sData)	
		elseif index == 2 then
			local how = self:SendMenu(user, "虔诚祈祷，你会强大的。", self:GetMenu(user, menuList[2].subMenu, true))
			local sData = {content = "如你所愿！\n你获得了"}
			if how == 1 then
				user:SetVigour(50)
				sData.content = sData.content .. "50点精力。"
			elseif how == 2 then
				user:SetSkillPoint(5)
				sData.content = sData.content .. "5个技能点。"
			elseif how == 3 then
				user:SetExp(500000)
				sData.content = sData.content .. "500000点经验。"
			end
			user:SendProtocol("c_npc_click", sData)
		elseif index == 3 then
			local how = self:SendMenu(user, "虔诚祈祷，你会强大的。", self:GetMenu(user, menuList[3].subMenu, true))
			local sData = {content = "如你所愿！\n"}
			if how == 1 then
				user:RestoreAllHp()
				for k, v in pairs(user:GetHeroList()) do
					v:RestoreAllHp()
				end
				sData.content = sData.content .. "你的队伍已经恢复到最佳状态。"
			elseif how == 2 then
				for i = 1, 10 do
					user:Upgrade()
				end
				sData.content = sData.content .. "你的等级提升了10级。"
			elseif how == 3 then
				for k, v in pairs(user:GetHeroList()) do
					for i = 1, 10 do
						if v:GetLv() < v:GetMaxLv() then
							v:Upgrade()
						end
					end
				end
				sData.content = sData.content .. "你的武将全体提升10级。"
			end
			user:SendProtocol("c_npc_click", sData)
		end
	else
		self:ProMission(user, menu, index)
	end
end

--[[
function clsAdminNpc:sendmenu(user, msg, option)
	local co = coroutine.running()
	connection:SendProtocol("c_npc_click", sData)
	user.CoObj = co
	return coroutine.yield(co)
end
function clsAdminNpc:OnTalk(user)
		local index = self:sendmenu(user, "你要做什么", {"加血", "addMp"})
		if index == 1 then --加血
			local how = self:sendmenu(user, "how much", {"100", "200", ...})
			if how == 1 then
				self:sendmenu()
			else
			end
		elseif index == 2 then --addMp
		end
end
]]
--[[
function clsAdminNpc:sendmenu(user, msg, option, callback)
	user.npcmenu_callback = callback
	user:sendpro()
end

function clsAdminNpc:OnTalk(user)
	local function callback(index)
		if index == 1 then
			local function howcallback()
			end
			self:sendmenu(user, "how much", {100, 200...}, howcallback)
		end
	end
	self:sendmenu(user, "msg", option, callback)
end

]]

return clsAdminNpc