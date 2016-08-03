MESSAGE(STATUS "Using bundled FindSYS.cmake...")
FIND_PATH(
    SYS_INCLUDE_DIR
    NAMES pthread.h
    PATHS /usr/include/ /usr/local/include/ 
    )

FIND_LIBRARY(
    SYS_LIBRARY 
    NAMES libpthread.so libpthread.a libm.a libm.so librt.a librt.so libdl.a libdl.so libz.a libz.so
    PATHS /usr/lib/ /usr/local/lib/ /usr/lib64/ /usr/local/lib64/
    )

#MESSAGE(${SYS_LIBRARY})
