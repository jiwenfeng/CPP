--任务列表
function ParseTable(table)
	local CodeString = "--武将资质\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tapt = {min = %g, max = %g},\n", Data[2], Data[3])
			CodeString = CodeString .. string.format("\t\tpoint = %d,\n", Data[4])
			CodeString = CodeString .. string.format("\t\thpMax = {min = %d, max = %d},\n", Data[5], Data[6])
			CodeString = CodeString .. string.format("\t\tphyscAtk = {min = %d, max = %d},\n", Data[7], Data[8])
			CodeString = CodeString .. string.format("\t\tphyscDef = %d,\n", Data[9])
			CodeString = CodeString .. string.format("\t\tmagicAtk = {min = %d, max = %d},\n", Data[10], Data[11])
			CodeString = CodeString .. string.format("\t\tmagicDef = %d,\n", Data[12])
			CodeString = CodeString .. string.format("\t\tspeed = %d,\n", Data[13])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[14])
			CodeString = CodeString .. string.format("\t},\n")
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
	local excelfile = "table/heroApt.xls"
	local outfile = "autocode/heroApt.lua"
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
