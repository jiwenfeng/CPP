MESSAGE(STATUS "Using bundled FindMYSQL.cmake...")
FIND_PATH(
    MYSQL_INCLUDE_DIR
    mysql.h
    /usr/include/ 
    /usr/local/include/ 
    /usr/local/mysql/include/
    )

FIND_LIBRARY(
    MYSQL_LIBRARY 
    libmysqlclient_r.a
    PATHS /usr/lib/ /usr/local/lib/ /usr/local/mysql/lib/
    )
