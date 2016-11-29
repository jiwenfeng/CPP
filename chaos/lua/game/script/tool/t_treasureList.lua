function ParseTable(table)
	local CodeString = "--藏宝点表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tspadeList = {\n")
			if Data[3] then
				CodeString = CodeString .. string.format("\t\t\t[%d] = 1,\n", Data[3])
			end
			if Data[4] then
				CodeString = CodeString .. string.format("\t\t\t[%d] = 1,\n", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format("\t\t\t[%d] = 1,\n", Data[5])
			end
			CodeString = CodeString .. string.format("\t\t},\n", Data[3])
			CodeString = CodeString .. string.format("\t\tmapId = %d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\tpicId = %d,\n", Data[7])
	--[[		CodeString = CodeString .. "\t\tshowList = {"
			for i = 1, 5 do
				if Data[i + 6] then
					CodeString = CodeString .. string.format("%s,", Data[i + 6])
				end
			end
			CodeString = CodeString .. "},\n"
			CodeString = CodeString .. "\t\tshowType = {"
			CodeString = CodeString .. string.format("%d", Data[12])
			if Data[13] then
				CodeString = CodeString .. string.format(", %d", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format(", %d", Data[14])
			end
			CodeString = CodeString .. "},\n"]]
			CodeString = CodeString .. "\t\trateList = {\n"
			local curSpade, curLine
			for k, v in ipairs(table[2]) do
				if k > 2 then
					if v[1] == Data[1] then
						if curSpade and curSpade ~= v[2] then
							CodeString = CodeString .. "\t\t\t\t\t},\n"
							CodeString = CodeString .. "\t\t\t\t},\n"
							CodeString = CodeString .. "\t\t\t},\n"
							CodeString = CodeString .. string.format("\t\t\t[%d] = {\n", v[2])
							curSpade = v[2]
							curLine = nil
						elseif not curSpade then
							CodeString = CodeString .. string.format("\t\t\t[%d] = {\n", v[2])
							curSpade = v[2]
						end
						if curLine and curLine ~= v[3] then
							CodeString = CodeString .. "\t\t\t\t\t},\n"
							CodeString = CodeString .. "\t\t\t\t},\n"
						--	CodeString = CodeString .. "\t\t\t},\n"
							CodeString = CodeString .. string.format("\t\t\t\t[%d] = {\n", v[3])
							curLine = v[3]
						elseif not curLine then
							CodeString = CodeString .. string.format("\t\t\t\t[%d] = {\n", v[3])
							curLine = v[3]
						end
						CodeString = CodeString .. string.format("\t\t\t\t\tmin = %d,\n", v[4])
						if v[5] then
							CodeString = CodeString .. string.format("\t\t\t\t\tmax = %d,\n", v[5])
						else
							CodeString = CodeString .. string.format("\t\t\t\t\tmax = math.huge,\n")
						end
						CodeString = CodeString .. "\t\t\t\t\tprizeList = {\n"
						for i = 1, 18 do
							if v[5 + i] then
								CodeString = CodeString .. string.format("\t\t\t\t\t\t{type = %d, rate = %g},\n", i, v[5+i])
							end
						end
					end
				end
			end
			CodeString = CodeString .. "\t\t\t\t\t},\n"
			CodeString = CodeString .. "\t\t\t\t},\n"
			CodeString = CodeString .. "\t\t\t},\n"
			CodeString = CodeString .. "\t\t},\n"
			CodeString = CodeString .. "\t},\n"
		end
	end
	CodeString = CodeString .. "}\n\nfunction GetTable()\n\treturn autoTable\nend\n"
	return CodeString
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/treasureList.xls"
	local outfile = "autocode/treasureList.lua"
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
