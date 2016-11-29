--城池列表
function ParseTable(table)
	local CodeString = "--城市列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tphyle = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tneighber = {")
			CodeString = CodeString .. string.format("\"%s\"", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[4])
			end
			if Data[5] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[5])
			end
			if Data[6] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[6])
			end
			if Data[7] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[7])
			end
			if Data[8] then
				CodeString = CodeString .. string.format(", \"%s\"", Data[8])
			end
			CodeString = CodeString .. string.format("},\n")
			CodeString = CodeString .. string.format("\t\tinitLv = %d,\n", Data[10])
			CodeString = CodeString .. string.format("\t\tlimitLv = %d,\n", Data[11])
			if Data[12] then
				CodeString = CodeString .. string.format("\t\tgodHero = %d,\n", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\tisMain = %d,\n", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\tposX = %d,\n", Data[15])
			end
			if Data[16] then
				CodeString = CodeString .. string.format("\t\tposY = %d,\n", Data[16])
			end
			if Data[17] then
	--			CodeString = CodeString .. string.format("\t\tpicId = %d,\n", Data[17])
			end
			if Data[18] then
				CodeString = CodeString .. string.format("\t\tlinker = {")
				CodeString = CodeString .. string.format("\"%s\"", Data[18])
				for i = 1, 5 do
					if Data[18 + i] then
						CodeString = CodeString .. string.format(", \"%s\"", Data[18 + i])
					end
				end
				CodeString = CodeString .. string.format("},\n")
				CodeString = CodeString .. string.format("\t\tlinkSkill = \"%s\",\n", Data[24])
		--		CodeString = CodeString .. string.format("\t\tskillTarget = %d,\n", Data[25])
			end
			CodeString = CodeString .. string.format("\t},\n")
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
	local excelfile = "table/cityList.xls"
	local outfile = "autocode/cityList.lua"
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
