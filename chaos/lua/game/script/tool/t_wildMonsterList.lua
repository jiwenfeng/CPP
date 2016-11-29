function ParseTable(table)
	local CodeString = "--野地怪物\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			end
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tlocation = {x = %d, y = %d},\n", Data[3], Data[4])
			end
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
	local excelfile = "table/wildMonsterList.xls"
	local outfile = "autocode/wildMonsterList.lua"
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
