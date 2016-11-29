function ParseTable(table)
	local CodeString = "--装备表\n local autoTable = {\n"
	local suitId = 250001
	local flag = 0
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then	
			CodeString = CodeString .. string.format("\t[%d]={\n", Data[1])
			CodeString = CodeString .. string.format("\t\tclass = \"suit\",\n")
			CodeString = CodeString .. string.format("\t\tname=\"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tlist = {\n")
			CodeString = CodeString .. string.format("\t\t\t[1] = {\n")
			CodeString = CodeString .. string.format("\t\t\t\t[\"warrior\"] = {id = %d, name = \"%s\"},\n", Data[3], Data[4])
			CodeString = CodeString .. string.format("\t\t\t\t[\"archer\"] = {id = %d, name = \"%s\"},\n", Data[5], Data[6])
			CodeString = CodeString .. string.format("\t\t\t\t[\"taoist\"] = {id = %d, name = \"%s\"},\n", Data[7], Data[8])			
			CodeString = CodeString .. string.format("\t\t\t},\n")
			CodeString = CodeString .. string.format("\t\t\t[2] = {id = %d, name = \"%s\"},\n", Data[9], Data[10])
			CodeString = CodeString .. string.format("\t\t\t[3] = {id = %d, name = \"%s\"},\n", Data[11], Data[12])
			CodeString = CodeString .. string.format("\t\t\t[4] = {id = %d, name = \"%s\"},\n", Data[13], Data[14])
			CodeString = CodeString .. string.format("\t\t\t[5] = {id = %d, name = \"%s\"},\n", Data[15], Data[16])
			CodeString = CodeString .. string.format("\t\t\t[6] = {id = %d, name = \"%s\"},\n", Data[17], Data[18])
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\texplain = {\n")
			CodeString = CodeString .. string.format("\t\t\t[1] = {num = %d, desc = \"%s\"},\n", Data[19], Data[20])
			CodeString = CodeString .. string.format("\t\t\t[2] = {num = %d, desc = \"%s\"},\n", Data[21], Data[22])
			CodeString = CodeString .. string.format("\t\t\t[3] = {num = %d, desc = \"%s\"},\n", Data[23], Data[24])
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. "\t\tattr={\n"
			for k, v in ipairs(table[2]) do
				if k > 1 then
					if Data[1] == v[1] then
						CodeString = CodeString .. string.format("\t\t\t{num = %d, list ={", v[2])
						if v[3] then
							CodeString = CodeString .. string.format(" physcDef=%d,", v[3])
						end
						if v[4] then
							CodeString = CodeString .. string.format(" magicDef=%d,", v[4])
						end
						if v[5] then
							CodeString = CodeString .. string.format(" hpMax=%d,", v[5])
						end
						if v[6] then
							CodeString = CodeString .. string.format(" jink=%g,", v[6])
						end
						if v[7] then
							CodeString = CodeString .. string.format(" physcResist=%g,", v[7])
						end
						if v[8] then
							CodeString = CodeString .. string.format(" magicResist=%g,", v[8])
						end
						if v[9] then
							CodeString = CodeString .. string.format(" speed=%g,", v[9])
						end
						CodeString = CodeString .. "}},\n"
					end
				end
			end
			CodeString = CodeString .. "\t\t},\n"
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
	local excelfile = "table/itemSuit.xls"
	local outfile = "autocode/itemSuit.lua"
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
