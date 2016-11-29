--科技列表

function ParseTable(xls)
	local t = {"--科技列表\nlocal autoTable = {\n"}
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			if Data[1] then
				if line ~= 2 then
					table.insert(t, "\t\t},\n")
					table.insert(t, "\t},\n")
				end
				table.insert(t, string.format("\t[\"%s\"] = {\n", Data[1]))
				table.insert(t, string.format("\t\tname = \"%s\",\n", Data[2]))
				table.insert(t, string.format("\t\ttype = %d,\n", Data[3]))
				table.insert(t, string.format("\t\tlist = {\n"))
			end
			if Data[4] then
				table.insert(t, string.format("\t\t\t[%d] = {\n", Data[4]))
				if Data[5] then
					table.insert(t, string.format("\t\t\t\tdescribe = \"%s\",\n", Data[5]))
				end
				table.insert(t, "\t\t\t\tattr = {\n")
				if Data[6] then
					table.insert(t, string.format("\t\t\t\t\tgoldRate = %d,\n", Data[6]))
				end
				if Data[7] then
					table.insert(t, string.format("\t\t\t\t\ttradeRate = %g,\n", Data[7]))
				end
				if Data[8] then
					table.insert(t, string.format("\t\t\t\t\twoodRate = %g,\n", Data[8]))
				end
				if Data[9] then
					table.insert(t, string.format("\t\t\t\t\tstoneRate = %g,\n", Data[9]))
				end
				if Data[10] then
					table.insert(t, string.format("\t\t\t\t\tironRate = %d,\n", Data[10]))
				end
				if Data[11] then
					table.insert(t, string.format("\t\t\t\t\tsparRate = %g,\n", Data[11]))
				end
				if Data[12] then
					table.insert(t, string.format("\t\t\t\t\ttimeCut = %g,\n", Data[12]))
				end
				if Data[13] then
					table.insert(t, string.format("\t\t\t\t\tphysicAtk = %g,\n", Data[13]))
				end
				if Data[14] then
					table.insert(t, string.format("\t\t\t\t\tphysicDef = %g,\n", Data[14]))
				end
				if Data[15] then
					table.insert(t, string.format("\t\t\t\t\tspeed = %g,\n", Data[15]))
				end
				if Data[16] then
					table.insert(t, string.format("\t\t\t\t\tlinkRate = %g,\n", Data[16]))
				end
				if Data[17] then
					table.insert(t, string.format("\t\t\t\t\tforayNum = %g,\n", Data[17]))
				end
				if Data[18] then
					table.insert(t, string.format("\t\t\t\t\tforayTimeCut = %g,\n", Data[18]))
				end
				if Data[22] then
					table.insert(t, string.format("\t\t\t\t\tmagicAtk = %g,\n", Data[22]))
				end
				if Data[23] then
					table.insert(t, string.format("\t\t\t\t\tmagicDef = %g,\n", Data[23]))
				end
				if Data[24] then
					table.insert(t, string.format("\t\t\t\t\thpMax = %g,\n", Data[24]))
				end
				table.insert(t, "\t\t\t\t},\n")
				if Data[20] then
					table.insert(t, string.format("\t\t\t\tneedSpar = %d,\n", Data[20]))
				end
				if Data[21] then
					table.insert(t, string.format("\t\t\t\ttimelong = %d,\n", Data[21]))
				end
				table.insert(t, "\t\t\t},\n")
			end
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
	local excelfile = "table/techList.xls"
	local outfile = "autocode/techList.lua"
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
