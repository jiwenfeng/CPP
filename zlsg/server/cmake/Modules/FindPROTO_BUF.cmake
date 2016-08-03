MESSAGE(STATUS "Using bundled FindPROTO_BUF.cmake...")
FIND_PATH(
    PROTO_BUF_INCLUDE_DIR
    NAMES google/protobuf/message.h
    PATHS /usr/include/  /usr/local/include/ /usr/local/protobuf/include/
    )

FIND_LIBRARY(
    LIB_PROTO_BUF
    NAMES libprotobuf.a 
    PATHS /usr/lib/ /usr/local/lib/ /usr/local/protobuf/lib/
    )

FIND_LIBRARY(
    LIB_TCMALLOC_MINIMAL
    NAMES libtcmalloc_minimal.a
    PATHS /usr/lib/ /usr/local/lib/ /usr/local/protobuf/lib/
    )

if (LIB_PROTO_BUF)
    set(PROTO_BUF_LIBRARIES ${LIB_PROTO_BUF})
endif (LIB_PROTO_BUF)

if (LIB_TCMALLOC_MINIMAL)  
    set(PROTO_BUF_LIBRARIES ${LIB_TCMALLOC_MINIMAL} ${PROTO_BUF_LIBRARIES})
endif (LIB_TCMALLOC_MINIMAL)
