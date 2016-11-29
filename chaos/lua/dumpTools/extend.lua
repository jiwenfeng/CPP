--��չ

--string����չ
--һ��Ѱ���ִ���Ч��ʵ�֣�ע�ⲻ֧��Pattern
function string.rfind(str, sub)
	local str1 = string.reverse(str)
	local sub1 = string.reverse(sub)
	local a,b = string.find(str1, sub1, 1, true)
	if a and b then
		return #str-b+1,#str-a+1
	end
end

--�ж��Ƿ���ĸ������
function string.isalpha(n)
	--number
	if n >= 48 and n <= 57 then
		return true
	end
	--a ~ z
	if n >= 97 and n <= 122 then
		return true
	end
	--A ~ Z
	if n >= 65 and n <= 90 then
		return true
	end
	return false
end

--��һ��str��del�ָ�Ϊ���ɸ�table�е�Ԫ��
--nΪ�ָ����
function string.split( line, sep, maxsplit ) 
	if string.len(line) == 0 then
		return {}
	end
	sep = sep or ' '
	maxsplit = maxsplit or 0
	local retval = {}
	local pos = 1   
	local step = 0
	while true do   
		local from, to = string.find(line, sep, pos, true)
		step = step + 1
		if (maxsplit ~= 0 and step > maxsplit) or from == nil then
			local item = string.sub(line, pos)
			table.insert( retval, item )
			break
		else
			local item = string.sub(line, pos, from-1)
			table.insert( retval, item )
			pos = to + 1
		end
	end     
	return retval  
end

function string.splitlines(text, maxsplit)
	return string.split(text,"\n",maxsplit)
end

--ɾ���հ�ǰ���հ��ַ�����ָ���ַ����е��ַ�
function string.lstrip(str, chars)
	if chars then
		for k=1,#str do
			local sub = string.sub(str,k,k)
			--
			if not string.find(chars, sub, 1, true) then
				return string.sub(str, k)
			end
		end
	else
		return string.gsub(str, "^%s*", "")
	end
end

--ɾ���հ׺󵼿հ��ַ�����ָ���ַ����е��ַ�
function string.rstrip(str, chars)
	if chars then
		for k=#str,1 do
			local sub = string.sub(str,k,k)
			--
			if not string.find(chars, sub, 1, true) then
				return string.sub(str, 1, k)
			end
		end
	else
		return string.gsub(str, "%s*$", "")
	end
end

--ɾ���հ�ǰ��հ��ַ�����ָ���ַ����е��ַ�
function string.strip(str, chars)
	return string.rstrip(string.lstrip(str, chars), chars)
end

--���ַ����е��ظ����ݼ��г�һ��
--squeeze("1222322","2") -> "1232"
function string.squeeze(str, rep)
	if not str then return nil end
        local pos1, pos2 = string.find(str, rep, 1, true)
        if not pos1 or not pos2 then
                return str
        end

        local pos = pos2 + 1
        while(true) do
                local tmp1, tmp2 = string.find(str,rep,pos,true)
		--print(tmp1, tmp2)
                if pos ~= tmp1 then
                        local remain = string.sub(str, pos)
			local head = string.sub(str, 1, pos2)
			--print(pos2, head, remain)
			if head and remain then
                        	str = head .. string.squeeze(remain, rep)
			end
                        break
                end
                pos = tmp2 + 1
        end
	return str
end

--�ж�һ���ַ����Ƿ���$ends��β
function string.endswith(str, ends)
	local i, j = string.rfind(str, ends)
	return (i and j == #str)
end

--�ж�һ���ַ����Ƿ���$begins��ʼ
function string.beginswith(str, begins)
	local i, j = string.find(str, begins, 1, true)
	return (i and i == 1)
end

function string.startswith( str, prefix )
	local from, to, matched = string.find( str, prefix, 1, true)
	return from == 1
end

function string.isnumber(str)
	return tostring(tonumber(str)) == str 
end


function __init__()
	--print(string.strip(" 	bfwo  ").."|end")
end

math = math or {}
sys = sys or {}
sys.path = sys.path or {}
string = string or {}
os = os or {}
os.path = os.path or {}
nt = nt or {}
posix = posix or {}

local _env = os.getenv("OS")
if _env then
	local _, _, value = string.find( _env, "([wW]in)" )
	if string.upper(value) == 'WIN' then
		os.name = 'nt'
	else
		os.name = 'posix'
	end
else 
	os.name = 'posix'
end

function math.round( num, idp )
	return tonumber( string.format("%."..idp.."f",num) )
end	

--��������
function math.roundedto(num)
	assert(type(num) == "number")
	if num < 0 and num%1 == 0.5 then
		return math.floor(num)
	else
		return math.floor(num + 0.5)
	end
end

function math.toint( num )
	return tonumber( string.format("%d", num) )
end

local function _normalize(value)
	local retval = ''
	if type(value) == 'function' then
		retval = '<' .. tostring(value) .. '>'
	elseif type(value) == 'table' then
		retval = '<' .. tostring(value) .. '>'
	elseif type(value) == 'string' then
		retval = string.format('%q',value)
	else
		retval = tostring(value)
	end
	return retval
end

function sys.repr(value)
	local retval = ''
	if type(value) == 'table' then
		local visited = {}
		retval = retval .. '{'
		for i, v in ipairs(value) do
			retval = retval .. _normalize(v) .. ','
			visited[i] = 1
		end
		for k, v in pairs(value) do
			if not visited[k] then
				retval = retval .. '[' .. _normalize(k) .. '] = ' .. _normalize(v) .. ', '
			end
		end
		retval = retval .. '}'
		return retval 
	else
		retval = _normalize(value)
	end
	return retval
end

function sys.FindErrorType (value, dummy)
	if not value then return end
	local Types = {number=1,string=1,table=1,boolean=1,}
	--print (value, dummy)
	if not Types[type (value)] then
		print (value, dummy)
		error()
	else
		if type(value) == "table" then
			print ("table", sys.repr(value), dummy)
			for k, v in pairs (value) do
				sys.FindErrorType(k, v)
				sys.FindErrorType(v, k)
			end
		end
	end

end

local function dodump (value, c)
	local retval = ''
	if type(value) == 'table' then
		c = (c or 0) + 1
		if c >= 100 then error("sys.dump too deep:"..retval) end

		retval = retval .. '{'
		for k, v in pairs(value) do
			retval = retval .. '[' .. dodump(k, c) .. '] = ' ..dodump(v, c) .. ', '
		end
		retval = retval .. '}'
		return retval 
	else
		retval = _normalize(value)
	end
	return retval
end
--Ϊ�˷�ֹ��ѭ������������������100����㡣����ʹ�á�
function sys.dump (value)
	local ni, ret = pcall (dodump, value)
	return ret
end

local REPR_INDENT = '  '
function bdump(value, forprint, deep)
	local linebr
	if forprint then
		linebr = '\n'
	else
		linebr = '#r'
	end
	local ret = ''
	if type(value) == 'table' then
		deep = deep or 0
		if deep >= 100 then error("sys.dump too deep:"..ret) end
		local indent = string.rep(REPR_INDENT, deep) 
		ret = ret .. '{' .. linebr
		for k, v in pairs(value) do
			local krepr
			if type(k)=='string' and string.match(k, '[_%a][_%a%d]*')==k then
				krepr = k
			else
				krepr = '[' .. bdump(k, forprint, deep + 1) .. ']'
			end
			ret = ret .. indent .. REPR_INDENT .. krepr .. ' = ' .. bdump(v, forprint, deep + 1) .. ',' .. linebr
		end
		ret = ret .. indent .. '}'
		return ret 
	else
		return _normalize(value)
	end
end

-- �������sys.dump
function sys.bdump(obj, forprint)
	local ni, ret = pcall(bdump, obj, forprint)
	return ret
end

function os.path.exists( path )
	local f = io.open(path, "r")
	return f ~= nil
end

local _seps = 
{
	nt = '\\',
	posix = '/',
}

os.path.sep = _seps[os.name]

local function _join( a, b )
	--[[
	>>> print os.getcwd()
	z:\trunk\logic\base
	>>> d = os.path.join('.','z:/trunk')
	>>> print d
	z:/trunk
	>>> d = os.path.join('.','\\')
	>>> print os.getcwd()
	z:\trunk\logic\base
	>>> d = os.path.join('.','/')
	>>> print d
	/
	freebsd
	>>> d = os.path.join('.','\\')
	>>> print d
	./\
	>>> d = os.path.join('.','/') 
	>>> print d
	/
	--]]
	if string.match(a,'^[ ]*$') then
		return b
	end
	if string.sub( b, 1, 1) == _seps.posix then
		return b
	else
		if os.name == 'nt' then
			if string.match( b, '^%a:' ) then
				return b
			end
		end

		if string.sub( a, -1 ) == os.path.sep then
			retval = a .. b
		else 
			retval = a .. os.path.sep .. b
		end
		return retval
	end
	error("don\'t know how to join", a, b)
end

function os.path.join( ... )
	local retval = ''
	for i,v in ipairs(arg) do
		retval = _join( retval, v)
	end
	return retval
end

function splitdrive(p)
	--[[Split a pathname into drive and path specifiers. Returns a 2-tuple
	"(drive,path)";  either part may be empty]]
	if string.sub(p,2,2) == ':' then
		return string.sub(p,1,2), string.sub(p,3,string.len(p))
	end
	return '', p
end

function nt.normpath( path )
	local path = string.gsub(path, '/', '\\' )
	local prefix, path = splitdrive(path)
	-- We need to be careful here. If the prefix is empty, and the path starts
	-- with a backslash, it could either be an absolute path on the current
	-- drive (\dir1\dir2\file) or a UNC filename (\\server\mount\dir1\file). It
	-- is therefore imperative NOT to collapse multiple backslashes blindly in
	-- that case.
	-- The code below preserves multiple backslashes when there is no drive
	-- letter. This means that the invalid filename \\\a\b is preserved
	-- unchanged, where a\\\b is normalised to a\b. It's not clear that there
	-- is any better behaviour for such edge cases.	
	if prefix == '' then
		-- No drive letter - preserve initial backslashes
		while string.sub(path,1,1) == "\\" do
			prefix = prefix .. "\\"
			path = string.sub(path,2,string.len(path))
		end
	else
		-- We have a drive letter - collapse initial backslashes
		if string.startswith(path,"\\") then
			prefix = prefix .. "\\"
			path = string.sub(path,2,string.len(path))
		end
	end

	local comps = string.split( path, "\\" )
	local i = 1 
	while i < #comps + 1 do
		if comps[i] == '.' or comps[i] == '' then
			table.remove(comps,i)
		elseif comps[i] == '..' then
			if i > 0 and comps[i-1] ~= ".." then
				table.remove(comps,i)
				table.remove(comps,i-1)
				i = i - 1
			elseif i == 0 and string.startswith("\\") then
				table.remove(comps,i)
			else
				i = i + 1
			end
		else
			i = i + 1
		end
	end

	-- If the path is now empty, substitute '.'
	if not prefix and #comps == 0 then
		table.insert(comps,'.')
	end
	return prefix .. table.concat(comps,"\\")
end

-- Normalize a path, e.g. A//B, A/./B and A/foo/../B all become A/B.
-- It should be understood that this may change the meaning of the path
-- if it contains symbolic links!

function os.getcwd()
	local retval = nil
	local cmd = nil

	if os.name == 'nt' then
		cmd = 'cd'
	else
		cmd = 'pwd'
	end
	local fin = io.popen(cmd,'r')
	retval = fin:read('*l')
	fin:close()	
	return retval
end

function os.path.abspath( path )
	if os.name == 'nt' then
		if string.match('^%a:',path) then
			return path
		end
	end

	local cwd = os.getcwd()
	if os.name == 'nt' then			
		if _seps[ string.sub( path, 1, 1 ) ] ~= nil then
			return os.path.join(string.sub(cwd,1,2),path)
		end
	elseif os.name == 'posix' then
		if string.sub( path, 1, 1 ) == os.path.sep then
			return path
		end
	end

	return os.path.join(cwd,path)

	--[[
	python:
	>>> d = os.getcwd()
	>>> print d
	z:\trunk\logic\base
	>>> c = os.path.abspath('trunk')
	>>> print(c)
	z:\trunk\logic\base\trunk
	>>> d = os.path.abspath('./z:trunk')
	>>> print d
	z:\trunk\logic\base\z:trunk
	>>> d = os.path.abspath('\\')
	>>> print d
	z:\
	>>> d = os.path.abspath('/')
	>>> print d
	z:\
	--]]

end

function math.bresenham(x0, y0, x1, y1)
	-- please refer following linnk for detail information :
	-- http://en.wikipedia.org/w/index.php?title=Bresenham%27s_line_algorithm&printable=yes 
	-- the following implementation is devired from this article. 	

	local retval = {}
	local deltax = math.abs(x1 - x0)
	local deltay = math.abs(y1 - y0)
	local steep = deltay > deltax
	if steep then
		x0,y0 = y0,x0
		x1,y1 = y1,x1
		deltax, deltay = deltay, deltax
	end
	local xstep, ystep
	if x1 - x0 > 0 then xstep = 1 else xstep = -1 end
	if y1 - y0 > 0 then ystep = 1 else ystep = -1 end
	local y = y0
	local err = 0
	for x = x0, x1, xstep do
		if steep then
			table.insert(retval, {y,x})
		else
			table.insert(retval, {x,y})
		end
		err = err + deltay
		if err + err >= deltax then
			y = y + ystep
			err = err - deltax
		end
	end
	return retval
end
--����һ���������������
--input {1, 2}
--output { {1, 2}, {2, 1} }
function math.permutation(Array)
	local PList = {}
	if table.getn(Array) < 1 then
		return nil
	end
	if table.getn(Array) == 1 then
		return {{Array[1]}}
	end
	local First = Array[1]
	table.remove(Array, 1)
	local Last = math.permutation(Array)
	for _, t in pairs(Last) do
		for i=1, table.getn(t) + 1 do
			local TmpList = {}
			--copy table
			for k, v in pairs(t) do
				TmpList[k] = v
			end
			table.insert(TmpList, i, First)
			table.insert(PList, TmpList)
		end
	end
	return PList
end

function table.subarray(Array, First, Last)
	local Ret = {}
	for i=First, Last do
		table.insert(Ret, Array[i])
	end
	return Ret
end

function table.update( a, b )
	assert( type(a) == type(b) and type(b) == "table")
	for k,v in pairs(b) do
		a[k] = v
	end	
end

--
-- same as UTIL.deepcopy
--
function table.deepcopy(src)
    if type(src) ~= "table" then
        return src
    end
    local cache = {}
    local function clone_table(t, level)
        if not level then
            level = 0
        end

        if level > 100 then
			return t
        end

        local k, v
        local rel = {}
        for k, v in pairs(t) do
            if type(v) == "table" then
                if cache[v] then
                    rel[k] = cache[v]
                else
                    rel[k] = clone_table(v, level+1)
                    cache[v] = rel[k]
                end
            else
                rel[k] = v
            end
        end
        setmetatable(rel, getmetatable(t))
        return rel
    end
    return clone_table(src)
end

--
-- ����t1�����ݺϲ���t��
--
function table.merge(t, t1)
    for k, v1 in pairs(t1) do
        local vt1 = type(v1)
        if not t[k] then
            t[k] = table.deepcopy(v1)
        else
            local v = t[k]
            local vt = type(v)
            if vt == vt1 then
                if vt == 'table' then
                    table.merge(t[k], t1[k])
                else
                    t[k] = t1[k]
                end
            end
        end
    end
end

--
-- �Լӵķ�ʽ����t1�ϲ�����t�С�
--
function table.addmerge(t, t1)
	for k, v1 in pairs(t1) do
		local vt1 = type(v1)
		if not t[k] then
			t[k] = UTIL.DeepCopy(v1, true)
		else
			local v = t[k]
			local vt = type(v)
			if vt == vt1 then
				if vt == 'table' then
					table.addmerge(t[k], t1[k])
				elseif vt == 'number' then
					t[k] = t[k] + t1[k]
				elseif vt == 'string' then
					t[k] = t[k] .. t1[k]
				end
			end
		end
	end
end

array = array or {}
function array.merge( a, b )
	assert( type(a) == type(b) and type(b) == "table")
	for k,v in ipairs(b) do
		table.insert(a, v)
	end
end

function array.random_pick(ar)
	if #ar == 0 then
		return nil
	end
	return ar[math.random(1, #ar)]
end

--�鿴Var�����ĸ�����
-- Array : { [1] = {Limit = 10}, [2] = {Limit = 20}, [3] = {}} , i.e: 0-10, 11-20, 20-
function table.in_range (Var, Range)
	for i, v in ipairs (Range) do
		if not v.Limit or Var <= v.Limit then
			return i, v
		end
	end
end

function table.member_key(Table, Value)
	for k,v in pairs(Table) do
		if v == Value then
			return k
		end
	end

	return nil
end

function table.has_key(Table, Key)
	for k,v in pairs(Table) do
		if k == Key then
			return true
		end
	end

	return false
end
--�������е�key����Ϊһ�����飬Ч�ʱȽϵͣ�������Ƶ������
function table.keys(Table)
	local Keys = {}
	for k,_ in pairs(Table) do
		table.insert(Keys, k)
	end

	return Keys
end

--�������е�value����Ϊһ������,Ч�ʱȽϵͣ�������Ƶ������
function table.values(Table)
	local Values = {}
	for _,v in pairs(Table) do
		table.insert(Values, v)
	end

	return Values
end

--����һ�������key
function table.random_key(Table)
	local Keys = table.keys(Table)
	local n = table.maxn(Keys)
	if n <= 0 then
		return nil
	end
	return Keys[math.random(1,n)]
end

--��table���������n��value
function table.random_values(Table, n)
	local n = n or 1
	local Values = table.values(Table)
	if n >= #Values then
		return Values
	end
	local Ret = {}
	for i=1, n do
		local R = math.random(1, #Values)
		table.insert(Ret, Values[R])
		table.remove(Values, R)
	end
	return Ret
end

--��Array(key)�����������
--���ı����Array�����ݣ�����Ľ��ͨ������ֵ����, ����������ǰ���key�Ķ�Ӧ��ϵ
function table.random_sort (Array)
	local n = #Array

	local k = {}
	for i = 1, n do
		k[i] = i
	end 

	local o = {}
	local s = {}
	for i = 1, n do
		local j = math.random (n - i + 1)
		s[k[j]] = i 
		table.insert(o, Array[k[j]])
		table.remove (k, j)
	end

	return o, s 
end

--��һ��mapping�����������k,v������µ�mapping
function table.random_kv(Table, n)
	local n = n or 1
	local Keys = table.keys(Table)
	if n > #Keys then
		return Table
	end
	local Ret = {}
	for i=1, n do
		local Rand = math.random(1, #Keys)
		local RandKey = Keys[Rand]
		--Ret[RandKey] = Table[RandKey]
		table.insert( Ret, Table[RandKey])
		table.remove(Keys, Rand)
	end
	return Ret
end

function table.random_kv2(Table, n)
	local n = n or 1
	local Keys = table.keys(Table)
	if n > #Keys then
		return Table
	end
	local Ret = {}
	for i=1, n do
		local Rand = math.random(1, #Keys)
		local RandKey = Keys[Rand]
		Ret[RandKey] = Table[RandKey]
		table.remove(Keys, Rand)
	end
	return Ret
end


--��table���������1��value
function table.random_value(Table)
	local Values = table.values(Table)
	local n = table.maxn(Values)
	if n <= 0 then
		return nil
	end
	return Values[math.random(1,n)]
end



function table.filter(Tbl, Item)
	for i=1,#Tbl do
		if Tbl[i] == Item then
			table.remove(Tbl, i)
			i = i-1
		end
	end
end

function table.remove_array_value(Tbl,Value)
	for idx, v in ipairs (Tbl) do
		if v == Value then
			table.remove(Tbl, idx)
			return true
		end
	end

	return false
end

function table.remove_array_value2(tbl, value)
	local find_idx = nil
	for idx, v in pairs(tbl) do
		if v == value then
			find_idx = idx
			break
		end    
	end            

	if find_idx then
		local last_index = table.maxn(tbl)
		tbl[find_idx] = tbl[last_index]
		table.remove(tbl)
	end                    
end   

function table.remove_by_value(Tbl, Value)
	for k,v in pairs(Tbl) do
		if v == Value then
			Tbl[k] = nil
			return true
		end
	end
	return false
end

function table.add(To, From)
	for k,v in pairs(From) do --������ipairs��������ڷ�������table������
		table.insert(To, v)
	end
	return To
end

function table.increase(Tbl, Key, Value)
	Value = Value or 1
	Tbl[Key] = (Tbl[Key] or 0) + Value 
	return Tbl[Key]
end

--����Array�е����ֵ
--ע��:����Hash-table
function table.max(Array)
	return math.max (unpack(Array))
end

--����Array�е���Сֵ
--ע��:����Hash-table
function table.min(Array)
	return math.min (unpack(Array))
end

--����table��size
function table.size(Table)
	if Table then
		local Ret = 0
		for _,_ in pairs(Table) do
			Ret = Ret + 1
		end
		return Ret
	else
		return 0
	end
end

--����number�������ƽ��ֵ(float)
function table.avg(Array)
	if #Array == 0 then
		return 0
	end
	local All = 0
	local i = 0
	for _, Data in ipairs(Array) do
		assert(type(Data) == "number")
		All = All + Data
		i = i + 1
	end
	return All/i
end

function table.empty(tbl)
	for k,v in pairs(tbl) do
		return false
	end
	return true
end

function table.copy(tbl)
	if not tbl then return nil end
	local ret = {}
	for k,v in pairs(tbl) do
		ret[k] = v
	end
	return ret
end

function table.equal_map(tbl1, tbl2)
	if tbl1 == tbl2 then
		return true
	end
	if not tbl1 or not tbl2 then
		return false
	end
	if table.size(tbl1) ~= table.size(tbl2) then
		return false
	end
	for k,v in pairs(tbl1) do
		if tbl2[k] ~= v then
			return false
		end
	end
	return true
end

--Like LPC's sort_by_value
function table.sort_by_value(tbl)
	--��˵�е�ѡ������
	local Keys = table.keys(tbl)
	local Size = #Keys
	for i=1, Size do
		for j=i, Size do
			if tbl[Keys[i]] > tbl[Keys[j]] then
				--����
				local Tmp = Keys[i]
				Keys[i] = Keys[j]
				Keys[j] = Tmp
			end
		end
	end
	return Keys
end

function table.revert_sort(tbl)
	local Keys = table.keys(tbl)
	local Size = #Keys
	for i=1, Size do
		for j=i, Size do
			if tbl[Keys[i]] < tbl[Keys[j]] then
			--����
			local Tmp = Keys[i]
			Keys[i] = Keys[j]
			Keys[j] = Tmp
			end
		end
	end
	return Keys
end

function math.limit( value, a, b)
	local min, max = a, b
	if b and a > b then
		min, max = b, a
	end
	if value < min then
		return min
	elseif max and value >  max then
		return max
	else
		return value
	end
end

function os.daydiff( from, to )
	-- 1970-1-1,0:0:0 => 2649600s
	-- 24 * 60 * 60 => 86400s
	return math.floor((to-2649600)/86400 - (from-2649600)/86400)
end

function os.zeroclock( time )
	-- ��ʱ���0��
	local time = time or os.time()
	local data = os.date('*t', time)
	data.hour = 0
	data.min  = 0
	data.sec  = 0
	return os.time( data )
end

--// CHILL CODE ? //--

--// table.binsert( table, value [, comp] )

-- LUA 5.x ADD-On for the table library
-- Inserts a given value through BinaryInsert into the table sorted by [,comp]
-- If comp is given, then it must be a function that receives two table elements,
-- and returns true when the first is less than the second or reverse
-- e.g.  comp = function( a, b ) return a > b end , will give a sorted table, with the biggest value on position 1
-- [, comp] behaves as in table.sort( table, value [, comp] )

-- This method is faster than a regular table.insert( table, value ) and a table.sort( table [, comp] )

function table.binsert( t, value, fcomp )

	-- Initialise Compare function
	fcomp = fcomp or function( a, b ) return a < b end

	--  Initialise Numbers
	local iStart, iEnd, iMid, iState =  1, table.getn( t ), 1, 0

	-- Get Insertposition
	while iStart <= iEnd do

		-- calculate middle
		iMid = math.floor( ( iStart + iEnd )/2 )

		-- compare
		if fcomp( value , t[iMid] ) then
			iEnd = iMid - 1
			iState = 0
		else
			iStart = iMid + 1
			iState = 1
		end
	end

	table.insert( t, ( iMid+iState ), value )
end

--// table.bfind( table, value [, compvalue] [, reverse] )

-- LUA 5.x ADD-On for the table library
-- Searches the table through BinarySearch for value, if the value is found it returns the index
--     and the value of the table where it was found
-- If compvalue is given then it must be a function that takes one value and returns a second value2
-- to be compared with the input value, e.g. compvalue = function( value ) return value[1] end
-- If reverse is given then the search assumes that the table is sorted with the biggest value on position 1

function table.bfind( t, value, fcompval, reverse )

	-- initialise Functions
	fcompval = fcompval or function( value ) return value end
	fcomp = function( a, b ) return a < b end
	if reverse then
		fcomp = function( a, b ) return a > b end
	end

	--  Initialise Numbers
	local iStart, iEnd, iMid =  1, table.getn( t ), 1

	-- Binary Search
	while (iStart <= iEnd) do

		-- calculate middle
		iMid = math.floor( ( iStart + iEnd )/2 )

		-- get compare value
		local value2 = fcompval( t[iMid] )

		if value == value2 then
			return iMid, t[iMid]
		end

		if fcomp( value , value2 ) then
			iEnd = iMid - 1
		else
			iStart = iMid + 1
		end
	end
end

--����һ������������table��{[1] = {Odds = 50}, [2] = {Odds = 50}}
--�������ȡ��key����
function table.get_key_by_odds(Tbl, FullOdds)
	if Tbl == nil then
		return nil
	end
	--����ĳЩ�������С�������
	local Ext=100	
	if not FullOdds then
		FullOdds = 0
		for k,v in pairs(Tbl) do
			FullOdds = FullOdds + v.Odds*Ext
		end
	else
		FullOdds = FullOdds*Ext
	end

	--ע�⣬nil���޲����ǲ�һ����
	local Ran = math.random(FullOdds)

	local TotalRan = 0
	for key, subTbl in pairs(Tbl) do
		TotalRan = TotalRan + subTbl.Odds*Ext
		if Ran <= TotalRan then
			return key
		end
	end
end

function table.get_key_by_odds_ex(Tbl, FullOdds, GetOdds)
	if Tbl == nil then
		return nil
	end
	--����ĳЩ�������С�������
	local Ext=100	
	if not FullOdds then
		FullOdds = 0
		for k,v in pairs(Tbl) do
			FullOdds = FullOdds + GetOdds(v)*Ext
		end
	else
		FullOdds = FullOdds*Ext
	end

	--ע�⣬nil���޲����ǲ�һ����
	local Ran = math.random(FullOdds)

	local TotalRan = 0
	for key, v in pairs(Tbl) do
		TotalRan = TotalRan + GetOdds(v)*Ext
		if Ran <= TotalRan then
			return key
		end
	end
end

--Returns a new tbl that is n copies of the tbl. 
function table.rep(Tbl, n)
	if n < 1 then
		return nil
	end
	local Tmp = {}
	for k=1, n do
		table.insert(Tmp, Tbl)
	end
end

--only for hashtable
function table.key_contain(Big, Small)
	assert(type(Big) == 'table' and type(Small) == 'table')
	for k, v in pairs(Small) do 
		if not Big[k] then 
			return false
		end
	end
	return true
end

--�жϰ�����ϵ(hash table only)
function table.same_table(Tbl1, Tbl2)
	assert(type(Tbl1) == 'table')
	assert(type(Tbl2) == 'table')
	if table.size(Tbl1) ~= table.size(Tbl2) then
		return false
	end

	for k, v in pairs(Tbl2) do
		if (not Tbl1[k]) or (Tbl1[k] ~= v) then
			return false
		end
	end
	return true
end

--�жϰ�����ϵ
function table.contain(Big, Small)
	for _, Each in pairs(Small) do
		if not table.member_key(Big, Each) then
			return false, Each
		end
	end
	
	return true
end

--��Array1��Array2�����Ӧλ�úϳ�mapping��һ��pair
function table.mapping(Array1, Array2)
	local Ret = {}
	for k,v in ipairs(Array1) do
		Ret[v] = Array2[k]
	end
	return Ret
end

--�����������keylist�ϳ�һ��deep map
--���紫�� 'a','b','c','d'���return һ��map {a={b={c='d'}}}
function table.keymap(map, ...)
	map = map or {}
	if arg['n'] <= 1 then
		return map
	end

	local tmp = map
	for k,v in ipairs(arg) do
		if k >= arg['n'] - 1 then
			tmp[v] = arg[k+1]
			break
		end
		
		tmp[v] = tmp[v] or {}
		tmp = tmp[v]
	end
	return map
end

function table.reverse(Array)
	local size = #Array
	for i=1, math.floor(size/2) do
		local tmp = Array[i]
		Array[i] = Array[size+1-i]
		Array[size+1-i] = tmp
	end
	return Array
end

function table.copy(src, rel)
	local rel = rel or {}
	if type(src) ~= "table" then
		return rel
	end
	for k, v in pairs(src) do
		rel[k] = v
	end
	return rel
end

function table.deepcopy(src, quiet)
	if type(src) ~= "table" then
		return src
	end
	local cache = {}
	local function clone_table(t, level)
		if not level then
			level = 0
		end

		if level > 20 then
			if not quiet then
				error("table clone failed, "..
						"source table is too deep!")
			else
				return t
			end
		end

		local k, v
		local rel = {}
		for k, v in pairs(t) do
			--if k == "Name" then print(k, tostring(v)) end
			if type(v) == "table" then
				if cache[v] then
					rel[k] = cache[v]
				else
					rel[k] = clone_table(v, level+1)
					cache[v] = rel[k]
				end
			else
				rel[k] = v
			end
		end
		setmetatable(rel, getmetatable(t))
		return rel
	end
	return clone_table(src)
end



function pack(...)
	return arg
end

function io.readfile(file)
	local fh = io.open(file)
	if not fh then return nil end
	local data = fh:read("*a")
	fh:close()
	return data
end
function filter_map(t, op, ...)
	local o = {}
	for k,v in pairs(t) do
		local r = op(k,v,...)
		if r then
			table.insert(o, r)
		end
	end
	return o
end

function filter_imap(t, op, ...)
	local o = {}
	for i,v in ipairs(t) do
		local r = op(i,v,...)
		if r then
			table.insert(o, r)
		end
	end
	return o
end

function map(t, op, ...)
	local o = {}
	for k,v in pairs(t) do
		table.insert(o, op(k,v,...))
	end
	return o
end

function imap(t, op, ...)
	local o = {}
	for i,v in ipairs(t) do
		table.insert(o, op(i,v,...))
	end
	return o
end

function IsString( Elem )
	return (type(Elem) == "string")
end

function IsFunc( Elem )
	return (type(Elem) == "function")
end

function IsTable( Elem )
	return (type(Elem) == "table")
end

--�ж�table�Ƿ�������
function IsArray( Elem )
	if not IsTable(Elem) then
		return false
	end

	local Map = {}
	local Size = 0
	for k, _ in pairs(Elem) do 
		if not IsPosInt(k) then
			return false
		end

		Map[k] = true
		Size = Size + 1
	end

	if Size <= 0 then
		return false
	end

	for idx = 1, Size do
		if not Map[idx] then
			return false
		end
	end

	return true
end

function IsNumber( Elem )
	return (type(Elem) == "number")
end

function IsBoolean( Elem )
	return (type(Elem) == "boolean")
end

function IsFunction( Elem)
	return (type(Elem) == "function")
end

function IsInt( Num )
	if (not Num) or (not IsNumber(Num)) then
		return false
	end

	return (math.floor(Num) == Num)
end

--touchһ���ļ����������û�����·�������Զ�����
function Touch(PathFile)
	if posix.stat(PathFile) then
		return
	end

	local Start = 1
	while 1 do
		local TmpStart, TmpEnd = string.find(PathFile, "%/", Start)
		if TmpStart and TmpEnd then
			local Path = string.sub(PathFile, 1, TmpEnd)
			if not posix.stat(Path) then
				posix.mkdir(Path)
			end
			Start = TmpEnd+1
		else
			break
		end
	end
	if not posix.stat(PathFile) then
		local fh = io.open(PathFile, "a+")
		fh:close()
	end
end

function FormatSerialize(Object)
	local function ConvSimpleType(v)
		if type(v)=="string" then
			return string.format("%q",v)
		end
		return tostring(v)
	end

	local function RealFun(Object, Depth)
		--TODO: gxzou ѭ������û�д���
		Depth = Depth or 0
		Depth = Depth + 1
		assert(Depth<20, "too long Depth to serialize")

		if type(Object) == 'table' then
			--if Object.__ClassType then return "<Object>" end
			local Ret = {}
			table.insert(Ret,'{\n')
			for k, v in pairs(Object) do
				--print ("serialize:", k, v)
				local _k = ConvSimpleType(k)
				if _k == nil then
					error("key type error: "..type(k))
				end
				table.insert(Ret,'[' .. _k .. ']')
				table.insert(Ret,'=')
				table.insert(Ret,RealFun(v, Depth))
				table.insert(Ret,',\n')
			end
			table.insert(Ret,'}\n')
			return table.concat(Ret)
		else
			return ConvSimpleType(Object)
		end
	end
	
	return RealFun(Object)
end

function _Serialize(Object)
	local function ConvSimpleType(v)
		if type(v)=="string" then
			return string.format("%q",v)
		end
		return tostring(v)
	end

	local function RealFun(Object, Depth)
		--TODO: gxzou ѭ������û�д���
		Depth = Depth or 0
		Depth = Depth + 1
		assert(Depth<20, "too long Depth to serialize")

		if type(Object) == 'table' then
			--if Object.__ClassType then return "<Object>" end
			local Ret = {}
			table.insert(Ret,'{')
			for k, v in pairs(Object) do
				--print ("serialize:", k, v)
				local _k = ConvSimpleType(k)
				if _k == nil then
					error("key type error: "..type(k))
				end
				table.insert(Ret,'[' .. _k .. ']')
				table.insert(Ret,'=')
				table.insert(Ret,RealFun(v, Depth))
				table.insert(Ret,',')
			end
			table.insert(Ret,'}')
			return table.concat(Ret)
		else
			return ConvSimpleType(Object)
		end
	end
	
	return RealFun(Object)
end

Serialize = _Serialize

--Data�����л�������(�ַ���)
function UnSerialize(Data)
	return assert(loadstring("return "..Data))()
end

--����һ��TableΪֻ��
--Sample: Wizard = ReadOnly{"gm001","gm002","gm003"}
function ReadOnly(t)
	local proxy = {}
	local mt = {       -- create metatable
		__index = t,
		__newindex = function (t,k,v)
			error("attempt to update a read-only table", 2)
		end
	}
	setmetatable(proxy, mt)
	return proxy
end


--�����binary�Ľṹ
BinPath = "../binary/"
function DumpFile(PathFile, ToPathFile)
	local s1 = posix.stat(PathFile)
	local s2 = posix.stat(ToPathFile)
	if s1 and s2 then
		if s2.mtime >= s1.mtime then
			return 
		end
	end
	--print("compiling ", PathFile)

	local fh = io.open(PathFile)
	
	local FileData = fh:read("*a")
	
	--������Ҫ
	--FileData = string.gsub(FileData, '([%(%s%,])__FILE__', 
	--	'%1"'..PathFile..'"')

	local func, err = loadstring(FileData, PathFile)
	assert(func, err)
	
	if not ToPathFile then
		ToPathFile = BinPath..PathFile
	end
	
	--��ʱ���ø���
	--ToPathFile = string.gsub(ToPathFile, "%.[%w_-]+$", ".bin")
	Touch(ToPathFile)
	local Handler = io.open(ToPathFile,"w+")
	if not Handler then
		print("no such file", ToPathFile)
		return
	end
	Handler:write(string.dump(func))	
	Handler:close()	
end
