--用于服务器启动时载入数据

function Log(str, filename)
	local file = io.open(filename, "w")
	assert(file)
	file:write(str .. "\n")
	file:close()
end

local protableS = {}   --服务器协议
local protableC = {}   --客户端协议

function LoadPro(tbl)
	local count = 1
	for _, proName in ipairs(tbl) do
		if string.find(proName, "^s_") then
			table.insert(protableS, proName)
		elseif string.find(proName, "^c_") then
		--	if protableC[proName] then
		--		print("LoadPro", proName)
		--	end
			protableC[proName] = count			
			count = count + 1
		end
	end
end

function LoadSellerPro(tbl)
	local buffS = 1000
	local buffC = 1000
	for _, proName in ipairs(tbl) do
		if string.find(proName, "^s_") then
			protableS[buffS] = proName
			buffS = buffS + 1
		elseif string.find(proName, "^c_") then
			protableC[proName] = buffC
			buffC = buffC + 1
		end
	end
end

function LoadGmPro(tbl)
	local buffS = 2000
	local buffC = 2000
	for _, proName in ipairs(tbl) do
		if string.find(proName, "^s_") then
			protableS[buffS] = proName
			buffS = buffS + 1
		elseif string.find(proName, "^c_") then
			protableC[proName] = buffC
			buffC = buffC + 1
		end
	end
end

function GetProtocolNameById(Cmd)
	return protableS[Cmd]
end

function GetProtocolIdByName(Cmd)
	return protableC[Cmd]
end

function GmGetCProNameById(Cmd)
	for k, v in pairs(protableC) do
		if v == Cmd then
			return k
		end
	end
	return nil
end

USERPRO = Import("protocol/userPro.lua")
LoadPro(USERPRO.GetTable())

local GMPRO = Import("protocol/gmPro.lua")
LoadGmPro(GMPRO.GetTable())


function LogProtocolList(type)
	local function log_list(type)
		local list = {}
		if type == "s" then
			for k, v in ipairs(protableS) do
				table.insert(list, {id = k, name = v})
			end
		elseif type == "c" then
			for k, v in pairs(protableC) do
				table.insert(list, {id = v, name = k})
			end
			table.sort(list, function(a, b) return a.id < b.id end)
		end
		local buff = {}
		for _, v in ipairs(list) do
			table.insert(buff, string.format("%3d  ----  %s\n", v.id, v.name))
		end
		Log(table.concat(buff), string.format("protocol/%s_pro.log", type))
	end
	if type then
		log_list(type)
	else
		log_list("s")
		log_list("c")
	end
end

