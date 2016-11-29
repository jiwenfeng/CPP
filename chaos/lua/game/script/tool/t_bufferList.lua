function ParseTable(table)
	local CodeString = "--buffer列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"]={\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tlabel = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tnum = %g,\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\ttimelong = %d,\n", Data[7])
			end
			
			
			if Data[8] then
				CodeString = CodeString .. "\t\tattr = {\n"
				if Data[9] then
					CodeString = CodeString .. string.format("\t\t\tstrength = %d,\n", Data[9])
				end
				if Data[10] then
					CodeString = CodeString .. string.format("\t\t\tagility = %d,\n", Data[10])
				end
				if Data[11] then
					CodeString = CodeString .. string.format("\t\t\tintellect = %d,\n", Data[11])
				end
				if Data[12] then
					CodeString = CodeString .. string.format("\t\t\tcaptain = %d,\n", Data[12])
				end
				CodeString = CodeString .. "\t\t},\n"
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\tcanAdd = %d,\n", Data[13])
			end
			CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/bufferList.xls"
	local outfile = "autocode/bufferList.lua"
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
