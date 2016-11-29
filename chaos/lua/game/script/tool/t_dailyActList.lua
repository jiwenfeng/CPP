function ParseTable(table)
	local CodeString = "--日常活动列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tpicId = %d,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format("\t\ttimes = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tneedItemId = %d,\n", Data[6])
			end
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[7])
			CodeString = CodeString .. string.format("\t\tdescrip = \"%s\",\n", Data[9])
			CodeString = CodeString .. string.format("\t\ttime = \"%s\",\n", Data[10])
			if Data[11] then
				CodeString = CodeString .. string.format("\t\tlink = %s,\n", Data[11])
			end
			CodeString = CodeString .. string.format("\t\tfuncId = %d,\n", Data[12])
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
	local excelfile = "table/dailyActList.xls"
	local outfile = "autocode/dailyActList.lua"
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
