CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(pyvle CXX)

INCLUDE(FindPythonLibs)

EXECUTE_PROCESS(
  COMMAND
  python -c "from distutils import sysconfig; print sysconfig.get_python_version()"
  OUTPUT_VARIABLE PYTHON_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

##
# Check libraries with pkgconfig
##

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(VLE vle-0.9)

FIND_PACKAGE(Boost 1.35 COMPONENTS python)

##
# Browse the src subdirectory
##

ADD_SUBDIRECTORY(src)

##
# Status
##

MESSAGE(STATUS "- - - -")
MESSAGE(STATUS "pyvle configured successfully")
MESSAGE(STATUS "Using ${CMAKE_INSTALL_PREFIX} for installation")
MESSAGE(STATUS "Build type ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Python version ${PYTHON_VERSION}")
MESSAGE(STATUS "- - - -")

