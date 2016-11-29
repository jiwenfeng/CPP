function ParseTable(table)
	local CodeString = "--徽章等级表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tneedPoint = %d,\n", Data[2])
			end
			CodeString = CodeString .. string.format("\t\tattr = {\n")
			if Data[3] then
				CodeString = CodeString .. string.format("\t\t\tstrength = %d,\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\t\tintellect = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\t\tagility = %d,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\t\tcaptain = %d,\n", Data[6])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tname = '%s',\n", Data[7])
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
	local excelfile = "table/badgeUpgradeList.xls"
	local outfile = "autocode/badgeUpgradeList.lua"
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
