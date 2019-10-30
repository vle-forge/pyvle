

INCLUDE_DIRECTORIES(${VLE_INCLUDE_DIRS} ${NUMPY_INCLUDES})
LINK_DIRECTORIES(${VLE_LIBRARY_DIRS})

pybind11_add_module(pyvle pyvle_binding.cpp)
target_link_libraries(pyvle PUBLIC ${VLE_LIBRARIES})


#add_library(pyvle MODULE pyvle_binding.cpp)

#INCLUDE_DIRECTORIES( ${VLE_INCLUDE_DIRS})

#target_include_directories(pyvle PUBLIC ${VLE_INCLUDE_DIRS})
#target_link_libraries(pyvle PRIVATE pybind11::module ${VLE_LIBRARIES})
#set_target_properties(pyvle PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
#                                          SUFFIX "${PYTHON_MODULE_EXTENSION}")

#INCLUDE_DIRECTORIES(
#  ${VLE_INCLUDE_DIRS}
#  ${PYTHON_INCLUDE_PATH}
#  ${pyvle_SOURCE_DIR}/src
#  /pub/softs/pybind11/include/)

#LINK_DIRECTORIES(
#  ${VLE_LIBRARY_DIRS})

#ADD_LIBRARY(pyvle SHARED pyvle_binding.cpp)

#TARGET_LINK_LIBRARIES(pyvle ${VLE_LIBRARIES} ${PYTHON_LIBRARIES})

INSTALL(TARGETS pyvle DESTINATION lib/python${PYTHON_VERSION}/lib-dynload)
#INSTALL(FILES pyvle.py DESTINATION lib/python${PYTHON_VERSION})
