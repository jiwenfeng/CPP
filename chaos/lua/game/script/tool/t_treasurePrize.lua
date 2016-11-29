--寻宝奖励物品表
function ParseTable(table)
	local CodeString = "--寻宝奖励\nlocal autoTable = {\n"
	for line, Data in ipairs(table[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			--for i = 0, 40 do
			local i = 2
			while Data[i] do
				--if Data[2 + i] then
				--	if i % 5 == 0 then
				--		CodeString = CodeString .. "\n\t\t"
				--	end
				--	CodeString = CodeString .. string.format("%d, ", Data[2 + i])
				--end
				CodeString = CodeString .. string.format("\t\t{id = %d, num = %d},\n", Data[i], Data[i + 1])
				i = i + 2
			end
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
	local excelfile = "table/treasurePrize.xls"
	local outfile = "autocode/treasurePrize.lua"
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
