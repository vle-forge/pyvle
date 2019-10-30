add_test(test_pyvle
  "${PROJECT_SOURCE_DIR}/tests/pyvletest.py")

set_property(TEST test_pyvle PROPERTY
    ENVIRONMENT "VLE_HOME_TEST=/tmp/vlehome"
                "VLE_TEST_DIR=${PROJECT_SOURCE_DIR}/tests"
)

