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
			for i = 0, 1 do
				if Data[7 + i * 5] then
					CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", ", Data[7 + i * 5])
					if Data[8 + i * 5] then
						CodeString = CodeString .. string.format("quality = %d, ", Data[8 + i * 5])
					end
					if Data[9 + i * 5] then
						CodeString = CodeString .. string.format("Lv = %d, ", Data[9 + i * 5])
					end
					CodeString = CodeString .. string.format("num = %d, rate = %g},\n", Data[10 + i * 5], Data[11 + i * 5])
				end
			end
			for i = 0, 4 do
				if Data[17 + i * 3] then
					CodeString = CodeString .. string.format("\t\t\t{id = %d, num = %d, rate = %g},\n", Data[17 + i * 3], Data[18 + i * 3], Data[19 + i * 3])
				end
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
	local excelfile = "table/wildChestLvInfo.xls"
	local outfile = "autocode/wildChestLvInfo.lua"
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
