function ParseTable(table)
	local CodeString = "local autoTable = {\n"
	for line, Data in pairs(table[1]) do --只读第一页
		--第一行不读
		CodeString = CodeString .. "\t"
		for i = 1, 20 do
			if Data[i] then
				CodeString = CodeString .. string.format("\"%s\",", Data[i])
			end
		end
		CodeString = CodeString .. "\n"
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
	local excelfile = "table/firstName.xls"
	local outfile = "autocode/firstName.lua"
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
