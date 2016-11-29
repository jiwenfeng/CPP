function ParseTable (table)
	local CodeString = "--宝箱列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
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
			CodeString = CodeString .. string.format("\t\tmin = %d,\n", Data[8])
			CodeString = CodeString .. string.format("\t\tmax = %d,\n", Data[9])
			CodeString = CodeString .. "\t\titemList = {\n"
			local n = 4
			for i = 0, 5, n do
				if Data[10 + i * n] or Data[10 + i * n + 2] then
					if Data[10 + i * n] then
						CodeString = CodeString .. string.format("\t\t\t{type = \"%s\", Lv = %d, rate = %g},\n", Data[10 + i * n], Data[10 + i * n + 1], Data[10 + i * n + 3])
					else
						CodeString = CodeString .. string.format("\t\t\t{id = %d, rate = %g},\n", Data[10 + i * n + 2], Data[10 + i * n + 3])
					end
				end
			end
			CodeString = CodeString .. "\t\t},\n"
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
	local excelfile = "table/chestList.xls"
	local outfile = "autocode/chestList.lua"
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
