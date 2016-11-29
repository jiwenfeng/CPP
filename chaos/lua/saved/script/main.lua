--saved主函数
dofile("base/extend.lua")
dofile("base/class.lua")
dofile("base/import.lua")
dofile("base/clean.lua")

_G.PORT_DEBUG = 21009
_G.PORT_CONNECT_DB = 21010
_G.PORT_CONNECT_SERVER = 21002

SOCKET = Import("base/socket.lua")
CALLOUT = Import("base/call_out.lua")
SAVE = Import("base/save.lua")
DEBUG = Import("base/debug.lua")
STATS = Import("base/stats.lua")


print("saved start ok")

