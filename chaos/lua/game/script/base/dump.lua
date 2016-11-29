function DumpFile(FromFile, ToFile)
        local s1 = posix.stat(FromFile)
        local s2 = posix.stat(ToFile)
        if s1 and s2 then
                if s2.mtime >= s1.mtime then
                        return
                end
        end

        local fh = io.open(FromFile)

        local FileData = fh:read("*a")

        local func, err = loadstring(FileData, FromFile)
        assert(func, err)
        Touch(ToFile)
        local Handler = io.open(ToFile,"w+")
        if not Handler then
                print("no such file", ToFile)
                return
        end
        Handler:write(string.dump(func))
        Handler:close()
end

function DumpForOneFile(PathFile, ToPathFile)
	if string.endswith(PathFile, ".lua") then
		DumpFile(PathFile, ToPathFile)
	elseif string.endswith(PathFile, ".log") then
		return
	else --其他类型的文件直接copy过去
		Touch(ToPathFile)
		os.execute(string.format("cp -r %s %s", PathFile, ToPathFile))
	end
end

function DumpPath(SrcPath, ToPath)
	os.execute(string.format("rm -rf %s/*", ToPath))
	posix.mkdir(ToPath)
	for file in posix.files(SrcPath) do
		if file ~= "." and file ~= ".." and file ~=".svn" and file ~= "excel" and file ~= "tool" then
			local PathFile = SrcPath.."/"..file
			local ToPathFile = ToPath.."/"..file
			local filetype = posix.stat(PathFile).type
			if filetype == "directory" then
				DumpPath(PathFile, ToPathFile)
			--文件类型
			elseif filetype == "regular" then
				DumpForOneFile(PathFile, ToPathFile)
			else
				print("filetype:"..filetype..",filename:"..PathFile )
			end
		end
	end
end
