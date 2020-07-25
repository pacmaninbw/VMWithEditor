cmake_minimum_required(VERSION 3.16)
project(VMWithEditor C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(VMWithEditor
    arg_flags.c
    arg_flags.h
    common_console_functions.c
    common_console_functions.h
    common_program_logic.c
    common_program_logic.h
    control_console.c
    control_console.h
    file_io_vm.c
    file_io_vm.h
    human_readable_program_format.c
    human_readable_program_format.h
    main.c
    opcode.c
    opcode.h
    text_editor_console.c
    text_editor_console.h
    virtual_machine.c
    virtual_machine.h)
