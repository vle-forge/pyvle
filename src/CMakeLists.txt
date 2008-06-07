INCLUDE_DIRECTORIES(
	${VLE_INCLUDE_DIRS}
	${PYTHON_INCLUDE_PATH}
	${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
	${VLE_LIBRARY_DIRS}
	${Boost_LIBRARY_DIRS})

add_library(pyvle SHARED convert.cpp convert.hpp pyvle.cpp pyvle.hpp wrapper.cpp)

TARGET_LINK_LIBRARIES(pyvle ${VLE_LIBRARIES} 
			    ${PYTHON_LIBRARIES} 
			    ${Boost_LIBRARIES})

INSTALL(TARGETS pyvle DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python/lib-dynload")
