function ParseTable(table)
	local CodeString = "--仙印列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[3])
				CodeString = CodeString .. string.format("\t\tneedNimbus = %d,\n", Data[4])
				if Data[5] then
					CodeString = CodeString .. string.format("\t\tneedMissionId = \"%s\",\n", Data[5])
				end
			end
		--	CodeString = CodeString .. string.format("\t\tslotNum = %d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[7])
			CodeString = CodeString .. string.format("\t\tmaxGemLv = %d,\n", Data[8])
			CodeString = CodeString .. string.format("\t},\n")
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
	local excelfile = "table/statusList.xls"
	local outfile = "autocode/statusList.lua"
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
