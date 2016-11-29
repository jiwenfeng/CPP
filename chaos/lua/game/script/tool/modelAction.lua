dofile("base/extend.lua")
dofile("base/json.lua")

function FormatXlsData(xls)
	local list = {}
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 2 then
			local buff = {}
			buff.id = string.format("%s", Data[1])
			if Data[2] then
				buff.x = tonumber(Data[2])
			end
			if Data[3] then
				buff.y = tonumber(Data[3])
			end
			if Data[4] then
				buff.width = tonumber(Data[4])
			end
			if Data[5] then
				buff.height = tonumber(Data[5])
			end
			buff.hurt = tonumber(Data[6])
			table.insert(list, buff)
		end
	end
	SaveCode(list)
end

function SaveCode(t, file)
	local outfile = string.format("../client/%s.txt", file or "modelAction")
	local fd = io.open(outfile, "w")
	assert(fd)
	fd:write(encode(t))
	fd:close()
end

function gen()
	local excelfile = "../client/modelAction.xls"
	local cmd = string.format([[python 'tool/xls2table.py' '%s']], excelfile)
	local fd = io.popen(cmd)
	assert(fd)
	local loadxls = loadstring(fd:read("*a"))
	assert(loadxls)
	loadxls()
	assert(__XLS_END)
	fd:close()
	
	FormatXlsData(xlstable)
end
gen()
