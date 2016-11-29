function ParseTable(table)
	local CodeString = "--本命精灵天赋技能表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tinit = %g,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tmax = %g,\n", Data[3])
			CodeString = CodeString .. "\t\tattrList = {\n"
			if Data[4] then
				CodeString = CodeString .. string.format("\t\t\tphyscAtk = %g,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\t\tmagicAtk = %g,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\t\tphyscDef = %g,\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\t\tmagicDef = %g,\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\t\tmorale = %d,\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\t\trage = %d,\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\t\taddExp = %g,\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\t\treduceRes = %g,\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\t\tcdTime = %g,\n", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\t\tAddRes = %g,\n", Data[13])
			end
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. string.format("\t\tdescrip = \"%s\",\n", Data[14])
			CodeString = CodeString .. "\t\tuplimit = {\n"
			local i = 15 
			while Data[i] and Data[i] + 1 do
				CodeString = CodeString .. string.format("\t\t\t[%d] = %g,\n", Data[i], Data[i + 1])
				i = i + 2
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
	local excelfile = "table/petTalentList.xls"
	local outfile = "autocode/petTalentList.lua"
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
