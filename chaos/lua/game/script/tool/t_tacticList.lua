function ParseTable(table)
	local CodeString = "--计谋列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = %d,\n", Data[3])
			CodeString = CodeString .. string.format("\t\ttarget = %d,\n", Data[4])
			
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[6])
			if Data[18] then
				CodeString = CodeString .. string.format("\t\texplain = \"%s\",\n", Data[18])
			end
			CodeString = CodeString .. string.format("\t\tneedIngot = %d,\n", Data[7])
			if Data[9] then
				CodeString = CodeString .. string.format("\t\ttimelong = %d,\n", Data[9])
				CodeString = CodeString .. string.format("\t\tuplimit = %d,\n", Data[10])
			end
			CodeString = CodeString .. "\t\tattr = {\n"
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\tphyscAtkRate = %g,\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t\tmagicAtkRate = %g,\n", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\t\tphyscDefRate = %g,\n", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format("\t\t\tmagicDefRate = %g,\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\t\tresRate = %g,\n", Data[15])
			end
			if Data[16] then
				CodeString = CodeString .. string.format("\t\t\tpillageIncrease = %g,\n", Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format("\t\t\tpillageReduce = %g,\n", Data[17])
			end
			CodeString = CodeString .. "\t\t},\n"
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
	local excelfile = "table/tacticList.xls"
	local outfile = "autocode/tacticList.lua"
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
