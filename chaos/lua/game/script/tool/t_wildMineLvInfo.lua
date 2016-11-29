function ParseTable(table)
	local CodeString = "--野地矿产等级信息\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tgoldIncome = %d,\n", Data[2])
			end
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tsparIncome = %d,\n", Data[3])
			end
			CodeString = CodeString .. "\t\tmonsterList = {"
			CodeString = CodeString .. string.format("\"%s\"", Data[4])
			if Data[5] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[8])
			end
			CodeString = CodeString .. "},\n"
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
	local excelfile = "table/wildMineLvInfo.xls"
	local outfile = "autocode/wildMineLvInfo.lua"
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
