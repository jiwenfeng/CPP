--在线奖励
function ParseTable(xls)
	local CodeString = "--在线奖励列表\nlocal autoTable = {\n"
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tnext = %d,\n",Data[2])
			end
			CodeString = CodeString .. string.format("\t\ttimelong = %d,\n",Data[3])
			CodeString = CodeString .. string.format("\t\titemList = {\n")
			local i = 4
			while(Data[i] and Data[i + 1]) do
				CodeString = CodeString .. string.format("\t\t\t{id = %d, num = %d},\n", Data[i], Data[i + 1])
				i = i + 2
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. "\t},\n"
		end
	end
	CodeString = CodeString .. string.format("}\n\nfunction GetTable()\n\treturn autoTable\nend")
	return CodeString
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/onlinePrize.xls"
	local outfile = "autocode/onlinePrize.lua"
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
