function ParseTable(table)
	local CodeString = "local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tsupport = %d,\n", Data[3])
			CodeString = CodeString .. string.format("\t\tagainst = %d,\n", Data[4])
			CodeString = CodeString .. "\t\telectNeed = {"
			if Data[5] then
				CodeString = CodeString .. string.format("[\"%d\"] = 1", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format(", [\"%d\"] = 1", Data[6])
			end
			CodeString = CodeString .. "},\n"
			CodeString = CodeString .. "\t\tattr = {\n"
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\texpRate = %g,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\t\tgoldRate = %g,\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\tban = %d,\n", Data[9])
			end
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. string.format("\t\tlevel = %d,\n", Data[10])
			CodeString = CodeString .. "\t\timpeachNeed = {"
			if Data[11] then
				CodeString = CodeString .. string.format("[\"%d\"] = 1", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format(", [\"%d\"] = 1", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format(", [\"%d\"] = 1", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format(", [\"%d\"] = 1", Data[14])
			end
			CodeString = CodeString .. "},\n"
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
	local excelfile = "table/officialList.xls"
	local outfile = "autocode/officialList.lua"
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
