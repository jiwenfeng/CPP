--阵型列表

function ParseTable(xls)
	local t = {"--阵型列表\nlocal autoTable = {\n"}
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			if Data[1] then
				if line ~= 2 then
					table.insert(t, "\t\t},\n")
					table.insert(t, "\t},\n")
				end
				table.insert(t, string.format("\t[\"%s\"] = {\n", Data[1]))
				table.insert(t, string.format("\t\tname = \"%s\",\n", Data[2]))
			--	table.insert(t, string.format("\t\ttype = 3,\n"))
				table.insert(t, string.format("\t\tinitPos = \"%s\",\n", Data[3]))
				table.insert(t, string.format("\t\tneedLv = %d,\n", Data[5]))
				if Data[6] then
					table.insert(t, string.format("\t\tneedGold = %d,\n", Data[6]))
				end
				if Data[7] then
					table.insert(t, string.format("\t\tneedNimbus = %d,\n", Data[7]))
				end
				table.insert(t, string.format("\t\texplain = \"%s\",\n", Data[8]))
				if Data[31] then
					table.insert(t, string.format("\t\trestraintDesc = \"%s\",\n", Data[31]))
				end
				if Data[33] then
					table.insert(t, string.format("\t\tneedCity = %d,\n", Data[33]))
				end
				table.insert(t, string.format("\t\trestraint = {\n"))
				if Data[9] then
					table.insert(t, string.format("\t\t\t[\"%d\"] = %g,\n", Data[9], Data[10]))
				end
				if Data[11] then
					table.insert(t, string.format("\t\t\t[\"%d\"] = %g,\n", Data[11], Data[12]))
				end
				table.insert(t, "\t\t},\n")
				table.insert(t, "\t\tlinkSkill = {")
				if Data[13] then
					table.insert(t, string.format("\"%s\"", (Data[13])))
				end
				if Data[32] then
					if Data[13] then table.insert(t, ", ") end
					table.insert(t, string.format("\"%s\"", (Data[32])))
				end
				table.insert(t, "},\n")
				if Data[33] then
					table.insert(t, string.format("\t\tisNeedCity = 1,\n"))
				end
				table.insert(t, string.format("\t\tlist = {\n"))
			end
			table.insert(t, string.format("\t\t\t[%d] = {\n", Data[14]))
			if Data[15] then
				table.insert(t, string.format("\t\t\t\tlinkRate = %g,\n", Data[15]))
			end
			table.insert(t, "\t\t\t\tattr = {\n")
			if Data[16] then
				table.insert(t, string.format("\t\t\t\t\thpMax = %d,\n", Data[16]))
			end
			if Data[17] then
				table.insert(t, string.format("\t\t\t\t\tphyscAtk = %d,\n", Data[17]))
			end
			if Data[18] then
				table.insert(t, string.format("\t\t\t\t\tmagicAtk = %d,\n", Data[18]))
			end
			if Data[19] then
				table.insert(t, string.format("\t\t\t\t\tphyscDef = %d,\n", Data[19]))
			end
			if Data[20] then
				table.insert(t, string.format("\t\t\t\t\tmagicDef = %d,\n", Data[20]))
			end
			if Data[21] then
				table.insert(t, string.format("\t\t\t\t\tspeed = %g,\n", Data[21]))
			end
			if Data[22] then
				table.insert(t, string.format("\t\t\t\t\tfatal = %g,\n", Data[22]))
			end
			if Data[23] then
				table.insert(t, string.format("\t\t\t\t\tcrush = %g,\n", Data[23]))
			end
			if Data[24] then
				table.insert(t, string.format("\t\t\t\t\tharmDeep = %g,\n", Data[24]))
			end			
			table.insert(t, "\t\t\t\t},\n")
			if Data[4] then
				table.insert(t, string.format("\t\t\t\tposList = {%s}, \n", Data[4]))
			end
			if Data[25] then
				table.insert(t, string.format("\t\t\t\tdescribe = \"%s\",\n", Data[25] or ""))
			end
			if Data[27] then
				table.insert(t, string.format("\t\t\t\tneedGold = %g,\n", Data[27]))
			end
			if Data[28] then
				table.insert(t, string.format("\t\t\t\tneedNimbus = %g,\n", Data[28]))
			end
			if Data[29] and Data[30] then
				table.insert(t, string.format("\t\t\t\tneedItem = {goodsId = %d, num = %d},\n", Data[29], Data[30]))
			end
			table.insert(t, "\t\t\t},\n")
			

		end
	end
	table.insert(t, string.format("\t\t},\n"))
	table.insert(t, string.format("\t},\n"))
	table.insert(t, "}\n\n")
	table.insert(t, "function GetTable()\n\treturn autoTable\nend\n")
	return table.concat(t)
end

function SaveCode(file, Code)
	local fd = io.open(file, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "table/formation.xls"
	local outfile = "autocode/formationList.lua"
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
