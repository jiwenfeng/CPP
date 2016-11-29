function ParseTable(table)
	local CodeString = "--城镇列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tmapId = \"%d\",\n", Data[4])
			if Data[5] then
				CodeString = CodeString .. string.format("\t\tkind = \"%s\",\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tsize = {x = %d, y = %d},\n", Data[6], Data[7])
			end
			CodeString = CodeString .. string.format("\t\tborn = {x = %d, y = %d},\n", Data[8], Data[9])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[10])
			CodeString = CodeString .. string.format("\t\tnpcList = {\n")
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",",  Data[11])
				CodeString = CodeString .. string.format(" x = %d,", Data[13])
				CodeString = CodeString .. string.format(" y = %d", Data[14])
				if Data[12] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[12])
				else
					CodeString = CodeString .. string.format("},\n")
				end
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[15])
				CodeString = CodeString .. string.format(" x = %d,", Data[17])
				CodeString = CodeString .. string.format(" y = %d", Data[18])
				if Data[16] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[16])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[19] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[19])
				CodeString = CodeString .. string.format(" x = %d,", Data[21])
				CodeString = CodeString .. string.format(" y = %d", Data[22])
				if Data[20] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[20])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[23] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[23])
				CodeString = CodeString .. string.format(" x = %d,", Data[25])
				CodeString = CodeString .. string.format(" y = %d", Data[26])
				if Data[24] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[24])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[27] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[27])
				CodeString = CodeString .. string.format(" x = %d,", Data[29])
				CodeString = CodeString .. string.format(" y = %d", Data[30])
				if Data[28] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[28])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[31] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[31])
				CodeString = CodeString .. string.format(" x = %d,", Data[33])
				CodeString = CodeString .. string.format(" y = %d", Data[34])
				if Data[32] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[32])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[35] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[35])
				CodeString = CodeString .. string.format(" x = %d,", Data[37])
				CodeString = CodeString .. string.format(" y = %d", Data[38])
				if Data[36] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[36])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[39] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[39])
				CodeString = CodeString .. string.format(" x = %d,", Data[41])
				CodeString = CodeString .. string.format(" y = %d", Data[42])
				if Data[40] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[40])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[43] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[43])
				CodeString = CodeString .. string.format(" x = %d,", Data[45])
				CodeString = CodeString .. string.format(" y = %d", Data[46])
				if Data[44] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[44])
				else
					CodeString = CodeString .. string.format("},\n")
				end				
			end
			if Data[47] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[47])
				CodeString = CodeString .. string.format(" x = %d,", Data[49])
				CodeString = CodeString .. string.format(" y = %d", Data[50])
				if Data[48] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[48])
				else
					CodeString = CodeString .. string.format("},\n")
				end
			end
			if Data[51] then
				CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",", Data[51])
				CodeString = CodeString .. string.format(" x = %d,", Data[53])
				CodeString = CodeString .. string.format(" y = %d", Data[54])
				if Data[48] then
					CodeString = CodeString .. string.format(", init = %d},\n", Data[52])
				else
					CodeString = CodeString .. string.format("},\n")
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[55] and Data[56] then
				CodeString = CodeString .. string.format("\t\ttransX = %d,\n\t\ttransY = %d,\n\t\ttransType = \"%s\",\n", Data[55], Data[56], Data[57])
			end
			if Data[58] then
				CodeString = CodeString .. string.format("\t\tminCopyId = %d,\n", Data[58])
				CodeString = CodeString .. string.format("\t\tmaxCopyId = %d,\n", Data[59])
			end
			if Data[60] then
				CodeString = CodeString .. string.format("\t\tnextTown = \"%d\",\n", Data[60])
			end
			if Data[61] then
				CodeString = CodeString .. string.format("\t\tselectMap = \"%d\",\n", Data[61])
			end
			CodeString = CodeString .. string.format("\t\tvalidY = %d,\n", Data[62])
			if Data[63] then
				CodeString = CodeString .. string.format("\t\tcopyListId = \"%s\",\n", Data[63])
			end
			if Data[64] then
				CodeString = CodeString .. string.format("\t\tcopyListName = \"%s\",\n", Data[64])
			end
			if Data[65] then
				CodeString = CodeString .. string.format("\t\tcopyType = \"%s\",\n", Data[65])
			end
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
	local excelfile = "table/townSceneList.xls"
	local outfile = "autocode/townSceneList.lua"
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
