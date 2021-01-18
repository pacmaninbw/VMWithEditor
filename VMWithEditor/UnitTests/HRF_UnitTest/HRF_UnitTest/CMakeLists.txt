set(EXECUTABLE_NAME "HRF_Unit_Test")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
    set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
endif()

set(VM_SRC_DIR "../../..")
set(COMMON_TEST_DIR "../../Common_UnitTest_Code")

add_executable(${EXECUTABLE_NAME} hrf_unit_test_main.c unit_test_human_readable_program_format.c ${VM_SRC_DIR}/error_reporting.c ${VM_SRC_DIR}/default_program.c ${VM_SRC_DIR}/safe_string_functions.c ${COMMON_TEST_DIR}/unit_test_logging.c ${VM_SRC_DIR}/human_readable_program_format.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC UNIT_TESTING)
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC HRF_UNIT_TESTING_ONLY)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${VM_SRC_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${COMMON_TEST_DIR}")

add_test(NAME "HRF-unit-test"
         COMMAND "${EXECUTABLE_NAME}")
