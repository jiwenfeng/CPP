

function ReadTxt(path)
	local fd = io.open(path, "r")
	assert(fd, "no such file:" .. path)
	local content = fd:read("*a")
	fd:close()
	local ret = Analyse(content)

	local out = io.open("out.txt", "w")
	assert(out)
	out:write(table.concat(ret, "\n"))
	out:close()
end

local inList = {}
local temp = {}

function AddToList(list, str, flag)
	if not inList[str] then
		inList[str] = true
		table.insert(list, str)
		table.insert(temp, str)
	end
	if #temp >= 20 or flag then
		print(table.concat(temp, "\t"))
		temp = {}
	end
end

function Analyse(content)
	local ret = {}
	local str = content
	local i, p = 1, 1
	local sp = ","
	while true do
		i = string.find(str, sp, p)
		if not i then
			AddToList(ret, string.sub(str, p, -1), true)
			print("end", string.sub(str, p, -1))
			break
		end
		AddToList(ret, string.sub(str, p, i - 1))
		p = i + #sp
	end
	return ret
end

local path = "pbzk.txt"

ReadTxt(path)


