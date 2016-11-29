function ParseTable(table)
	local CodeString = "--怪物列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"]={\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tisBoss = %d,\n", Data[2])
			end
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\theadId = %d,\n", Data[35] or 101)
			CodeString = CodeString .. string.format("\t\tLv = %d,\n", Data[4])
			CodeString = CodeString .. string.format("\t\taptLv = %d,\n", Data[5])
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tstrength = %d,\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tintellect = %d,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\tagility = %d,\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\tcaptain = %d,\n", Data[9])
			end
			CodeString = CodeString .. string.format("\t\thpMax = %d,\n", Data[10])
			CodeString = CodeString .. string.format("\t\tphyscAtkMin = %g,\n", Data[11])
			CodeString = CodeString .. string.format("\t\tphyscAtkMax = %g,\n", Data[12])
			CodeString = CodeString .. string.format("\t\tphyscDef = %g,\n", Data[13])
			CodeString = CodeString .. string.format("\t\tmagicAtkMin = %g,\n", Data[14])
			CodeString = CodeString .. string.format("\t\tmagicAtkMax = %g,\n", Data[15])
			CodeString = CodeString .. string.format("\t\tmagicDef = %g,\n", Data[16])
			CodeString = CodeString .. string.format("\t\tspeed = %g,\n", Data[17])
			CodeString = CodeString .. string.format("\t\tjink = %g,\n", Data[18] or 0)
			CodeString = CodeString .. string.format("\t\tfatal = %g,\n", Data[19] or 0)
			CodeString = CodeString .. string.format("\t\tcrush = %g,\n", Data[20] or 0)
			CodeString = CodeString .. string.format("\t\tharmDeep = %g,\n", Data[21] or 0)
			CodeString = CodeString .. string.format("\t\tphyscResist = %g,\n", Data[22] or 0)
			CodeString = CodeString .. string.format("\t\tmagicResist = %g,\n", Data[23] or 0)
			CodeString = CodeString .. string.format("\t\tskillList = {")
			if Data[24] then
				CodeString = CodeString .. string.format("{id = \"%d\", Lv = %d}", Data[24], Data[25])
			end
			if Data[26] then
				CodeString = CodeString .. string.format(", {id = \"%d\", Lv = %d}", Data[26], Data[27])
			end
			if Data[28] then
				CodeString = CodeString .. string.format(", {id = \"%d\", Lv = %d}", Data[28], Data[29])
			end
			if Data[30] then
				CodeString = CodeString .. string.format(", {id = \"%d\", Lv = %d}", Data[30], Data[31])
			end
			CodeString = CodeString .. string.format("},\n")
			if Data[32] then
				CodeString = CodeString .. string.format("\t\tfuryRounds = %d,\n", Data[32])
			end
			if Data[33] then
				CodeString = CodeString .. string.format("\t\tfurySkill = \"%s\",\n", Data[33])
			end
			CodeString = CodeString .. string.format("\t\tforce = %d,\n", Data[34] or 1000)
			CodeString = CodeString .. string.format("\t\tjob = \"%s\",\n", Data[36])
			CodeString = CodeString .. string.format("\t\tmodelId = %d,\n", Data[37])
			CodeString = CodeString .. string.format("\t\ttype = %d,\n", Data[38])
			CodeString = CodeString .. "\t},\n"
		end
	end
	CodeString = CodeString .. "}\n\n"
	CodeString = CodeString .. "function GetTable()\n\treturn autoTable\nend\n"
	return CodeString
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/monsterList.xls"
	local outfile = "autocode/monsterList.lua"
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
