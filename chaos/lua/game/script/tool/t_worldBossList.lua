function ParseTable(table)
	local CodeString = "--世界boss表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\theadId = %d,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[4])
			CodeString = CodeString .. string.format("\t\tbeginTime = \"%s\",\n", Data[5])
			CodeString = CodeString .. string.format("\t\tendTime = \"%s\",\n", Data[6])
			CodeString = CodeString .. string.format("\t\tnpcId = \"%s\",\n", Data[7])
			CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[8])
			CodeString = CodeString .. string.format("\t\tmapId = \"%d\",\n", Data[9])
			CodeString = CodeString .. "\t},\n"
		end
	end
	CodeString = CodeString .. "}\n"
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
	local excelfile = "table/worldBossList.xls"
	local outfile = "autocode/worldBossList.lua"
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
