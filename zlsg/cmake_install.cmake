# Install script for directory: /home/jwf/code/CPP/zlsg

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/base/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/center/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/chores/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/script/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/config/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/navmesh/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/pb/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/player_role/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/frame/scene/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/agent_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/center_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/chores_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/data_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/game_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/gate_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/logger_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/role_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/sender_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/watchdog_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/service/node_service/cmake_install.cmake")
  INCLUDE("/home/jwf/code/CPP/zlsg/amtsvr/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/jwf/code/CPP/zlsg/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/jwf/code/CPP/zlsg/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
