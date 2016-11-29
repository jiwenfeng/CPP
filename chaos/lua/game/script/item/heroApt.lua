--飞仙丹
--特点：使用后提升武将品质
local FAIL_RATE = 0.1
local SUCCESS_RATE = 0.6
local LUCKY_RATE = 0.3

local heroAptList = BASEITEM.GetItemListByClass("heroApt")

function GetHeroAptList()
	return heroAptList
end

function GetHeroAptInfoById(id)
	return heroAptList[id]
end

-------------------------------------------------
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

--使用物品
function clsItem:Use(obj, num)
	if obj:GetRoleType() == "godHero" then
		return false, string.format("神魂不能服用【%s】。", self:GetName())
	end
	if obj:GetRoleType() ~= "hero" then
		return false, string.format("%s只能给战魂使用。", self:GetName())
	end
	local aptLv = obj:GetAptLv()
	if aptLv >= obj:GetMaxAptLv() then
		return false, string.format("该战魂已为最高品质，不能继续服用%s。", self:GetName())
	end
	local result, msg = 0, ""
	local ran = math.random()
	if ran <= LUCKY_RATE then
		result = math.random(5, 8)
		msg = "突飞猛进!"
	elseif ran <= (LUCKY_RATE + SUCCESS_RATE) then
		result = math.random(1, 4)
		msg = "提升成功!"
	else
		result = -math.random(1, 2)
	end
	obj:ModAptitude(result)
	if result < 0 then
		msg = string.format("提升失败! %s 的潜质-1。", obj:GetName())
	else
		msg = msg .. string.format(" %s 的潜质+%d", obj:GetName(), result)
		if aptLv ~= obj:GetAptLv() then
		--	obj:AdjustPoint()			--策划要求品质升级后不重新调整属性点
			local aptInfo = CLSHERO.GetHeroAptInfoByLv(obj:GetAptLv())
			msg = msg .. string.format("，品质提升为%s", aptInfo.name)
		end
		msg = msg .. "。"
	end
	local con = LOGIN.GetConnection(obj:GetOwner())
	if con and con.roleId == obj:GetOwner() then
		local buff = {heroId = obj:GetId()}
		local aptLv = obj:GetAptLv()
		buff.aptLv = aptLv
		buff.aptitude = obj:GetAptitude()
		local aptInfo = CLSHERO.GetHeroAptInfoByLv(aptLv)
		buff.aptMax = aptInfo.apt.min
		con:SendProtocol("c_hero_aptUpdate", buff)
	end
	return true, msg
end