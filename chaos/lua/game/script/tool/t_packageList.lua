function ParseTable (table)
	local CodeString = "--宝箱列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("--\t\tname = \"%s\",\n", Data[2])
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tgold = %d,\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\texp = %d,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\tnimbus = %d,\n", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format("\t\tvoucher = %d,\n", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format("\t\tspar = %d,\n", Data[7])
			end
			
			local start, num, step = 8, 10, 5
			if Data[start] or Data[start + 2] then
				CodeString = CodeString .. "\t\titemList = {\n"
				for i = 0, num - 1, step do
					if Data[start + i * step] or Data[start + i * step + 2] then
						if Data[start + i * step] then
							CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, num = %d, rate = %g},\n", Data[start + i * step], Data[start + i * step + 1], Data[start + i * step + 3], Data[start + i * step + 4])
						else
							CodeString = CodeString .. string.format("\t\t\t{id = %d, num = %d, rate = %g},\n", Data[start + i * step + 2], Data[start + i * step + 3], Data[start + i * step + 4])
						end
					end
				end
				CodeString = CodeString .. "\t\t},\n"
			end			
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
	local excelfile = "table/packageList.xls"
	local outfile = "autocode/packageList.lua"
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
