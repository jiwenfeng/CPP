MESSAGE(STATUS "Using bundled FindLUA.cmake...")

FIND_PATH(
    LUA_INCLUDE_DIR
    lua.h
    /usr/include/ 
    /usr/local/include/ 
    /usr/local/lua/include/
    )

FIND_LIBRARY(
    LUA_LIBRARY 
    liblua.a
    PATHS /usr/lib/ /usr/local/lib/ /usr/local/lua/lib/
    )

