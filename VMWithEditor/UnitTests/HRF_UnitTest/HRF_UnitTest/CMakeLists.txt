cmake_minimum_required(VERSION 3.16)
project(HRF_UnitTest C)

set(CMAKE_C_STANDARD 90)

add_compile_definitions(UNIT_TESTING)

include_directories(. ../../../ ../../RunAllUnitTests_and_CommonUnitTestCode/RunAllUnitTests_and_CommonUnitTestCode)

add_executable(HRF_UnitTest hrf_unit_test_main.c
        hrf_unit_test_main.h
        ../../RunAllUnitTests_and_CommonUnitTestCode/RunAllUnitTests_and_CommonUnitTestCode/common_unit_test_logic.c
        ../../../human_readable_program_format.c
        ../../../opcode.c
        ../../../virtual_machine.c
        )