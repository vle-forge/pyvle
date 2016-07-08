CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(pyvle CXX)
ENABLE_TESTING()

SET(PYVLE_MAJOR 1)
SET(PYVLE_MINOR 2)
SET(PYVLE_PATCH 0)
SET(PYVLE_EXTRA "")

IF ("${PYVLE_EXTRA}" STREQUAL "")
  SET(PYVLE_NAME "${PROJECT_NAME}-${PYVLE_MAJOR}.${PYVLE_MINOR}.${PYVLE_PATCH}")
ELSE ("${PYVLE_EXTRA}" STREQUAL "")
  SET(PYVLE_NAME
    "${PROJECT_NAME}-${PYVLE_MAJOR}.${PYVLE_MINOR}.${PYVLE_PATCH}-${PYVLE_EXTRA}")
ENDIF ("${PYVLE_EXTRA}" STREQUAL "")

INCLUDE(FindPythonLibs)

EXECUTE_PROCESS(
  COMMAND
  python -c "from distutils import sysconfig; print sysconfig.get_python_version()"
  OUTPUT_VARIABLE PYTHON_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

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
PKG_CHECK_MODULES(VLE vle-2.0)

FIND_PACKAGE(Boost 1.35 COMPONENTS python)

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
