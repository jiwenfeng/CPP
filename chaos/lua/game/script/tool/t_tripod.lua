function ParseTable(table)
	local CodeString = "--炼器鼎列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tneedGold = %d,\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tneedNimbus = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\tneedIngot = %d,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tforward = %g,\n", Data[6])
			end
			CodeString = CodeString .. string.format("\t\trateList = {\n")
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\t{quality = 1, rate = %g},\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\t\t{quality = 2, rate = %g},\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\t{quality = 3, rate = %g},\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\t\t{quality = 4, rate = %g},\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t{quality = 5, rate = %g},\n", Data[11])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tnext = %d,\n", Data[12])
			if Data[13] then
				CodeString = CodeString .. string.format("\t\tneedVipLv = %d,\n", Data[13])
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
	local excelfile = "table/tripod.xls"
	local outfile = "autocode/tripodList.lua"
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
