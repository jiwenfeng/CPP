function GetCityInfo(connection, data)
	local city = CITY.GetCityObjById(data.cityId)
	if not city then
		return false, "无效的城池ID"
	end
	local sData = {}
	sData.cityId = data
	local locate = city:GetLocation()
	sData.cityName = string.format("%s(%d, %d)", city:GetName(), locate.x, locate.y)
	sData.cityLv = city:GetLv()
	sData.process = string.format("%d/%d", city:GetSparNum(), city:GetSparMax())
	sData.limit =  city:GetLv() >= city:GetLimitLv() 
	local info = CITY.GetFormatedCityLink()
	sData.linkDesc = info.describe
	sData.owner = city:GetOwner()
	sData.nick = USER.GetUserNickInfo(city:GetOwner(), "nick")
	sData.attachNum = city:GetMemberNum()
	sData.form = {}
	local form = city:GetCurForm()
	for k, v in pairs(form:GetGridList()) do
		local list = {}
		local id = v:GetRoleId()
		list.roleId = id
		list.identity = id == sData.owner
		if IsUserId(id) then
			list.Lv = USER.GetUserNickInfo(id, "Lv")
		end
		table.insert(sData.form, list)
	end
	return true, sData
end


function __init__()
	GAMEMANAGE.RegisterFunc(CMD.LOAD_CITY_INFO, GetCityInfo)
end
