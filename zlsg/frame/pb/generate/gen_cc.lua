-- generat pb C++ file
local cfg_file = io.open("./config", "r")
local cfg_buffer = cfg_file:read "*a"
cfg_file:close()

local data_file = io.open("./data", "r")
local data_buffer = data_file:read "*a"
data_file:close()

local h_file = io.open("./pb_mgr_base.h", "w+")
local cc_file = io.open("./pb_mgr_base.cc", "w+")

local function gen_h_include(my_buffer)
    local tmp=my_buffer
	while true do
	    i = string.find(tmp, "\n")
		if i == nil then 
			break 
		end
		
		local line = string.sub(tmp, 1, i)
		tmp = string.sub(tmp, i+1)
		
		local pos=string.find(line, "|")
		if pos ~= nil then
		    line = string.sub(line, pos+1)
			pos = string.find(line, "|")
			if pos ~= nil then 
		        local cfg_pb_h="#include \"mysql."..string.sub(line, 1, pos-1)..".pb.h\"\n"
		        h_file:write(cfg_pb_h)
			end
		end
	end
end

local function gen_h_obj(my_buffer)
    local tmp=my_buffer
	while true do
	    i = string.find(tmp, "\n")
		if i == nil then 
			break 
		end
		
		local line = string.sub(tmp, 1, i)
		tmp = string.sub(tmp, i+1)
		
		local pos=string.find(line, "|")
		if pos ~= nil then
		    line = string.sub(line, pos+1)
			pos = string.find(line, "|")
			if pos ~= nil then
		        local obj_pb_name=string.sub(line, 1, pos-1)
		        h_file:write("    mysql::"..obj_pb_name.." mpb_"..obj_pb_name..";\n")
			end
		end
	end
end

local function gen_h()
	h_file:write("#ifndef __PB_CODER_BASE_AUTO_H__\n#define __PB_CODER_BASE_AUTO_H__\n\n")
	h_file:write("#include <iostream>\n#include <sstream>\n#include <string>\n#include <map>\n#include <vector>\n#include <google/protobuf/descriptor.h>\n#include \"define.h\"\n\n")

	h_file:write("//------------cfg pb code------------------------\n")
	gen_h_include(cfg_buffer)
	
	h_file:write("\n//------------data pb code------------------------\n")
	gen_h_include(data_buffer)
	
	h_file:write("\n\n")
	h_file:write("class pb_mgr_base\n{\npublic:\n    pb_mgr_base(){}\n    ~pb_mgr_base(){}\n\n    int base_init();\n\n\nprotected:\n")
	
	h_file:write("    map<string, pb_obj*> cfg_mappbs;\n    map<string, pb_obj*> data_mappbs;\n\n")
	
	h_file:write("//------------cfg pb obj code------------------------\n")
	gen_h_obj(cfg_buffer)
	
	h_file:write("\n//------------data pb obj code------------------------\n")
	gen_h_obj(data_buffer)
	
	h_file:write("};\n\n")
	h_file:write("#endif")
	
end

local function gen_cc_obj(my_buffer,maps)
    local tmp=my_buffer
	while true do
	    i = string.find(tmp, "\n")
		if i == nil then 
			break 
		end
		
		local line = string.sub(tmp, 1, i)
		tmp = string.sub(tmp, i+1)
		
		local pos=string.find(line, "|")
		if pos ~= nil then
		    line = string.sub(line, pos+1)
			pos = string.find(line, "|")
			if pos ~= nil then
				local obj_pb_name=string.sub(line, 1, pos-1)
				local obj_pb="mpb_"..obj_pb_name;
		
				line = string.sub(line, pos+1)
				pos = string.find(line, "|")
				local sql = string.sub(line, 1, pos-1)
		
				cc_file:write("    obj = new pb_obj(&"..obj_pb..", \"\", \""..sql.."\", \""..obj_pb_name.."\");\n")
				cc_file:write("    "..maps..".insert(pair<string, pb_obj*>("..obj_pb..".GetTypeName(), obj));\n")
		
				line = string.sub(line, pos+1)
		
				while true do
					j = string.find(line, "|")
					if j == nil then 
						break 
					end
					local key = string.sub(line, 1, j-1)
					line = string.sub(line, j+1)
					cc_file:write("    obj->add_key(\""..key.."\");\n")
			
				end
				cc_file:write("\n")
			end
		end
	end
end


local function gen_cc()
    cc_file:write("#include \"pb_mgr_base.h\"\n\n")
	cc_file:write("int pb_mgr_base::base_init()\n{\n    pb_obj* obj = NULL;\n")
	
	h_file:write("\n//------------cfg pb code------------------------\n")
	gen_cc_obj(cfg_buffer,"cfg_mappbs")
	
	h_file:write("\n//------------data pb code------------------------\n")
	gen_cc_obj(data_buffer,"data_mappbs")
	
	cc_file:write("    return 0;\n}\n")
end

gen_h()
gen_cc()

h_file.close()
cc_file.close()
