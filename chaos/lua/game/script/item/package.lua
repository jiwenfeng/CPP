
local packageList = {}

function GetPackageList()
	return packageList
end

function GetPackageInfoById(id)
	return packageList[id]
end

---------------------------------------------
clsItem = BASEITEM.clsBaseItem:Inherit()

function clsItem:__init__()
	Super(clsItem).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsItem:initsavedata()
	self.__save.id = 0
	self.__save.num = 0
end

function clsItem:inittempdata()

end

function clsItem:GetPackageInfo()
	return GetPackageInfoById(self:GetId())
end

function clsItem:EnhanceSkill(obj)
	local info = BASEITEM.GetItemById(self:GetId())
	if info.type == "skillPoint" then
		if obj:IsAllSkillMaxLv() then
			return false, string.format("%s所有技能都已到满级，不能使用帝脉丹", obj:GetName())
		end
		obj:SetSkillPoint(info.num)
	elseif info.type == "skillLv" then
		local skillId = tostring(info.num)
		local skillInfo = BASESKILL.GetSkillInfo(skillId)
		local pos = obj:IsSkillExist(skillId)
		if not pos then
			return false, string.format("%s没有学习技能【%s】。", obj:GetRoleType() == "user" and "您" or "该武将", skillInfo.name)
		end
		local skillData = obj:GetSkill(pos)
		if skillData.Lv >= #skillInfo.list then
			return false, string.format("技能【%s】已经达到满级。", skillInfo.name)
		end
		obj:SkillUpgrade(pos, true)
	else
		return false, "无效的道具类型。"
	end
	return true
end

--使用物品
function clsItem:Use(obj, num)
	local type = self:GetType()
	if type == "skillPoint" or type == "skillLv" then
		return self:EnhanceSkill(obj)
	end
	if obj:GetRoleType() ~= "user" then
		return false, "该道具只能由主角使用。"
	end
	local info = GetPackageInfoById(self:GetId())
	if info.spar then
		local tribe = obj:GetTribe()
		if not tribe then
			return false, "您还没有建立部落，不能使用该道具。"
		end
	end
	if info.itemList then
		if obj:GetGridMax(USERBASE.PACKAGE) - obj:GetItemGridNum(USERBASE.PACKAGE) < #info.itemList then
			return false, "您的背包空间不足。"
		end
	end
	local msg_money = ""
	if info.gold then
		obj:SetGold(info.gold)
		msg_money = msg_money .. string.format("钱币%d", info.gold)
	end
	if info.ingot then
		obj:SetVoucher(info.ingot)
		msg_money = msg_money .. string.format("元宝%d", info.ingot)
	end
	if info.nimbus then
		obj:SetNimbus(info.nimbus)
		msg_money = msg_money .. string.format("灵气%d", info.nimbus)
	end
	if info.voucher then
		obj:SetVoucher(info.voucher)
		msg_money = msg_money .. string.format("礼券%d", info.voucher)
	end
	if info.exp then
		obj:SetExp(info.exp)
		msg_money = msg_money .. string.format("经验%d", info.exp)
	end
	if info.spar then
		local tribe = obj:GetTribe()
		tribe:ModResourceStat("spar", "num", info.spar)
		msg_money = msg_money .. string.format("晶石%d", info.spar)
	end
	if info.feat then
		obj:SetPhyleFeatGot(info.feat)
		msg_money = msg_money .. string.format("军功%d", info.feat)
	end
	if #msg_money > 0 then
		obj:SendProtocol("c_show_tips", string.format("您获得了%s。", msg_money))
	end
	local msg_item = ""
	if info.itemList then
		local list = BASEITEM.GenRandomItemByRate(info.itemList)
		for k, v in pairs(list) do
			local item = BASEITEM.NewItem(v.id, v.num, {lock = 1})
			if item then
				obj:AddItem(USERBASE.PACKAGE, item)
				msg_item = msg_item .. string.format("%s×%d ", item:GetName(), item:GetNum())
			end
		end
	end
	if #msg_item > 0 then
		obj:SendProtocol("c_show_tips", string.format("您获得物品%s。", msg_item))
	end
	return true
end


function __init__()
	local PACKAGE = Import("autocode/packageList.lua")
	packageList = PACKAGE.GetTable()
end
