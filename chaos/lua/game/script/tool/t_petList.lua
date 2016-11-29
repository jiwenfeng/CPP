function ParseTable(table)
	local CodeString = "--本命精灵表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tskillId = \"%d\",\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tskillName = \"%s\",\n", Data[4])
			end
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[5] or "")
			CodeString = CodeString .. string.format("\t\tmodelList = {\n")
			if Data[6] and Data[7] then
				CodeString = CodeString .. string.format("\t\t\t[0] = {headId = %d, modelId = %d},\n", Data[6], Data[7])
			end
			if Data[8] and Data[9] then
				CodeString = CodeString .. string.format("\t\t\t[1] = {headId = %d, modelId = %d},\n",Data[8], Data[9])
			end
			if Data[10] and Data[11] then
				CodeString = CodeString .. string.format("\t\t\t[2] = {headId = %d, modelId = %d},\n",Data[10], Data[11])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
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
	local excelfile = "table/petList.xls"
	local outfile = "autocode/petList.lua"
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
