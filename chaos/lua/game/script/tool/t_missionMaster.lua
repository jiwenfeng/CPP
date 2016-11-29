--任务列表
function ParseTable(table)
	local CodeString = "--任务列表\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[\"%s\"] = {\n", Data[1])
			if Data[2] then
				CodeString = CodeString .. string.format("\t\tfore = \"%s\",\n", Data[2])
			end
			if Data[3] then
				CodeString = CodeString .. string.format("\t\tnext = \"%s\",\n", Data[3])
			end
			CodeString = CodeString .. string.format("\t\tclass = \"%s\",\n", Data[4])
			CodeString = CodeString .. string.format("\t\ttype = \"%s\",\n", Data[5])
			CodeString = CodeString .. string.format("\t\tchapter = \"%s\",\n", Data[6])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[7])
			CodeString = CodeString .. string.format("\t\taim = \"%s\",\n", Data[8])
			if Data[9] then
				CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\tneedCopyId = %d,\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\tcontent = \"%s\",\n", Data[11])
			end
			CodeString = CodeString .. string.format("\t\ttarget = {\n")
			for i = 0, 1 do
				if Data[12 + i * 5] then
					CodeString = CodeString .. string.format("\t\t\t{type = \"%s\"", Data[12 + i * 5])
					if Data[13 + i * 5] then
						CodeString = CodeString .. string.format(", mapId = %d", Data[13 + i * 5])
					end
					if Data[14 + i * 5] then
						CodeString = CodeString .. string.format(", npcId = \"%s\"", Data[14 + i * 5])
					end
					if tostring(Data[12 + i * 5] == "func") then
						if Data[15 + i * 5] then
							CodeString = CodeString .. string.format(", id = %d",Data[15 + i * 5])
						end
					else
						if Data[15 + i * 5] then
							CodeString = CodeString .. string.format(", id = \"%s\"",Data[15 + i * 5])
						end
					end
					CodeString = CodeString .. string.format(", num = %d},\n", Data[16 + i * 5])
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tshortDesc = \"%s\",\n", Data[22])
			CodeString = CodeString .. string.format("\t\tprize = {\n")
			if Data[25] then
				CodeString = CodeString .. string.format("\t\t\tgold = %d,\n", Data[25])
			end
			if Data[26] then
				CodeString = CodeString .. string.format("\t\t\texp = %d,\n", Data[26])
			end
			if Data[27] then
				CodeString = CodeString .. string.format("\t\t\tnimbus = %d,\n", Data[27])
			end
			if Data[28] then
				CodeString = CodeString .. string.format("\t\t\tvoucher = %d,\n", Data[28])
			end
			if Data[29] then
				CodeString = CodeString .. string.format("\t\t\tskill = {warrior = \"%s\", taoist = \"%s\", archer = \"%s\"},\n", Data[29], Data[30], Data[31])
			end
			if Data[32] then
				CodeString = CodeString .. string.format("\t\t\tjobPrize = {warrior = %d, taoist = %d, archer = %d, num = %d},\n", Data[32], Data[33], Data[34], Data[35])
			end
			if Data[36] or Data[38] or Data[40] then
				CodeString = CodeString .. "\t\t\titem = {\n"
				for i = 0, 2 do
					if Data[36 + i * 2] then
						CodeString = CodeString .. string.format("\t\t\t\t{id = %d, num = %d},\n", Data[36 + i * 2], Data[37 + i * 2])
					end
				end
				CodeString = CodeString .. "\t\t\t},\n"
			end
			if Data[54] then
				CodeString = CodeString .. string.format("\t\t\tfairyDev = {id = %d, num = %d, Lv = %d},\n", Data[54], Data[55], Data[56])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			CodeString = CodeString .. string.format("\t\tgiven = {\n")
			if Data[42] then
				CodeString = CodeString .. string.format("\t\t\tgold = %d,\n", Data[42])
			end
			if Data[43] then
				CodeString = CodeString .. string.format("\t\t\titem = {id = %d, num = %d},\n", Data[43], Data[44])
			end
			CodeString = CodeString .. string.format("\t\t},\n")
			if Data[45] then
				CodeString = CodeString .. string.format("\t\tstartNpcId = \"%s\",\n", Data[45])
			end
			if Data[46] then
				CodeString = CodeString .. string.format("\t\tstartQ = \"%s\",\n", Data[46])
			end
			if Data[47] then
				CodeString = CodeString .. string.format("\t\tstartA = \"%s\",\n", Data[47])
			end
			if Data[48] then
				CodeString = CodeString .. string.format("\t\tdoingQ = \"%s\",\n", Data[48])
			end
			if Data[49] then
				CodeString = CodeString .. string.format("\t\tdoingA = \"%s\",\n", Data[49])
			end
			if Data[50] then
				CodeString = CodeString .. string.format("\t\tendNpcId = \"%s\",\n", Data[50])
			end
			if Data[51] then
				CodeString = CodeString .. string.format("\t\tendQ = \"%s\",\n", Data[51])
			end
			if Data[52] then
				CodeString = CodeString .. string.format("\t\tendA = \"%s\",\n", Data[52])
			end
			if Data[53] then
				CodeString = CodeString .. string.format("\t\tremark = %d,\n", Data[53])
			end
			if Data[57] then
				CodeString = CodeString .. string.format("\t\tbeginPlot = {plotId = %d", Data[57])
				if Data[58] then
					CodeString = CodeString .. string.format(", monsterId = \"%s\"", Data[58])
				end
				CodeString = CodeString .. "},\n"
			end
			if Data[59] then
				CodeString = CodeString .. string.format("\t\tbossPlot = {plotId = %d", Data[59])
				if Data[60] then
					CodeString = CodeString .. string.format(", monsterId = \"%s\"", Data[60])
				end
				CodeString = CodeString .. "},\n"
			end
			if Data[61] then
				CodeString = CodeString .. string.format("\t\tendPlot = {plotId = %d},\n", Data[61])
			end
			if Data[62] then
				CodeString = CodeString .. string.format("\t\tautoRecv = %d,\n", Data[62])
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
	local excelfile = "table/missionMaster.xls"
	local outfile = "autocode/missionMaster.lua"
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
