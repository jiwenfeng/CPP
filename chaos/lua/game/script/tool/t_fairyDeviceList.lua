function ParseTable(table)
	local CodeString = "--炼器鼎列表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tclass = \"fairyDev\",\n")
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tquality = %d,\n", Data[3])
			CodeString = CodeString .. "\t\tareaType = \"fairyDev\",\n"
			if Data[4] then
				CodeString = CodeString .. string.format("\t\ttype = \"%s\", \n", Data[4])
			end
			CodeString = CodeString .. "\t\trateList = {\n"
			if Data[5] then
				CodeString = CodeString .. string.format("\t\t\t{num = 0, rate = %g},\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\t\t{num = 1, rate = %g},\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\t{num = 2, rate = %g},\n", Data[7])
			end
			CodeString = CodeString .. "\t\t},\n"
			if Data[8] then
				CodeString = CodeString .. string.format("\t\tisRefinable = %d,\n", Data[8])
			end
			CodeString = CodeString .. "\t\tlvList = {\n"
			for k, v in ipairs(table[2]) do 
				if v[1] == Data[1] then
					CodeString = CodeString .. string.format("\t\t\t[%d] = {\n", v[2])
					if v[3] then
						CodeString = CodeString .. string.format("\t\t\t\tdescribe = \"%s\",\n", v[3])
					end
					if v[4] then
						CodeString = CodeString .. string.format("\t\t\t\tisRange = %d,\n", v[4])
					end
					if v[5] then
						CodeString = CodeString .. string.format("\t\t\t\tisFusable = %d,\n", v[5])
					end
			--[[		if v[6] then
						CodeString = CodeString .. string.format("\t\t\t\tisRefinable = %d,\n", v[6])
					end]]
					CodeString = CodeString .. string.format("\t\t\t\tattr = {\n")
					if v[7] then
						CodeString = CodeString .. string.format("\t\t\t\t\tstrength = %d,\n", v[7])
					end
					if v[8] then
						CodeString = CodeString .. string.format("\t\t\t\t\tintellect = %d,\n", v[8])
					end
					if v[9] then
						CodeString = CodeString .. string.format("\t\t\t\t\tagility = %d,\n", v[9])
					end
					if v[10] then
						CodeString = CodeString .. string.format("\t\t\t\t\tcaptain = %d,\n", v[10])
					end
					if v[11] then
						CodeString = CodeString .. string.format("\t\t\t\t\tphyscAtk = %d,\n", v[11])
					end
					if v[12] then
						CodeString = CodeString .. string.format("\t\t\t\t\tmagicAtk = %d,\n", v[12])
					end
					if v[13] then
						CodeString = CodeString .. string.format("\t\t\t\t\tphyscDef = %d,\n", v[13])
					end
					if v[14] then
						CodeString = CodeString .. string.format("\t\t\t\t\tmagicDef = %d,\n", v[14])
					end
					if v[15] then
						CodeString = CodeString .. string.format("\t\t\t\t\thpMax = %g,\n", v[15])
					end
					if v[16] then
						CodeString = CodeString .. string.format("\t\t\t\t\tjink = %g,\n", v[16])
					end
					if v[17] then
						CodeString = CodeString .. string.format("\t\t\t\t\tfatal = %g,\n", v[17])
					end
					if v[18] then
						CodeString = CodeString .. string.format("\t\t\t\t\tcrush = %g,\n", v[18])
					end
					if v[19] then
						CodeString = CodeString .. string.format("\t\t\t\t\tharmDeep = %g,\n", v[19])
					end
					if v[20] then
						CodeString = CodeString .. string.format("\t\t\t\t\tphyscResist = %g,\n", v[20])
					end
					if v[21] then
						CodeString = CodeString .. string.format("\t\t\t\t\tmagicResist = %g,\n", v[21])
					end
					if v[23] then
						CodeString = CodeString .. string.format("\t\t\t\t\tspeed = %g,\n", v[23])
					end
					CodeString = CodeString .. string.format("\t\t\t\t},\n")
					CodeString = CodeString .. string.format("\t\t\t},\n")
				end				
			end
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. "\t},\n"
			CodeStirng = CodeString .. "\t},\n"
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
	local excelfile = "table/fairyDeviceList.xls"
	local outfile = "autocode/fairyDeviceList.lua"
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
