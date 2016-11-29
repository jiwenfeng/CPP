local times = 0
function ParseTable(table)
	local CodeString = "--称号表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			if Data[1] then
				CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
				CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
				CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
				if Data[4] then
					CodeString = CodeString .. string.format("\t\tsex = %d,\n", Data[4])
				end
				CodeString = CodeString .. string.format("\t\tquality = %d,\n", Data[5])
				if Data[6] then
					CodeString = CodeString .. string.format("\t\trank = %d,\n", Data[6])
				end
				if Data[7] then
					CodeString = CodeString .. string.format("\t\tmin = %d,\n", Data[7])
					CodeString = CodeString .. string.format("\t\tmax = %d,\n", Data[8])
				end
				if Data[9] then
					CodeString = CodeString .. string.format("\t\tkind = \"%s\",\n", Data[9])
				end
				if Data[10] then
					CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[10])
				end
				CodeString = CodeString .. "\t},\n"
			end
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
	local excelfile = "table/titleList.xls"
	local outfile = "autocode/titleList.lua"
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
