function ParseTable(table)
	local CodeString = "--功能开启表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			if Data[1] then
				CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
				CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
				if Data[3] then
					CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[3])
				end
				if Data[5] then
					CodeString = CodeString .. string.format("\t\torder = %d,\n", Data[5])
				end
				if Data[6] or Data[7] then
					CodeString = CodeString .. string.format("\t\tcondt = {\n")
					if Data[6] then
						CodeString = CodeString .. string.format("\t\t\tneedLv = %d,\n", Data[6])
					end
					if Data[7] then
						CodeString = CodeString .. string.format("\t\t\tneedMissionId = %d,\n", Data[7])
					end
					CodeString = CodeString .. string.format("\t\t},\n")
				end
				CodeString = CodeString .. "\t},\n"
			end
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
	local excelfile = "table/funcOpenList.xls"
	local outfile = "autocode/funcOpenList.lua"
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
