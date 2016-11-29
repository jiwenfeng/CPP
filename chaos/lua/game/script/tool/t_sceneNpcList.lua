function ParseTable(table)
	local CodeString = "--场景npc列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tmapId = %d,\n", Data[4])
				CodeString = CodeString .. string.format("\t\tposX = %d,\n", Data[5])
				CodeString = CodeString .. string.format("\t\tposY = %d,\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tcopyType = %d,\n", Data[7])
				CodeString = CodeString .. string.format("\t\tcopyId = %d,\n", Data[8])
			end
			CodeString = CodeString .. string.format("\t\tdialog = {\n")
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\t\"%s\",\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\t\t\"%s\",\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\t\"%s\",\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t\t\"%s\",\n", Data[12])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[13] then
				CodeString = CodeString .. string.format("\t\theadId = '%s',\n", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format("\t\tmodelId = '%s',\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\tdegree = '%s',\n", Data[15])
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
	local excelfile = "table/SceneNpcList.xls"
	local outfile = "autocode/sceneNpcList.lua"
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
