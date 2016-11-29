function ParseTable(table)
	local CodeString = "--野地矿产等级信息\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. "\t\tmonsterList = {"
			CodeString = CodeString .. string.format("\"%s\"", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[6])
			end
			CodeString = CodeString .. "},\n"
			CodeString = CodeString .. "\t\tprize = {\n"
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\t{gold = %d, rate = %g},\n", Data[7], Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\t{nimbus = %d, rate = %g},\n", Data[9], Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t{spar = %d, rate = %g},\n", Data[11], Data[12])
			end
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. "\t},\n"
		end
	end
	CodeString = CodeString .. "}\n\nfunction GetTable()\n\treturn autoTable\nend\n"
	return CodeString
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/wildMonsterLvInfo.xls"
	local outfile = "autocode/wildMonsterLvInfo.lua"
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
