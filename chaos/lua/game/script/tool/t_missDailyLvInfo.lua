--日常任务等级分类列表
function ParseTable(table)
	local CodeString = "--日常任务等级分类列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tminLv = %d,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tmaxLv = %d,\n", Data[3])
			CodeString = CodeString .. string.format("\t\tlist = {")
			if Data[4] then
				CodeString = CodeString .. string.format("\"%s\"", Data[4])
			end
			for i = 1, 10 do
				if Data[4 + i] then
					CodeString = CodeString .. string.format(", \"%s\"", Data[4 + i])
				end
			end
			CodeString = CodeString .. string.format("},\n")
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
	local excelfile = "table/missDailyLvInfo.xls"
	local outfile = "autocode/missDailyLvInfo.lua"
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
