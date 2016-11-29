function ParseTable(table)
	local CodeString = "--怪物队伍列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"]={\n", Data[1])
			CodeString = CodeString .. string.format("\t\ttype = \"npc\",\n")
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\theadId = %d,\n", Data[3] or Data[1])
			CodeString = CodeString .. string.format("\t\tLv = %d,\n", Data[4])
			CodeString = CodeString .. string.format("\t\tformId = \"%s\",\n", Data[5])
			CodeString = CodeString .. string.format("\t\tformLv = %d,\n", Data[6])
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tforce = %d,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\tquality = %d,\n", Data[8])
			end
			local posList = {}
			CodeString = CodeString .. string.format("\t\tlist = {\n")
			if Data[9] then
				posList[tostring(Data[9])] = true
				CodeString = CodeString .. string.format("\t\t\t{pos = \"%d\", id = \"%s\"%s},\n", Data[9], Data[11], Data[10] and ", isBoss = 1" or "")
			end
			for i = 0, 7 do
				if Data[12 + i * 2] then
					if posList[tostring(Data[12 + i * 2])] then
						print("ERROR: pos exist!", line, tostring(12 + i * 2))
					end
					posList[tostring(Data[12 + i * 2])] = true
					CodeString = CodeString .. string.format("\t\t\t{pos = \"%d\", id = \"%s\"},\n", Data[12 + i * 2], Data[13 + i * 2])
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if not Data[28] then
				print(line)
			end
			CodeString = CodeString .. string.format("\t\tmodelId = %d,\n", Data[28])
			if Data[29] then
				CodeString = CodeString .. "\t\tisBoss = 1,\n"
			end
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
	local excelfile = "table/monsterTeamList.xls"
	local outfile = "autocode/monsterTeamList.lua"
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
