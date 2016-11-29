function ParseTable(table)
	local CodeString = "--势力祭坛物品表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			if Data[2] or Data[4] then
				CodeString = CodeString .. "\t\t{"
				if Data[2] then
					CodeString = CodeString .. string.format("type = \"%s\", ", Data[2])
					if Data[3] then
						CodeString = CodeString .. string.format("Lv = %d, ", Data[3])
					end
				elseif Data[4] then	
					CodeString = CodeString .. string.format("goodsId = %d, ", Data[4])
				end
				CodeString = CodeString .. string.format("num = %d, ", Data[5])
				CodeString = CodeString .. string.format("rate = %g},\n", Data[6])
			end
			if Data[7] or Data[9] then
				CodeString = CodeString .. "\t\t{"
				if Data[7] then
					CodeString = CodeString .. string.format("type = \"%s\", ", Data[7])
					if Data[8] then
						CodeString = CodeString .. string.format("Lv = %d, ", Data[8])
					end
				elseif Data[9] then	
					CodeString = CodeString .. string.format("goodsId = %d, ", Data[9])
				end
				CodeString = CodeString .. string.format("num = %d, ", Data[10])
				CodeString = CodeString .. string.format("rate = %g},\n", Data[11])
			end
			if Data[12] or Data[14] then
				CodeString = CodeString .. "\t\t{"
				if Data[12] then
					CodeString = CodeString .. string.format("type = \"%s\", ", Data[12])
					if Data[13] then
						CodeString = CodeString .. string.format("Lv = %d, ", Data[13])
					end
				elseif Data[14] then	
					CodeString = CodeString .. string.format("goodsId = %d, ", Data[14])
				end
				CodeString = CodeString .. string.format("num = %d, ", Data[15])
				CodeString = CodeString .. string.format("rate = %g},\n", Data[16])
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
	local excelfile = "table/phylePrayList.xls"
	local outfile = "autocode/phylePrayList.lua"
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
