cmake_minimum_required(VERSION 3.16)
project(HRF_UnitTest C)

set(CMAKE_C_STANDARD 90)

add_compile_definitions(UNIT_TESTING UNIT_TEST_DEBUG REDUCED_VM_AND_HRF_DEPENDENCIES _CRT_SECURE_NO_WARNINGS)

include_directories(. ../../../ ../../Common_UnitTest_Code)

add_executable(State_Machine_UnitTest state_machine_unit_test_main.c
        state_machine_unit_test_main.h
        ../../Common_UnitTest_Code/common_unit_test_logic.c
        ../../../syntax_state_machine.c
        )