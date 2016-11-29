--属性培养
function ParseTable(table)
	local CodeString = "--属性培养\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {", Data[1])
			CodeString = CodeString .. string.format("%d,", Data[2])
			CodeString = CodeString .. string.format(" %d,", Data[3])
			CodeString = CodeString .. string.format(" %d,", Data[4])
			CodeString = CodeString .. string.format(" %d,", Data[5])
			CodeString = CodeString .. string.format(" %d,", Data[6])
			CodeString = CodeString .. string.format(" %d,", Data[7])
			CodeString = CodeString .. string.format(" %d,", Data[8])
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
	local excelfile = "table/attrTrain.xls"
	local outfile = "autocode/attrTrain.lua"
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
