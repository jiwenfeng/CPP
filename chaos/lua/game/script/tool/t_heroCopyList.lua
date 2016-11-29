function ParseTable(table)
	local CodeString = "--英魂副本列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"]={\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tmapId = \"%d\",\n", Data[4])
			CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[5])
			CodeString = CodeString .. string.format("\t\tneedMissionId = \"%s\",\n", Data[6])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[7])
			
			CodeString = CodeString .. string.format("\t\tmonsterList = {\n")
			for i = 0, 2 do
				if Data[10 + i * 14] then
					CodeString = CodeString .. string.format("\t\t\t[%d] = {\n", i + 1)
					CodeString = CodeString .. string.format("\t\t\t\tteamId = \"%s\",\n", Data[10 + i * 14])
					CodeString = CodeString .. string.format("\t\t\t\tposX = %d,\n", Data[11 + i * 14])
					CodeString = CodeString .. string.format("\t\t\t\tposY = %d,\n", Data[12 + i * 14])
					if Data[13 + i * 14] then
						CodeString = CodeString .. string.format("\t\t\t\tgold = %d,\n", Data[13 + i * 14])
					end
					if Data[14 + i * 14] then
						CodeString = CodeString .. string.format("\t\t\t\texp = %d,\n", Data[14 + i * 14])
					end
					if Data[15 + i * 14] then
						CodeString = CodeString .. string.format("\t\t\t\tnimbus = %d,\n", Data[15 + i * 14])
					end
					CodeString = CodeString .. string.format("\t\t\t\titemList = {\n")
					if Data[17 + i * 14] then
						CodeString = CodeString .. string.format("\t\t\t\t\t{id = %d, ", Data[17 + i * 14])
						if Data[16 + i * 14] then
							CodeString = CodeString .. string.format("needMissionId = \"%d\", ", Data[16 + i * 14])
						end
						CodeString = CodeString .. string.format("min = %d, ", Data[18 + i * 14])
						CodeString = CodeString .. string.format("max = %d},\n", Data[19 + i * 14])
					end
					if Data[21 + i * 14] then
						CodeString = CodeString .. string.format("\t\t\t\t\t{id = %d, ", Data[21 + i * 14])
						CodeString = CodeString .. string.format("min = %d, ", Data[22 + i * 14])
						CodeString = CodeString .. string.format("max = %d},\n", Data[23 + i * 14])
					end
					CodeString = CodeString .. string.format("\t\t\t\t},\n")
					CodeString = CodeString .. string.format("\t\t\t},\n")
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[52] then
				CodeString = CodeString .. string.format("\t\tgold = %d,\n", Data[52])
			end
			if Data[53] then
				CodeString = CodeString .. string.format("\t\texp = %d,\n", Data[53])
			end
			if Data[54] then
				CodeString = CodeString .. string.format("\t\tnimbus = %d,\n", Data[54])
			end
			CodeString = CodeString .. string.format("\t\titemList = {\n")
			if Data[56] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, quality = %d, num = %d, rate = %g},\n", Data[56], Data[57], Data[58], Data[59], Data[60])
			end
			if Data[61] then
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, quality = %d, num = %d, rate = %g},\n", Data[61], Data[62], Data[63], Data[64], Data[65])
			end
			for i = 0, 4 do
				if Data[66 + i * 3] then
					CodeString = CodeString .. string.format("\t\t\t{id = %d, num = %d, rate = %g},\n", Data[66 + i * 3], Data[67 + i * 3], Data[68 + i * 3])
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
	local excelfile = "table/heroCopyList.xls"
	local outfile = "autocode/heroCopyList.lua"
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
