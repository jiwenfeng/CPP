--充值奖励表
local tbList = {[16] = "taoist", [18] = "archer", [20] = "warrior"}
function ParseTable(xls)
	local CodeString = "--充值奖励列表\nlocal autoTable = {\n"
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			CodeString = CodeString .. string.format("\t[%d] = {\n", Data[1])
			CodeString = CodeString .. string.format("\t\tneedIngot = %d,\n", Data[2])
			CodeString = CodeString .. string.format("\t\tpicId = %d,\n", Data[4])
			CodeString = CodeString .. string.format("\t\tname = '%s',\n", Data[5])
			CodeString = CodeString .. string.format("\t\tprice = %d,\n", Data[6])
			CodeString = CodeString .. string.format("\t\titemList = {\n")
			if Data[10] then
				CodeString = CodeString .. string.format("\t\t\t{warrior = %d, taoist = %d, archer = %d, num = %d},\n", Data[7], Data[8], Data[9], Data[10])
			end
			for i = 0, 7 do
				if Data[11 + 3 * i] then
					CodeString = CodeString .. string.format("\t\t\t{goodsId = %d, num = %d},\n", Data[11 + 3 *i], Data[12 + i * 3])
				end
			end
			CodeString = CodeString .. string.format("\t\t},\n")
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
	local excelfile = "table/rechargePrizeList.xls"
	local outfile = "autocode/rechargePrizeList.lua"
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
