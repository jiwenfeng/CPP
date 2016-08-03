MESSAGE(STATUS "Using bundled FindPTHREAD.cmake...")
FIND_PATH(
    PTHREAD_INCLUDE_DIR
    pthread.h
    /usr/include/ 
    /usr/local/include/ 
    )

FIND_LIBRARY(
    PTHREAD_LIBRARY 
    libpthread.so
    PATHS /usr/lib/ /usr/local/lib/ /usr/lib64/ /usr/local/lib64/
    )

MESSAGE("${PTHREAD_LIBRARY}")
