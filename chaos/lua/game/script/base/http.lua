--from luasocket
-----------------------------------------------------------------------------
-- Parses a url and returns a table with all its parts according to RFC 2396
-- The following grammar describes the names given to the URL parts
-- <url> ::= <scheme>://<authority>/<path>;<params>?<query>#<fragment>
-- <authority> ::= <userinfo>@<host>:<port>
-- <userinfo> ::= <userid>[:<password>]
-- <path> :: = {<segment>/}<segment>
-- Input
--   url: uniform resource locator of request
--   default: table with default values for each field
-- Returns
--   table with the following fields, where RFC naming conventions have
--   been preserved:
--     scheme, authority, userinfo, userid, password, host, port,
--     path, params, query, fragment
-- Obs:
--   the leading '/' in {/<path>} is considered part of <path>
-----------------------------------------------------------------------------

function UrlEncode(s)
	return string.gsub(s, "([^A-Za-z0-9_])", function(c)
		return string.format("%%%02x", string.byte(c))
	end)
end


function UrlDecode(s)
	return (string.gsub(s, "%%(%x%x)", function(c)
		return string.char(tonumber(c, 16))
	end))
end


function _UrlEncode(URL)
	local NoEncode = "0123456789ABCDEF"
	local NewUrl = ""
	for i=1, string.len(URL) do
		local Char = string.byte(URL, i)
		if string.isalpha(Char) then
			NewUrl = NewUrl .. string.sub(URL, i, i)
		else
			if Char == 32 then
				NewUrl = NewUrl .. "+"
			else
				local High = math.floor(Char/16)+1
				local Low = math.floor(Char%16)+1
				local S1 = string.sub(NoEncode, High, High)
				local S2 = string.sub(NoEncode, Low, Low)
				NewUrl = string.format("%s%%%s%s", NewUrl, S1, S2)
			end
		end
	end
	return NewUrl
end


function Parse(url, default)
    -- initialize default parameters
    local parsed = {}
    for i,v in pairs(default or parsed) do parsed[i] = v end
    -- empty url is parsed to nil
    if not url or url == "" then return nil, "invalid url" end
    -- remove whitespace
    -- url = string.gsub(url, "%s", "")
    -- get fragment
    url = string.gsub(url, "#(.*)$", function(f)
        parsed.fragment = f
        return ""
    end)
    -- get scheme
    url = string.gsub(url, "^([%w][%w%+%-%.]*)%:",
        function(s) parsed.scheme = s; return "" end)
    -- get authority
    url = string.gsub(url, "^//([^/]*)", function(n)
        parsed.authority = n
        return ""
    end)
    -- get query stringing
    url = string.gsub(url, "%?(.*)", function(q)
        parsed.query = q
        return ""
    end)
    -- get params
    url = string.gsub(url, "%;(.*)", function(p)
        parsed.params = p
        return ""
    end)
    -- path is whatever was left
    if url ~= "" then parsed.path = url end
    local authority = parsed.authority
    if not authority then return parsed end
    authority = string.gsub(authority,"^([^@]*)@",
        function(u) parsed.userinfo = u; return "" end)
    authority = string.gsub(authority, ":([^:]*)$",
        function(p) parsed.port = p; return "" end)
    if authority ~= "" then parsed.host = authority end
    local userinfo = parsed.userinfo
    if not userinfo then return parsed end
    userinfo = string.gsub(userinfo, ":([^:]*)$",
        function(p) parsed.password = p; return "" end)
    parsed.userid = userinfo
    return parsed
end

local function is_ip(host)
	return string.match(host, "^%d+%.%d+%.%d+%.%d+$")
end

function Request(url, callback, head, usepost)
	local parsed = Parse(url)
	local host = parsed.host
	--print("host", host)
	local port = parsed.port or 80
	local req = parsed.path
	if (parsed.query) then
		req = req .. "?" ..parsed.query
	end
	head = head or {}
	if parsed.userinfo then
	--	head = head or {}
		head.Authorization = "Basic" .. " " .. engine.base64encode(parsed.userinfo)
	end
	local function _req(ip)
		if ip then
			--默认用get方法
			if usepost then
				netlib.http_post(ip, port, req, callback, head)
			else
				netlib.http_get(ip, port, req, callback, head)
			end
		else
			callback()
		end
	end

	head.Host = string.format("%s:%d", host,port)
	if is_ip(host) then
		_req(host)
	else
		netlib.dns_resolve(host, _req)
	end
	--兼容原来的接口，所以需要返回值
	return true
end

--coroutine模式的Request
function CoRequest(Url, Head)
	local Co = coroutine.running()
	assert(Co)
	local function  _CallBack(Code, Content)
		if coroutine.status(Co) ~= "dead" then
			return coroutine.resume(Co, Content)
		end
	end
	Request(Url, _CallBack, Head)
	return coroutine.yield()
end

function EncodeTable( Dict )
	assert(type(Dict) == "table")
	local res = {}
	for k, v in pairs(Dict) do
		table.insert(res, UrlEncode(tostring(k)).."="..UrlEncode(tostring(v)) )
	end

	local cres = ""

	for i=1,table.getn(res)-1 do
		cres = cres..res[i].."&"
	end
	cres = cres..res[table.getn(res)]
	return cres
end


function DecodeTable( Dict )
	assert(type(Dict) == "string")
	local res =	{}
	Dict = Dict .. '&'
	for m in string.gmatch(Dict, "[^&]+") do
		local k, v = string.match(m, "(.*)=(.*)")
		res[UrlDecode(k)] = UrlDecode(v)
	end
	return res
end

--[[ test http
function response(code, msg)
	print("HTTP BACK", code, msg)
end
HTTP.Request("http://www.google.com.hk/index.html", response)

local function proce()

	local content = HTTP.CoRequest(URL)
	
end

local co = coroutine.create(proce)
coroutine.resume(co)

]]--

