function ParseTable(xlsTable)
	local strList = {"--名将列表\n local autoTable = {\n"}
	for line, Data in ipairs(xlsTable[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			table.insert(strList, string.format("\t[\"%d\"]={\n", Data[1]))
		--	local buff = string.format("{\"heroId\":\"%s\",", Data[1])
			table.insert(strList, string.format("\t\tname = \"%s\",\n", Data[2]))
			table.insert(strList, string.format("\t\theadId = %d,\n", Data[3]))
			table.insert(strList, string.format("\t\tmodelId = %d,\n", Data[4]))
			table.insert(strList, string.format("\t\tcopyId = \"%s\",\n", Data[5]))
			table.insert(strList, string.format("\t\tjob = \"%s\",\n", Data[6]))
			table.insert(strList, string.format("\t\tgrownRate = {\n"))
			table.insert(strList, string.format("\t\t\t[\"strength\"] = %\d,\n", Data[7]))
			table.insert(strList, string.format("\t\t\t[\"intellect\"] = %d,\n", Data[8]))
			table.insert(strList, string.format("\t\t\t[\"agility\"] = %d,\n", Data[9]))
			table.insert(strList, string.format("\t\t\t[\"captain\"] = %d,\n", Data[10]))
			table.insert(strList, string.format("\t\t},\n"))
			
			table.insert(strList, string.format("\t\taptitude = %d,\n", Data[11]))
			table.insert(strList, string.format("\t\tmaxAptLv = %d,\n", Data[12]))
			table.insert(strList, string.format("\t\tLv = %d,\n", Data[13]))
			table.insert(strList, string.format("\t\tstrength = %d,\n", Data[14]))
			table.insert(strList, string.format("\t\tintellect = %d,\n", Data[15]))
			table.insert(strList, string.format("\t\tagility = %d,\n", Data[16]))
			table.insert(strList, string.format("\t\tcaptain = %d,\n", Data[17]))
			table.insert(strList, string.format("\t\tdescribe = \"%s\",\n", Data[18]))
		--	buff = buff .. string.format("\"story\":\"%s\"", Data[18])
			if Data[19] then
				table.insert(strList, string.format("\t\tcombin = \"%s\",\n", Data[19]))
		--		buff = buff .. string.format(",\"combin\":\"%s\"", Data[19])
			end
		--	buff = buff .. "}"
			if Data[20] then
				table.insert(strList, string.format("\t\tskillId = \"%s\",\n", Data[20]))
			end
			table.insert(strList, string.format("\t\tphyscAtk = %d,\n", Data[21]))
			table.insert(strList, string.format("\t\tphyscDef = %d,\n", Data[22]))
			table.insert(strList, string.format("\t\tmagicAtk = %d,\n", Data[23]))
			table.insert(strList, string.format("\t\tmagicDef = %d,\n", Data[24]))
			table.insert(strList, string.format("\t\thpMax = %d,\n", Data[25]))
			table.insert(strList, string.format("\t\tspeed = %d,\n", Data[26]))
			table.insert(strList, string.format("\t\tforce = %d,\n", Data[27]))
			table.insert(strList, "\t},\n")
	--		SaveCode(string.format("../client/famousHero/%s.txt", Data[1]), buff)
		end
	end
	table.insert(strList, "}\n")
	table.insert(strList, "function GetTable()\n\treturn autoTable\nend\n")
	return table.concat(strList)
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/famousHeroJinYong.xls"
	local outfile = "autocode/famousHeroJinYong.lua"
	local cmd = string.format([[python 'tool/xls2table.py' '%s']], excelfile)
	local fd = io.popen(cmd)
	local loadxls = loadstring(fd:read("*a"))
	assert(loadxls)
	loadxls()
	assert(__XLS_END)
	fd:close()
	os.execute("mkdir -p ../client/famousHero")
	local Code = ParseTable(xlstable)
	SaveCode(outfile, Code)
end
gen()
