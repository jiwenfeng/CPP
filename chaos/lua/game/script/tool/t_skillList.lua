--技能列表
local jobList = {
	{"warrior", "warrior2", "warrior3"},
	{"taoist", "taoist2", "taoist3"},
	{"archer", "archer2", "archer3"},
}

local function GetValidJobList(type)
	local list = {}
	local horz = math.ceil(type / 4)
	if horz	<= #jobList then
		for k, v in pairs(jobList[horz]) do
			local vert = type % 4
			if vert == 0 or vert == k then
				list[v] = 1
			end
		end
	elseif type == 13 then
		for _, Data in ipairs(jobList) do
			for _, job in ipairs(Data) do
				list[job] = 1
			end
		end
	elseif type == 17 then
		list.pet = 1
	else
		return
	end
	return list
end

function FormatTable(t)
	local list = {"{"}
	for k, v in pairs(t or {}) do
		if type(v) == "table" then
			table.insert(list, k .. "=") 
			for _, v in ipairs(FormatTable(v)) do
				table.insert(list, v)
			end
			table.insert(list, ",")
		else
			table.insert(list, string.format("%s = %s,", k, tostring(v)))
		end
	end
	table.insert(list, "}")
	return list
end

function ParseTable(xls)
	local t = {"--技能列表\nlocal autoTable = {\n"}
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			if Data[1] then
				if line ~= 2 then
					table.insert(t, "\t\t},\n")
					table.insert(t, "\t},\n")
				end
				table.insert(t, string.format("\t[\"%s\"] = {\n", Data[1]))
				table.insert(t, string.format("\t\tname = \"%s\",\n", Data[2]))
				if Data[3] then
					table.insert(t, string.format("\t\tnonMiss = %d,\t--必中\n", Data[3]))
				end
				table.insert(t, string.format("\t\ttype = %d,\t--攻击/辅助/被动/特性\n", Data[4]))
				table.insert(t, string.format("\t\ttarget = %d,\t--自己/我方/敌方\n", Data[5]))
				table.insert(t, string.format("\t\ttrigger = %d,\t--主动/被动\n", Data[6]))
				table.insert(t, string.format("\t\trange = %d,\t--目标范围\n", Data[7]))
				table.insert(t, string.format("\t\tvalidJob = "))
				for _, v in ipairs(FormatTable(GetValidJobList(Data[8]))) do
					table.insert(t, v)
				end
				table.insert(t, ",\n")
				if Data[9] then
					table.insert(t, string.format("\t\tmethod = %d,\n", Data[9]))
				end
				table.insert(t, string.format("\t\tdescribe = \"%s\",\n", Data[10]))
				if Data[11] then
					table.insert(t, string.format("\t\tclass = \"%s\",\n", Data[11]))
				end
				if Data[28] then
					table.insert(t, string.format("\t\tatkType=%d,\n", Data[28]))
				end
				if Data[29] then
					table.insert(t, string.format("\t\tatkAction=%d,\n", Data[29]))
				end
				if Data[30] then
					table.insert(t, string.format("\t\tneedMove = %d,\n", Data[30]))
				end
				table.insert(t, string.format("\t\tquality = %d,\n", Data[31]))
				if Data[32] then
					table.insert(t, string.format("\t\tlastTimes = %d,\n", Data[32]))
				end
				table.insert(t, string.format("\t\tlist = {\n"))
			end
			table.insert(t, string.format("\t\t\t[%d] = {\n", Data[12]))
			if Data[13] then
				table.insert(t, string.format("\t\t\t\tneedLv = %d,\n", Data[13]))
			end
			table.insert(t, string.format("\t\t\t\tcdTime = %d,\n", Data[14] or 0))
			table.insert(t, string.format("\t\t\t\texplain = \"%s\",\n", Data[15] or ""))
			if Data[16] then
				table.insert(t, string.format("\t\t\t\tnum = %g,\n", Data[16]))
			end
			if Data[17] then
				table.insert(t, string.format("\t\t\t\trate = %g,\n", Data[17]))
			end
		--[[	if Data[29] then
				table.insert(t, string.format("\t\t\t\trate = %g,\n", Data[29]))
			end]]
			table.insert(t, string.format("\t\t\t\tbuffer = {\n"))
			for i = 0, 1 do
				if Data[18 + i * 5] then
					table.insert(t, string.format("\t\t\t\t\t{id = \"%s\", last = %d, occur = %g", Data[18 + i * 5], Data[19 + i * 5] or 0, Data[20 + i * 5] or 1))
					if Data[21 + i * 5] then
						table.insert(t, string.format(", num = %g", Data[21 + i * 5]))
					end
					if Data[22 + i * 5] then
						table.insert(t, string.format(", rate = %g", Data[22 + i * 5]))
					end
					table.insert(t, string.format("},\n"))
				end
			end
			table.insert(t, string.format("\t\t\t\t},\n"))
			table.insert(t, string.format("\t\t\t},\n"))
		end
	
	end
	table.insert(t, string.format("\t\t},\n"))
	table.insert(t, string.format("\t},\n"))
	table.insert(t, "}\n\n")
	table.insert(t, "function GetTable()\n\treturn autoTable\nend\n")
	return table.concat(t)
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/skillList.xls"
	local outfile = "autocode/skillList.lua"
	local cmd = string.format([[python 'tool/xls2table.py' '%s']], excelfile)
	local fd = io.popen(cmd)
	local loadxls = loadstring(fd:read("*a"))
	assert(loadxls)
	loadxls()
	assert(__XLS_END)
	fd:close()
	local Code = ParseTable(xlstable)
	SaveCode(outfile, Code)
end
gen()
