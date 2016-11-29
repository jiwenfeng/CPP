function ParseTable(table)
	local CodeString = "--联盟密境\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			CodeString = CodeString .. string.format("\t\tmapId = \"%d\",\n", Data[4])
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tsize = {x = %d, y = %d},\n", Data[6], Data[7])
			end
			CodeString = CodeString .. string.format("\t\tborn = {x = %d, y = %d},\n", Data[8], Data[9])
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[10])
			CodeString = CodeString .. string.format("\t\tnpcList = {\n")
			for i = 0, 6 do
				if Data[11 + i * 5] then
					CodeString = CodeString .. string.format("\t\t\t{npcId = \"%s\",",  Data[11 + i * 5])
					CodeString = CodeString .. string.format(" x = %d,", Data[13 + i * 5])
					CodeString = CodeString .. string.format(" y = %d", Data[14 + i * 5])
					if Data[12 + i * 5] then
						CodeString = CodeString .. string.format(", init = %d", Data[12 + i * 5])
					end
					if Data[15 + i * 5] then
						CodeString = CodeString .. string.format(", isBoss = %d", Data[15 + i * 5])
					end
					CodeString = CodeString .. string.format("},\n")
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[55] and Data[56] then
				CodeString = CodeString .. string.format("\t\ttransX = %d,\n\t\ttransY = %d,\n", Data[55], Data[56])
			end
			CodeString = CodeString .. string.format("\t\tvalidY = %d,\n", Data[57])
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
	local excelfile = "table/uniSecretScene.xls"
	local outfile = "autocode/uniSecretScene.lua"
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
