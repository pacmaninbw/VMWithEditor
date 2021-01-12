cmake_minimum_required(VERSION 3.16.1)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
    set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
endif()

set(EXECUTABLE_NAME "VMWithEditor.exe")

project(${EXECUTABLE_NAME} LANGUAGES C VERSION 1.0)

add_executable(${EXECUTABLE_NAME} main.c arg_flags.c file_io_vm.c parser.c common_console_functions.c human_readable_program_format.c safe_string_functions.c control_console.c lexical_analyzer.c text_editor_console.c default_program.c virtual_machine.c error_reporting.c opcode.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")


