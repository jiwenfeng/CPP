--任务列表
function ParseTable(table)
	local CodeString = "--兵种列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\t[%d] = {minLv = %d, maxLv = %d, name = \"%s\", add = %g},\n", Data[2], Data[3], Data[4], Data[5], Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t[%d] = {minLv = %d, maxLv = %d, name = \"%s\", add = %g},\n", Data[7], Data[8], Data[9], Data[10], Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t[%d] = {minLv = %d, maxLv = %d, name = \"%s\", add = %g},\n", Data[12], Data[13], Data[14], Data[15], Data[16])
			end
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
	local excelfile = "table/soldierList.xls"
	local outfile = "autocode/soldierList.lua"
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
