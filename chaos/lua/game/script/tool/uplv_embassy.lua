function ParseTable(table)
	local CodeString = "--使馆升级表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tcondt = {\n")
			if Data[2] then
				CodeString = CodeString .. string.format("\t\t\tneedMainLv = %d,\n", Data[2])
			end
			if Data[3] then
				CodeString = CodeString .. string.format("\t\t\tneedGold = %d,\n", Data[3])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[4] then
				CodeString = CodeString .. string.format("\t\ttimelong = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\texpAdd = %g,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tattrAdd = %g,\n", Data[7])
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
	local excelfile = "table/uplv_embassy.xls"
	local outfile = "autocode/uplv_embassy.lua"
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
