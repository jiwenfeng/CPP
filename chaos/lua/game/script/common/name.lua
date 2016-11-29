--自动生成名字
local firstName = {}
local girlName = {}
local boyName = {}
local LoginView = {}
local MAX_VIEW_LIST = 10

local ReservedName = {}				--系统保留名

local ExistedName = {}
local path_existName = "chaos.existname"

function SetNameExist(name)
	if ExistedName[name] then
		return
	end
	ExistedName[name] = 1
	DATA.SaveData(path_existName, ExistedName, name)
end

function DelNameExist(name)
	ExistedName[name] = nil
	DATA.DBRemove(path_existName, {_id = name})
end

function IsNameExist(name)
	return ReservedName[name] or ExistedName[name]
end

--sex  (1男 0女)
function GenName(sex, type)
	local ran = math.random(1, #firstName)
	local first = firstName[ran]
	local last = ""
	if sex == 1 then
		last = last .. boyName[math.random(1, #boyName)]
	else
		last = last .. girlName[math.random(1, #girlName)]
	end
	local name = first .. last
	if type == "user" then
		if IsNameExist(name) then			--已经被占用
			DATA.log(string.format("Name repeat : %s", name), "mame.log")
			return GenName(sex, type)
		end
	elseif ReservedName[name] then
		return GenName(sex, type)
	end
	return name
end


local ViewEventList = {
	[1] = {event = "enter", rate = 0.8},
	[2] = {event = "equip", rate = 0.14, list = {"月影剑", "星尘剑", "玄黄破坚剑", "月影弓", "星尘弓", "玄黄破坚弓", "月影杖", "星尘杖", "玄黄破坚杖", "月影袍", "星尘铠", "玄黄破坚铠", "月影裤", "星尘护腿", "玄黄破坚下装", "月影靴", "星尘靴", "玄黄破坚战靴", "月影项链", "星尘项链", "玄黄破坚项链", "月影戒指", "星尘戒指", "玄黄破坚戒指"}},
	[3] = {event = "generals", rate = 0.02, list = {"令狐冲", "芙蓉姐姐", "风清扬"}},
	[4] = {event = "enhance", rate = 0.04, list = {"月影剑", "星尘剑", "玄黄破坚剑", "月影弓", "星尘弓", "玄黄破坚弓", "月影杖", "星尘杖", "玄黄破坚杖", "月影袍", "星尘铠", "玄黄破坚铠", "月影裤", "星尘护腿", "玄黄破坚下装", "月影靴", "星尘靴", "玄黄破坚战靴", "月影项链", "星尘项链", "玄黄破坚项链", "月影戒指", "星尘戒指", "玄黄破坚戒指"}, lv = {min = 10, max = 12}},
}

function GenLoginViewList()
	local boyList = table.copy(LoginView.boy)
	local girlList = table.copy(LoginView.girl)
	local list = {}
	local function gen_name(nameList)
		if #nameList <= 0 then
			return
		end
		local index = math.random(1, #nameList)
		local name = nameList[index]
		table.remove(nameList, index)
		return name
	end
	while (#list < MAX_VIEW_LIST) do
		local buff = {}
		if math.random() >= 0.7 then
			buff.sex = 0
			buff.nick = gen_name(girlList)
		else
			buff.sex = 1
			buff.nick = gen_name(boyList)
		end
		if not buff.nick then
			break
		end		
		local ran = math.random()
		local rate = 0
		for k, v in ipairs(ViewEventList) do
			rate = rate + v.rate
			if rate >= ran then
				buff.event = v.event
				if v.list then
					buff.name = v.list[math.random(1, #v.list)]
				end
				if v.lv then
					buff.Lv = math.random(v.lv.min, v.lv.max)
					buff.result = math.random(0, 1)
				end
				break
			end			
		end
		table.insert(list, buff)
	end
	return list
end





function __init__()
	FIRSTNAME = Import("autocode/firstName.lua")
	firstName = FIRSTNAME.GetTable()

	GIRLNAME = Import("autocode/girlName.lua")
	girlName = GIRLNAME.GetTable()

	BOYNAME = Import("autocode/boyName.lua")
	boyName = BOYNAME.GetTable()
	
	GRILLOGIN = Import("autocode/girlLogin.lua")
	LoginView.girl = GRILLOGIN.GetTable()
	
	BOYLOGIN = Import("autocode/boyLogin.lua")
	LoginView.boy = BOYLOGIN.GetTable()
	
	DATA.Restore(path_existName, ExistedName)
	

	PROTOCOL.s_gen_name = function(connection, data)
		if data ~= 1 and data ~= 0 then
			return ERROR.ErrorMsg(connection, "请选择性别")
		end
		local name = GenName(data)
		connection:SendProtocol("c_gen_name", name)
	end
	PROTOCOL.s_login_view = function(connection, data)
		if data ~= "get" then
			return ERROR.ErrorMsg(connection, "协议错误，无效的请求。")
		end
		local sData = GenLoginViewList()
		return connection:SendProtocol("c_login_view", sData)
	end
end
