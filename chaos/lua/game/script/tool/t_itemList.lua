function ParseTable(table)
	local CodeString = "--普通道具表\n local autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		--第一行不读
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tname = \"%s\",\n", Data[2])
			CodeString = CodeString .. string.format("\t\tquality = %d,\n", Data[3])
			if Data[4] then
				CodeString = CodeString .. string.format("\t\tLv = %d,\n", Data[4])
			end
			CodeString = CodeString .. string.format("\t\tneedLv = %d,\n", Data[5])
	--		CodeString = CodeString .. string.format("\t\ttype=%d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\tclass = \"%s\",\n", Data[7])
			if Data[8] then
	--			CodeString = CodeString .. string.format("\t\tstore=%d,\n", Data[8])
			end
			if Data[9] then
				CodeString = CodeString .. string.format("\t\tmall = %d,\n", Data[9])
			end
			if Data[10] then
				CodeString = CodeString .. string.format("\t\tisHot = %g,\n", Data[10])
			end
			if Data[11] then
				CodeString = CodeString .. string.format("\t\tneedGold = %d,\n", Data[11])
			end
			if Data[12] then
				CodeString = CodeString .. string.format("\t\tneedIngot = %d,\n", Data[12])
			end
			if Data[13] then
				CodeString = CodeString .. string.format("\t\tneedVoucher = %d,\n", Data[13])
			end
			if Data[14] then
				CodeString = CodeString .. string.format("\t\trecycleGold = %d,\n", Data[14])
			end
			if Data[15] then
				CodeString = CodeString .. string.format("\t\trecycleIngot = %d,\n", Data[15])
			end
			if Data[16] then
				CodeString = CodeString .. string.format("\t\tuseVoucher = %d,\n", Data[16])
			end
			if Data[17] then
				CodeString = CodeString .. string.format("\t\tclickUse = %d,\n", Data[17])
			end
			if Data[18] then
				CodeString = CodeString .. string.format("\t\ttarget = %d,\n", Data[18])
			end
			if Data[19] then
				CodeString = CodeString .. string.format("\t\tlock = %d,\n", Data[19])
			end
			CodeString = CodeString .. string.format("\t\tdescribe = \"%s\",\n", Data[20] or "")
			if Data[21] then
				CodeString = CodeString .. string.format("\t\texplain = \"%s\",\n", Data[21])
			end
			if Data[22] then
				CodeString = CodeString .. string.format("\t\tfuncId = %d,\n", Data[22])
			end
			if Data[23] then
				CodeString = CodeString .. string.format("\t\tbuffId = \"%d\",\n", Data[23])
			end
			if Data[24] then
				CodeString = CodeString .. string.format("\t\tnum = %g,\n", Data[24])
			end
			if Data[25] then
				CodeString = CodeString .. string.format("\t\ttimelong = %d,\n", Data[25])
			end
		--[[	CodeString = CodeString .. "\t\tattr={\n"
			if Data[26] then
				CodeString = CodeString .. string.format("\t\t\tstrength=%d,\n", Data[26])
			end
			if Data[27] then
				CodeString = CodeString .. string.format("\t\t\tagility=%d,\n", Data[27])
			end
			if Data[28] then
				CodeString = CodeString .. string.format("\t\t\tintellect=%d,\n", Data[28])
			end
			if Data[29] then
				CodeString = CodeString .. string.format("\t\t\tcaptain=%d,\n", Data[29])
			end
			CodeString = CodeString .. "\t\t},\n"]]
			if Data[30] then
				CodeString = CodeString ..string.format("\t\tareaType = \"%s\",\n", Data[30])
			end
			if Data[31] then
				CodeString = CodeString ..string.format("\t\tkind = %d,\n", Data[31])
			end
			CodeString = CodeString ..string.format("\t\ttype = \"%s\",\n", Data[32])
			
			CodeString = CodeString .. "\t},\n"
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
	local excelfile = "table/itemList.xls"
	local outfile = "autocode/itemList.lua"
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
