cmake_minimum_required(VERSION 3.18.4)

set(EXECUTABLE_NAME "VMWithEditor.exe")

project(${EXECUTABLENAME}.exe LANGUAGES C VERSION 1.0)

set(EXECUTABLE_NAME "VMWithEditor.exe")

add_executable(${EXECUTABLE_NAME} main.c arg_flags.c file_io_vm.c parser.c common_console_functions.c human_readable_program_format.c safe_string_functions.c control_console.c lexical_analyzer.c text_editor_console.c default_program.c virtual_machine.c error_reporting.c opcode.c)

set (CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)

target_include_directories(VMWithEditor.exe PUBLIC "${PROJECT_BINARY_DIR}")


