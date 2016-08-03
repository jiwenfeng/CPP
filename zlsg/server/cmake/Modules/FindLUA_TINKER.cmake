MESSAGE(STATUS "Using bundled FindLUA_TINKER.cmake...")
FIND_PATH(
    LUA_TINKER_INCLUDE_DIR
    lua_tinker.h
    /usr/include/ 
    /usr/local/include/ 
    /usr/local/lua/include/
    ./tpl/LuaTinker
    #/home/rockpile/server/tpl/LuaTinker
    )

FIND_LIBRARY(
    LUA_TINKER_LIBRARY 
    NAMES libluatinker.a
    PATHS /usr/lib/ /usr/local/lib/ /usr/local/lua/lib/ ./tpl/LuaTinker
    )

#MESSAGE(${LUA_TINKER_INCLUDE_DIR})
