function ParseTable(table)
	local CodeString = "--市场升级表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tLv = %d,\n", Data[3])
			CodeString = CodeString .. string.format("\t\tquality = %d,\n", Data[4])
			if Data[5] then
				CodeString = CodeString .. string.format("\t\tcomplex = %d,\n", Data[5])
			end
			if Data[6] then
--				CodeString = CodeString .. string.format("\t\tinStore=\"%s\",\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tmall = %d,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\tneedGold = %d,\n", Data[8])
			end
			CodeString = CodeString .. string.format("\t\tclass = \"%s\",\n", Data[9])
			if Data[10] then
				CodeString = CodeString .. string.format("\t\tneedIngot = %d,\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\tneedVoucher = %d,\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\trecycleGold = %d,\n", Data[12])
			end
			if Data[13] then
		--		CodeString = CodeString .. string.format("\t\trecycleIngot=%d,\n", Data[13])
			end
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[14])
			if Data[15] then
				CodeString = CodeString .. string.format("\t\texplain = \"%s\",\n", Data[15])
			end
			if Data[9] == "gem" then
				CodeString = CodeString .. string.format("\t\tenchaseRate = %g,\n", Data[16])
				CodeString = CodeString .. string.format("\t\tpickRate = %g,\n", Data[17])
				CodeString = CodeString .. "\t\tattr = {\n"
				if Data[18] then
					CodeString = CodeString .. string.format("\t\t\tstrength = %d,\n", Data[18])
				end
				if Data[19] then
					CodeString = CodeString .. string.format("\t\t\tintellect = %d,\n", Data[19])
				end
				if Data[20] then
					CodeString = CodeString .. string.format("\t\t\tagility = %d,\n", Data[20])
				end
				if Data[21] then
					CodeString = CodeString .. string.format("\t\t\tcaptain = %d,\n", Data[21])
				end
				if Data[22] then
					CodeString = CodeString .. string.format("\t\t\tphyscAtk = %d,\n", Data[22])
				end
				if Data[23] then
					CodeString = CodeString .. string.format("\t\t\tmagicAtk = %d,\n", Data[23])
				end
				if Data[24] then
					CodeString = CodeString .. string.format("\t\t\tphyscDef = %d,\n", Data[24])
				end
				if Data[25] then
					CodeString = CodeString .. string.format("\t\t\tmagicDef = %d,\n", Data[25])
				end
				if Data[26] then
					CodeString = CodeString .. string.format("\t\t\thpMax = %d,\n", Data[26])
				end
				if Data[27] then
					CodeString = CodeString .. string.format("\t\t\tspeed = %d,\n", Data[27])
				end
				CodeString = CodeString .. "\t\t},\n"
			end
			CodeString = CodeString .. string.format("\t\tareaType = \"%s\",\n", Data[28])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[29])
			if Data[30] then
				CodeString = CodeString .. string.format("\t\tkind = \"%s\",\n", Data[30])
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
	local excelfile = "table/gemList.xls"
	local outfile = "autocode/gemList.lua"
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
