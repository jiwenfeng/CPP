function ParseTable(table)
	local CodeString = "--修仙塔奖励表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tnum = %d,\n", Data[2])
			CodeString = CodeString .. "\t\tlist = {\n"
			if Data[3] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[3], Data[4], Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[6], Data[7], Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[9], Data[10], Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[12], Data[13], Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[15], Data[16], Data[17])
			end
			if Data[18] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[18], Data[19], Data[20])
			end
			if Data[21] then
				CodeString = CodeString .. string.format("\t\t\t{id = %d, rate = %g},\n", Data[21], Data[22])
			end
			if Data[23] then
				CodeString = CodeString .. string.format("\t\t\t{id = %d, rate = %g},\n", Data[23], Data[24])
			end
			if Data[25] then
				CodeString = CodeString .. string.format("\t\t\t{id = %d, rate = %g},\n", Data[25], Data[26])
			end
			if Data[27] then
				CodeString = CodeString .. string.format("\t\t\t{id = %d, rate = %g},\n", Data[27], Data[28])
			end
			CodeString = CodeString .. "\t\t},\n"
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
	local excelfile = "table/retreatPrize.xls"
	local outfile = "autocode/retreatPrize.lua"
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
