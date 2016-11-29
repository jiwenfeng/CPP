function ParseTable(table)
	local CodeString = "--怪物图鉴材料表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\trow = {\n")
			for i = 0, 2 do
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", num = %d},\n", Data[2 + i * 2], Data[3 + i * 2])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tcol = {\n")
			for i = 0, 2 do
				CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", num = %d},\n", Data[8 + i * 2], Data[9 + i * 2])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/monAlbumAttr.xls"
	local outfile = "autocode/monAlbumAttr.lua"
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
