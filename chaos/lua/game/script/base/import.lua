--使用方法Import("base/util.lua")
--代替Lua本身的module, require机制
_G._ImportModule = _G._ImportModule or {}
local _ImportModule = _G._ImportModule
local ModuleArray = {} --这个只是为了维护一个载入顺序， SystemStartup 函数希望能按Import的顺序执行

--用Src里面的内容递归替换Dest里面的内容
--目的是保持table的所有引用继续有效

local function ReplaceTbl(Dest, Src)
	local function RealFun(Dest, Src, Depth)
		assert(type(Dest)=="table" and type(Src)=="table",
			"ReplaceTbl error data type", Dest, Src)

		if not Depth then
			Depth = 0
		end
		if Depth>=20 then
			error("too long Depth to replace")
			return
		end

		for k,v in pairs(Dest) do
			if type(v) == "table" then
				--不更新 __SubClass
				if k ~= "__SubClass" then
					--__index and __SuperClass 不做deep替换
					if type(Src[k]) == "table" and k ~="__index" and k ~= "__SuperClass" then
						RealFun(v, Src[k], Depth+1)
					else
						Dest[k] = Src[k]
					end
				end

			else
				Dest[k] = Src[k]
			end
		end

		--把新增的数据或者方法引用进来
		for k,v in pairs(Src) do
			--注意一定要使用rawget，否则在以下情况更新会失败.
			--父类A实现了test函数，子类A1未实现，然后在线添加A1.test的实现并更新。
			--这时候Dest[k]实际上访问了metatable中父类的实现，所以不为nil，于是更新失败
			if rawget(Dest, k) == nil then
				Dest[k] = v
			end
		end


		setmetatable(Dest, getmetatable(Src))
	end
	RealFun(Dest, Src)
end

--不抛出错误的Import

local function GetAllClass (Module)
	local ClassList = {}
	for k, v in pairs (Module) do
		if type(v) == "table" then
			--如果是一个class则需要全部更新,其他则可能只是一些数据，不需要更新
			if v.__ClassType then
				local mt = getmetatable(v)
				--如果是一个对象数据则不需要更新
				if not (mt and  mt.__ObjectType) then
					ClassList[k] = v
				end
			end
		end
	end

	return ClassList
end

local function PropagateInherit (NewClassList, OldClassList)
	for k, v in pairs (NewClassList) do
		v:Update (OldClassList[k])
	end
end

--Update以后必须保证两个事情:
--1. function env不会改变，因为有大量的逻辑是依赖于fenv的，比如callout等
--2. module里面的table的引用继续有效，并且能够及时更新到。

--暂时不考虑local func = xxx.foo这样的引用的更新。
--目前的实现是把原来的module直接当成新的func的env来运行保证第一点
--第二点的实现不怎么优雅，继续使用replace的办法。
local function SafeImport(PathFile, Reload)
	if string.find(PathFile, "..", 1, true) or string.sub(PathFile, 1, 1) == "/" then
		_RUNTIME_ERROR( string.format("forbidden import,Path=%s", PathFile) )
		return nil, "forbidden import"
	end

	local Old = _ImportModule[PathFile]
	if Old and not Reload then
		return Old
	end

	--****先loadfile再clear环境
	local func, err = loadfile(PathFile)
	if not func then
		return func, err
	end

	local function CallInit(Module)
		--载入模块时调用其构造函数
		if Module.__init__ then
			Module.__init__()
		end
	end

	local function CallDestroy(Module)
		if Module.__destroy__ then
			Module.__destroy__()
		end
	end

	local function CallUpdate(Module)
		if Module.__update__ then
			Module.__update__()
		end
	end


	--第一次载入，不存在更新的问题
	if not Old then
		_ImportModule[PathFile] = {}
		local New = _ImportModule[PathFile]
		--设置原始环境
		setmetatable(New, {__index = _G})
		setfenv(func, New)()
		CallInit(New)
		--放到列表中
		table.insert(ModuleArray, New)

		local ClassList = GetAllClass(New)
		--一定要放在函数的最后出口
		--虽然是新的模块，但是在这里把模块或者类的继承依赖传播下去，可以处理一个class写在几个文件里的情况，比如：
		--Import (Parent1)
		--Import (Child)
		--Import (Parent2)
		PropagateInherit (ClassList, ClassList)

		New.__FILE__ = PathFile
		return New
	end

	--处理在线更新内容
	CALLOUT.RemoveAll(Old)
	CallDestroy(Old)

	--先缓存原来的旧内容
	local OldCache = {}
	for k,v in pairs(Old) do
		OldCache[k] = v
		Old[k] = nil
	end

	--使用原来的module作为fenv，可以保证之前的引用可以更新到
	setfenv(func, Old)()

	--更新以后的模块, 里面的table的reference将不再有效，需要还原.
	local New = Old

	local NewClassList = {}
	local OldClassList = {}
	--还原table(copy by value)
	--print(PrePathFile, Reload)
	for k,v in pairs(OldCache) do
		local TmpNewData = New[k]
		--默认不更新
		New[k] = v
		if TmpNewData then
			if type(v) == "table" then --原来的类型是table
				if type(TmpNewData) == "table" then --更新之后的类型依然是table
					--如果是一个class则需要全部更新,其他则可能只是一些数据，不需要更新
					if v.__ClassType then
						local mt = getmetatable(v)
						--如果是一个对象数据则不需要更新
						if not (mt and  mt.__ObjectType) then
							OldClassList[k] = table.copy(v)
							ReplaceTbl(v, TmpNewData)
							NewClassList[k] = v
						end
					end
					local mt = getmetatable(TmpNewData)
					if mt then setmetatable(v, mt) end
				end
			--函数段必须用新的
			elseif type(v) == "function" then
				New[k] = TmpNewData
			end
		end
	end

	CallInit(New)
	CallUpdate(New)

	--一定要放在函数的最后出口
	PropagateInherit (NewClassList, OldClassList)
	return New
end

function Import(PathFile)
	local Module, Err = SafeImport(PathFile, false)
	assert(Module, Err)


	return Module
end

--并不是所有模块都能够Update，比如一些包含local动态数据的模块
--如果更新这些模块，则会导致数据丢失。
--以usertbl.lua为例，一旦更新则UserTbl表里面的数据就会为空。
function Update(PathFile)
	local Ret, err
	if table.member_key(DOFILELIST, PathFile) then
		dofile(PathFile)
		Ret = true
		err = "(dofile done)"
	else
		Ret, err = SafeImport(PathFile, true)
		assert(Ret, err)
		if UpdatePto then UpdatePto();err = err or "(done, with UpdatePto)" end
	end
	return Ret, err
end

--系统启动时执行此函数，调用_ImportModule中所有预载入模块的Startup函数
function ExeModuleStartup()
	--注意:**先复制一份Module表，因为调用SystemStartup的时候有可能改变这个table。
	local _copy = {}
	for k,v in pairs(ModuleArray) do
		_copy[k] = v
	end

	for _, ModuleObj in ipairs(_copy) do
		if ModuleObj.SystemStartup then
			ModuleObj:SystemStartup()
		end
	end
end
