function ParseTable(table)
	local CodeString = "--锻造消耗\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tinject = %d,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tpromote = %d,\n", Data[3])
			CodeString = CodeString .. string.format("\t\tattr = {\n")
			CodeString = CodeString .. string.format("\t\t\tstrength = %d,\n", Data[4])
			CodeString = CodeString .. string.format("\t\t\tintellect = %d,\n", Data[5])
			CodeString = CodeString .. string.format("\t\t\tagility = %d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\t\tcaptain = %d,\n", Data[7])
			CodeString = CodeString .. string.format("\t\t\thpMax = %d,\n", Data[8])
			CodeString = CodeString .. string.format("\t\t\tphyscAtk = %d,\n", Data[9])
			CodeString = CodeString .. string.format("\t\t\tphyscDef = %d,\n", Data[10])
			CodeString = CodeString .. string.format("\t\t\tmagicAtk = %d,\n", Data[11])
			CodeString = CodeString .. string.format("\t\t\tmagicDef = %d,\n", Data[12])
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tenchase = %d,\n", Data[15])
			CodeString = CodeString .. string.format("\t\tpick = %d,\n", Data[16])
--[[			
			CodeString = CodeString .. string.format("\t\tenhance = {%d, 0, %d, %d, %d},\n", Data[2], Data[3], Data[4], Data[5])
			CodeString = CodeString .. string.format("\t\tinject = {%d, 0, %d, %d, %d},\n", Data[6], Data[7], Data[8], Data[9])
			CodeString = CodeString .. string.format("\t\tpromote = {%d, 0, %d, %d, %d},\n", Data[10], Data[11], Data[12], Data[13])	
]]			CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/forgeCost.xls"
	local outfile = "autocode/forgeCost.lua"
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
