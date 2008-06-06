PROJECT(pyvle CXX)

cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

INCLUDE(FindPythonLibs)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(VLE vle)

ADD_SUBDIRECTORY(src)
