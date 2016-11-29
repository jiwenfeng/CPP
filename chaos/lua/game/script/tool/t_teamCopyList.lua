function ParseTable(table)
	local CodeString = "--组队副本列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"]={\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tpicId = \"%d\",\n", Data[4])
			CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[5])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[6])
			
			CodeString = CodeString .. string.format("\t\tmonsterList = {")
			CodeString = CodeString .. string.format("\"%s\"", Data[10])
			if Data[11] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[12])
			end
			CodeString = CodeString .. string.format("},\n")
			if Data[13] then
				CodeString = CodeString .. string.format("\t\tgold = %d,\n", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format("\t\texp = %d,\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\tnimbus = %d,\n", Data[15])
			end
			CodeString = CodeString .. string.format("\t\tmapId = %d,\n", Data[16])
			CodeString = CodeString .. "\t\tshowList = {"
			if Data[17] then
				CodeString = CodeString .. string.format("%d", Data[17])
			end
			for i = 0, 4 do
				if Data[17 + i] then
					CodeString = CodeString .. string.format(", %d", Data[17 + i])
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\titemList = {\n")
			for i = 0, 52, 2 do
				if Data[i + 23] and Data[i + 24] then 
					CodeString = CodeString .. string.format("\t\t\t{id = %d, num = %d},\n", Data[i + 23], Data[i + 24])
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
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
	local excelfile = "table/teamCopyList.xls"
	local outfile = "autocode/teamCopyList.lua"
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
