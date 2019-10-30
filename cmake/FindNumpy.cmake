
execute_process(
 COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; print(numpy.__path__[0].rstrip('/numpy'))"
 RESULT_VARIABLE NUMPY_CMD_RES
 OUTPUT_VARIABLE NUMPY_CMD_OUT
 
)

string(REGEX REPLACE "\n$" "" NUMPY_CMD_OUT "${NUMPY_CMD_OUT}")

set(NUMPY_INCLUDES "${NUMPY_CMD_OUT}/numpy/core/include/")
