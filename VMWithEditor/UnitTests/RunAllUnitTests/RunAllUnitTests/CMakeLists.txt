cmake_minimum_required(VERSION 3.18.4)

set(EXECUTABLE_NAME "Run_All_Unit_Tests.exe")

project(${EXECUTABLE_NAME} LANGUAGES C VERSION 1.0)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
    set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
endif()

set(VM_SRC_DIR "../../..")
set(COMMON_TEST_DIR "../../Common_UnitTest_Code")
set(LEXICAL_TEST_DIR "../../State_Machine_Unit_Test/State_Machine_Unit_Test")
set(PARSER_TEST_DIR "../../Parser_Unit_Test/Parser_Unit_Test")
set(CMD_LINE_TEST_DIR "../../CommandLine_UnitTest/CommandLine_UnitTest")
set(HRF_TEST_DIR "../../HRF_UnitTest/HRF_UnitTest")


add_executable(${EXECUTABLE_NAME}
run_all_unit_tests_main.c
${HRF_TEST_DIR}/hrf_unit_test_main.c
${HRF_TEST_DIR}/unit_test_human_readable_program_format.c
${LEXICAL_TEST_DIR}/lexical_analyzer_unit_test_main.c 
${LEXICAL_TEST_DIR}/internal_character_transition_unit_tests.c
${LEXICAL_TEST_DIR}/internal_sytax_state_tests.c
${LEXICAL_TEST_DIR}/lexical_analyzer_test_data.c
${LEXICAL_TEST_DIR}/lexical_analyzer_unit_test_utilities.c
${VM_SRC_DIR}/error_reporting.c  
${VM_SRC_DIR}/safe_string_functions.c
${VM_SRC_DIR}/arg_flags.c
${VM_SRC_DIR}/file_io_vm.c
${VM_SRC_DIR}/opcode.c
${VM_SRC_DIR}/parser.c 
${VM_SRC_DIR}/default_program.c
${VM_SRC_DIR}/human_readable_program_format.c
${VM_SRC_DIR}/lexical_analyzer.c 
${VM_SRC_DIR}/virtual_machine.c 
${PARSER_TEST_DIR}/parser_unit_test_main.c
${PARSER_TEST_DIR}/internal_parser_tests.c
${PARSER_TEST_DIR}/parser_unit_test.c
${CMD_LINE_TEST_DIR}/command_line_unit_test_main.c
${VM_SRC_DIR}/error_reporting.c
${VM_SRC_DIR}/arg_flags.c
${VM_SRC_DIR}/safe_string_functions.c
${COMMON_TEST_DIR}/unit_test_logging.c
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC UNIT_TESTING)
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC ALL_UNIT_TESTING)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${VM_SRC_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${COMMON_TEST_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${LEXICAL_TEST_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMD_LINE_TEST_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${PARSER_TEST_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${HRF_TEST_DIR}")


