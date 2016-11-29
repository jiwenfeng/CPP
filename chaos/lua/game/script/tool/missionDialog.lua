function FormatXlsData(xls)
	local CodeString = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<data>\n"
	for line, Data in ipairs(xls[1]) do --只读第一页
		if line > 1 then
			local str = CodeString .. string.format("\t<mission id=\"%s\">\n", Data[1])
			for i = 0, 5 do
				if Data[3 + i * 2] then
					if not Data[4 + i *2] then
						print("error on line:", line)
					end
					str = str .. string.format("\t\t<dialogue type=\"npc\">\n\t\t\t%s\n\t\t</dialogue>\n", Data[3 + i * 2])
					str = str .. string.format("\t\t<dialogue type=\"user\">\n\t\t\t%s\n\t\t</dialogue>\n", Data[4 + i * 2])
				end
			end			
			str = str .. "\t</mission>\n</data>\n"
			SaveCode(Data[1], str)
		end
	end
end

function SaveCode(file, Code)
	local outfile = string.format("../client/missionDialog/%s.xml", file)
	local fd = io.open(outfile, "w")
	assert(fd)
	fd:write(Code)
	fd:close()
end

function gen()
	local excelfile = "../client/missionDialog.xls"
	local cmd = string.format([[python 'tool/xls2table.py' '%s']], excelfile)
	local fd = io.popen(cmd)
	assert(fd)
	local loadxls = loadstring(fd:read("*a"))
	assert(loadxls)
	loadxls()
	assert(__XLS_END)
	fd:close()
	
	os.execute("mkdir -p ../client/missionDialog")
	FormatXlsData(xlstable)
end
gen()
