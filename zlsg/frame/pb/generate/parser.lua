-- get all sql from sql file
local sql_file = io.open("./wj", "r")
local buffer = sql_file:read "*a"
sql_file:close()

local tables = {}
local pattern = {bigint = "uint64", int = "uint32", datetime = "string", longtext = "string", varchar = "string", time="string"
			, float="float", smallint="int32", double="double", date="string", tinyint="int32", char="string",text="string",blob="bytes", mediumblob="bytes", decimal="double"} 

local function parse_key_val(index, line, table_file)
	local i,j = string.find(line, "%`.-%`")
	local key = string.sub(line, i+1, j-1)
	
	line = string.sub(line, j+1)
	i,j = string.find(line, " .- ")
	if j == nil then
	    return
	end

	local ttype = string.sub(line, i+1, j-1)
	i,j = string.find(ttype, "%(.-%)")
	if j ~= nil then
	    ttype = string.sub(ttype, 1, i-1)
	end
	local val = pattern[ttype]
	if val ~= nil then
	    table_file:write("    required "..val.." "..key.." = "..index..";\n")
	    --print("    required "..val.." "..key.." = "..index..";")
	end

end

local function parse_table(table_name,buf)
    local i = 0
	local count = 0
	
	local name="./mysql."..table_name..".proto"
	local table_file = io.open(name, "w+") 
	
	table_file:write("\npackage mysql;\n\n")
	--print("\npackage mysql;\n\n")
	
	table_file:write("message "..table_name.."{\n")
	--print("message "..table_name.."{")
	
	while true do
	    i = string.find(buf, "\n")
		if i == nil then
		    break;
		end
		
		local line = string.sub(buf, 1, i)
		buf = string.sub(buf, i+1)
		local j = string.find(line, " %`.-%` ")
		if j ~= nil then
		    count = count+1
		    parse_key_val(count,line, table_file)
		else 
		    break
		end

	end
	
	table_file:write("}\n")
	--print("}\n")
	
	table_file:close()
	
	
	return buf
end

--

local function parse()
    local i = 0
	while true do
	    i = string.find(buffer, "\n")
		if i == nil then 
			break 
		end
		
		local line = string.sub(buffer, 1, i)
		buffer = string.sub(buffer, i+1)
		
		local j,k = string.find(line, "CREATE TABLE %`")
		if k ~= nil then
		    local table_name = string.sub(line, k+1)
			k = string.find(table_name, "%`")
			table_name = string.sub(table_name, 1, k-1)
			buffer = parse_table(table_name, buffer)
		end
		
	end
end

parse()
