set(EXECUTABLE_NAME "Parser_Unit_Test")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
    set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
endif()

set(VM_SRC_DIR "../../..")
set(LEXICAL_TEST_DIR "../../State_Machine_Unit_Test/State_Machine_Unit_Test")
set(COMMON_TEST_DIR "../../Common_UnitTest_Code")

add_executable(${EXECUTABLE_NAME} internal_parser_tests.c  parser_unit_test.c  parser_unit_test_main.c ${VM_SRC_DIR}/error_reporting.c ${VM_SRC_DIR}/human_readable_program_format.c ${VM_SRC_DIR}/lexical_analyzer.c ${VM_SRC_DIR}/opcode.c ${VM_SRC_DIR}/parser.c ${VM_SRC_DIR}/safe_string_functions.c  ${VM_SRC_DIR}/virtual_machine.c ${COMMON_TEST_DIR}/unit_test_logging.c ${LEXICAL_TEST_DIR}/internal_character_transition_unit_tests.c ${LEXICAL_TEST_DIR}/internal_sytax_state_tests.c ${LEXICAL_TEST_DIR}/lexical_analyzer_test_data.c ${LEXICAL_TEST_DIR}/lexical_analyzer_unit_test_main.c ${LEXICAL_TEST_DIR}/lexical_analyzer_unit_test_utilities.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC UNIT_TESTING)
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC PARSER_UNIT_TEST_ONLY)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${VM_SRC_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${COMMON_TEST_DIR}")
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${LEXICAL_TEST_DIR}")

add_test(NAME "parser-unit-test"
         COMMAND "${EXECUTABLE_NAME}")
