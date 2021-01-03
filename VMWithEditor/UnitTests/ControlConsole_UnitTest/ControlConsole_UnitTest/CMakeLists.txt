cmake_minimum_required(VERSION 3.18.4)

set(EXECUTABLE_NAME "Control_Console_Unit_Test.exe")

project(${EXECUTABLE_NAME} LANGUAGES C VERSION 1.0)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
    set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
endif()

set(VM_SRC_DIR "../../..")
set(COMMON_TEST_DIR "../../Common_UnitTest_Code")

add_executable(${EXECUTABLE_NAME} control_console_unit_test_main.c ${VM_SRC_DIR}/error_reporting.c ${VM_SRC_DIR}/safe_string_functions.c ${COMMON_TEST_DIR}/unit_test_logging.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC UNIT_TESTING)
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC CONTROL_CONSOLE_UNIT_TESTING)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${VM_SRC_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${COMMON_TEST_DIR}")


