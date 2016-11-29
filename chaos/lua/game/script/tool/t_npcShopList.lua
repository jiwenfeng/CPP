function ParseTable(table)
	local CodeString = "--npc商店列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%d\"] = {", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("%d", Data[2])
			end
			if Data[3] then
				CodeString = CodeString .. string.format(", %d", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format(", %d", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format(", %d", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format(", %d", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format(", %d", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format(", %d", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format(", %d", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format(", %d", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format(", %d", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format(", %d", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format(", %d", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format(", %d", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format(", %d", Data[15])
			end
			if Data[16] then
				CodeString = CodeString .. string.format(", %d", Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format(", %d", Data[17])
			end
			if Data[18] then
				CodeString = CodeString .. string.format(", %d", Data[18])
			end
			if Data[19] then
				CodeString = CodeString .. string.format(", %d", Data[19])
			end
			CodeString = CodeString .. string.format("},\n")
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
	local excelfile = "table/npcShopList.xls"
	local outfile = "autocode/npcShopList.lua"
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
