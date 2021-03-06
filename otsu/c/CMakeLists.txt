# This project is designed to be built outside the Insight source tree.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(OTSU)

# Define flags for specific OS building
IF(UNIX)
  ADD_DEFINITIONS(-DUNIX)
  IF(APPLE)
    ADD_DEFINITIONS(-DAPPLE)
    MESSAGE("Building on APPLE")
  ELSE(APPLE)
    SET(LINUX 1)
    ADD_DEFINITIONS(-DLINUX)
    MESSAGE("Building on LINUX")
  ENDIF(APPLE)
ENDIF(UNIX)

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
  MESSAGE("Building on WIN32")
ENDIF(WIN32)

IF(UNIX)
  # If you don't want the full compiler output, remove the following line
  SET(CMAKE_VERBOSE_MAKEFILE ON)
  #add definitions, compiler switches, etc.
  ADD_DEFINITIONS(-Wall -Wno-deprecated)
  #SET(CMAKE_CXX_FLAGS_DEBUG "-g")
  SET(CMAKE_SKIP_RPATH ON)

  IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
  ENDIF(CMAKE_COMPILER_IS_GNUCC)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(UNIX)

#######################################
# Project Specific Setup
#######################################

SET(Example1_NAME OTSU)
SET(Example1_SRCS
  main.cpp
  utils.h
  utils.cpp
)

add_executable(${Example1_NAME} ${Example1_SRCS})
target_link_libraries(${Example1_NAME})

