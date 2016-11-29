--职业列表
function ParseTable(table)
	local CodeString = "--职业列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tbasic = \"%s\",\n", Data[4])
			end
			CodeString = CodeString .. string.format("\t\trecommend = {\n")
			if Data[5] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"strength\", num = %d},\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"intellect\", num = %d},\n", Data[6])
			end
			CodeString = CodeString .. string.format("\t\t\t{type = \"agility\", num = %d},\n", Data[7])
			CodeString = CodeString .. string.format("\t\t\t{type = \"captain\", num = %d},\n", Data[8])
			CodeString = CodeString .. string.format("\t\t},\n")
			
			CodeString = CodeString .. string.format("\t\trestraint = {\n")
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\t[\"%s\"]= 1,\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\t\t[\"%s\"]= 1,\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t[\"%s\"]= 1,\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t\t[\"%s\"]= 1,\n", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\t\t[\"%s\"]= 1,\n", Data[13])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\trange = %d,\n", Data[14])
			CodeString = CodeString .. "\t\tmodelList = {\n"
			CodeString = CodeString .. string.format("\t\t\t[%d] = {", Data[15])
			if Data[16] then
				CodeString = CodeString .. string.format("%d,", Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format("%d,", Data[17])
			end
			if Data[18] then
				CodeString = CodeString .. string.format("%d,", Data[18])
			end
			if Data[19] then
				CodeString = CodeString .. string.format("%d,", Data[19])
			end
			if Data[20] then
				CodeString = CodeString .. string.format("%d,", Data[20])
			end
			CodeString = CodeString .. string.format("},\n")
			CodeString = CodeString .. string.format("\t\t\t[%d] = {", Data[21])
			if Data[22] then
				CodeString = CodeString .. string.format("%d,", Data[22])
			end
			if Data[23] then
				CodeString = CodeString .. string.format("%d,", Data[23])
			end
			if Data[24] then
				CodeString = CodeString .. string.format("%d,", Data[24])
			end
			if Data[25] then
				CodeString = CodeString .. string.format("%d,", Data[25])
			end
			if Data[26] then
				CodeString = CodeString .. string.format("%d,", Data[26])
			end
			CodeString = CodeString .. "},\n"
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
	local excelfile = "table/jobType.xls"
	local outfile = "autocode/jobType.lua"
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
