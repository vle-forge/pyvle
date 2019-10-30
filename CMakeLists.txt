CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(pyvle CXX)
ENABLE_TESTING()

SET(PYVLE_MAJOR 2)
SET(PYVLE_MINOR 0)
SET(PYVLE_PATCH 2)
SET(PYVLE_EXTRA "")

IF ("${PYVLE_EXTRA}" STREQUAL "")
  SET(PYVLE_NAME "${PROJECT_NAME}-${PYVLE_MAJOR}.${PYVLE_MINOR}.${PYVLE_PATCH}")
ELSE ("${PYVLE_EXTRA}" STREQUAL "")
  SET(PYVLE_NAME
    "${PROJECT_NAME}-${PYVLE_MAJOR}.${PYVLE_MINOR}.${PYVLE_PATCH}-${PYVLE_EXTRA}")
ENDIF ("${PYVLE_EXTRA}" STREQUAL "")


set(Python_ADDITIONAL_VERSIONS  "3.6")
find_package(PythonLibs)

#### set the following
# PYTHONLIBS_FOUND           - have the Python libs been found
# PYTHON_LIBRARIES           - path to the python library
# PYTHON_INCLUDE_PATH        - path to where Python.h is found (deprecated)
# PYTHON_INCLUDE_DIRS        - path to where Python.h is found
# PYTHON_DEBUG_LIBRARIES     - path to the debug library (deprecated)
# PYTHONLIBS_VERSION_STRING  - version of the Python libs found (since CMake 2.8.8)

find_package(PythonInterp)

#### set the following
# PYTHON_EXECUTABLE           - have the Python libs been found

#message(${PYTHON_LIBRARIES})
#message(${PYTHON_INCLUDE_PATH})
#message(${PYTHON_INCLUDE_DIRS})
#message(${PYTHONLIBS_VERSION_STRING})
#message(${PYTHON_EXECUTABLE})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
find_package(Numpy)

### NOT working with cmake 3.10 ?
#find_package (Python3 COMPONENTS Interpreter Development Numpy)

##
# Configure c++11
##
include(CheckIncludeFileCXX)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CMakeDetermineCCompiler)
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)

if (COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX1Y)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14/1y support.
                 Please use a different C++ compiler.")
endif()

##
# Check libraries with pkgconfig
##

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(VLE vle-2.1)

#set pybind11_DIR to cmake module path id necessary
find_package(pybind11 REQUIRED)

##
# Browse the src subdirectory
##

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

##
# Status
##

MESSAGE(STATUS "- - - -")
MESSAGE(STATUS "pyvle configured successfully")
MESSAGE(STATUS "Using ${CMAKE_INSTALL_PREFIX} for installation")
MESSAGE(STATUS "Build type ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Python version ${PYTHON_VERSION}")
MESSAGE(STATUS "- - - -")

INCLUDE(CMakeCPack.cmake)
