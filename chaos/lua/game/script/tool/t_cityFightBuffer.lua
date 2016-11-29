function ParseTable(table)
	local CodeString = "--slg战斗附加属性\n local autoTable = {\n"
	local id
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读		
		if line > 1 then
			if id and id ~= Data[1] then
				CodeString = CodeString .. "\t},\n"
			end
			if not id or id ~= Data[1] then
				CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			end
			id = Data[1]
			local start, num = 2, 20
			CodeString = CodeString .. string.format("\t\t[\"%s\"] = {", Data[start])
			if Data[start + 1] then
				CodeString = CodeString .. string.format("[\"%s\"] = 1", Data[start + 1])
			end
			for i = start + 2, start + num do
				if Data[i] then
					CodeString = CodeString .. string.format(", [\"%s\"] = 1", Data[i])
				end
			end
			CodeString = CodeString .. "},\n"
		end		
	end
	CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/cityFightBuffer.xls"
	local outfile = "autocode/cityFightBuffer.lua"
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
