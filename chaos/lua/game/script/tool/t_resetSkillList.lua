function ParseTable(xls)
	local CodeString = "--可重置技能列表\nlocal autoTable = {\n"
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\t[1] = {\"%s\"", Data[2])
			for i = 3, 6 do
				if Data[i] then
					CodeString = CodeString .. string.format(", \"%s\"", Data[i])
				end
			end
			CodeString = CodeString .. string.format("},\n")
			CodeString = CodeString .. string.format("\t\t[2] = {\"%s\"", Data[7])
			for i = 8, 13 do
				if Data[i] then
					CodeString = CodeString .. string.format(", \"%s\"", Data[i])
				end
			end
			CodeString = CodeString .. string.format("},\n")
			CodeString = CodeString .. string.format("\t\t[3] = {\"%s\"", Data[14])
			for i = 15, 18 do
				if Data[i] then
					CodeString = CodeString .. string.format(", \"%s\"", Data[i])
				end
			end
			CodeString = CodeString .. "\t},\n"
			if Data[22] then
				CodeString = CodeString .. string.format("\t\t[4] = {\"%s\"", Data[22])
				for i = 23, 24 do
					if Data[i] then
						CodeString = CodeString .. string.format(", \"%s\"", Data[i])
					end
				end
				CodeString = CodeString .. string.format("},\n")
			end
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
	local excelfile = "table/resetSkillList.xls"
	local outfile = "autocode/resetSkillList.lua"
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
