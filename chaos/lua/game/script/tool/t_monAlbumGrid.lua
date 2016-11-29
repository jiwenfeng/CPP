function ParseTable(table)
	local CodeString = "--怪物图鉴格子表\n local autoTable = {\n"
	local index
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			if not index then
				CodeString = CodeString .. string.format("\t[%s] = {\n", Data[1])
			elseif index ~= Data[1] then
				CodeString = CodeString .. "\t},\n"
				CodeString = CodeString .. string.format("\t[%s] = {\n", Data[1])
			end
			index = Data[1]
			CodeString = CodeString .. string.format("\t\t[%d] = %d,\n", Data[2], Data[3])
		end
	end
	CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/monAlbumGrid.xls"
	local outfile = "autocode/monAlbumGrid.lua"
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
