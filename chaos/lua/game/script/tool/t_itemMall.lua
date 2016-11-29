function ParseTable(table)
	local CodeString = "--商城道具列表\n local autoTable = {\n"	
	for page, pageData in ipairs(table) do
		CodeString = CodeString .. string.format("\t[%d] = {\n", page)
		for line, Data in ipairs(pageData) do
			if line > 1 then
				CodeString = CodeString .. string.format("\t\t{id = %d, goldType = \"%s\", price = %d", Data[1], Data[3], Data[4])
				if Data[5] then
					CodeString = CodeString .. string.format(", oldPrice = %d", Data[5])
				end
				CodeString = CodeString .. string.format("},\n")
			end
		end
		CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/itemMall.xls"
	local outfile = "autocode/itemMall.lua"
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
