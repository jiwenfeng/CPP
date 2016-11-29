--技能效果列表

function ParseTable(table)
	local CodeString = "--技能效果列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[3]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			end
		--	CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tIsBuffer = 1,\n")
				CodeString = CodeString .. string.format("\t\tstate = \"%s\",\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tIsPassive = 1,\n")
				CodeString = CodeString .. string.format("\t\tattr = \"%s\",\n", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format("\t\tIsActive = 1,\n")
			end
			CodeString = CodeString .. string.format("\t},\n")
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
	local excelfile = "table/skillList.xls"
	local outfile = "autocode/skillEffect.lua"
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
