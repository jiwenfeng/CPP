function ParseTable(table)
	local CodeString = "--成就列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tkind = \"%s\",\n", Data[4])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[5])
			CodeString = CodeString .. string.format("\t\tpoint = %d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\tprize = {\n")
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\tgold = %d,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\t\tnimbus = %d,\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\titem = {goodsId = %d, num = %d},\n", Data[9], Data[10])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[11] then
				CodeString = CodeString .. string.format("\t\tshowNum = %g,\n", Data[11])
			end
			CodeString = CodeString .. string.format("\t\tlabel = \"%s\",\n", Data[12])
			CodeString = CodeString .. string.format("\t\tnum1 = %g,\n", Data[13])
			if Data[14] then
				CodeString = CodeString .. string.format("\t\tnum2 = %g,\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\tnum3 = %g,\n", Data[15])
			end
			if Data[16] then
				CodeString = CodeString .. string.format("\t\tnum4 = %g,\n", Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format("\t\tnum5 = %g,\n", Data[17])
			end
--[[			CodeString = CodeString .. string.format("\t\tcondt = {\n")
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t{label = \"%s\", needNum = %g},\n", Data[11], Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\t\t{label = \"%s\", needNum = %g},\n", Data[13], Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\t\t{label = \"%s\", needNum = %g},\n", Data[15], Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format("\t\t\t{label = \"%s\", needNum = %g},\n", Data[17], Data[18])
			end
			CodeString = CodeString .. string.format("\t\t},\n")]]
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
	local excelfile = "table/achvList.xls"
	local outfile = "autocode/achvList.lua"
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
