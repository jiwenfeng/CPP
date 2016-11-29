function ParseTable (table)
	local CodeString = "--宝箱列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tchestId = %d,\n", Data[2])
			CodeString = CodeString .. "\t\tlist = {\n"
			for i = 3, 20, 3 do
				CodeString = CodeString .. string.format("\t\t\t{min = %d,", Data[i])
				CodeString = CodeString .. string.format("max = %d,", Data[i + 1])
				CodeString = CodeString .. string.format("num = %d},\n", Data[i+2])
			end
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/TotetsuReward.xls"
	local outfile = "autocode/TotetsuReward.lua"
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
