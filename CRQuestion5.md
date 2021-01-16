Portable Build System for Virtual Machine with Editor and Unit Tests

I am automating the building and unit testing of a [personal
project](https://github.com/pacmaninbw/VMWithEditor) using shell scripts
(parts of the unit testing have previously been reviewed on Code Review
[A](https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a),
[B](https://codereview.stackexchange.com/questions/248560/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-b),
[C](https://codereview.stackexchange.com/questions/248561/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-c),
[C2](https://codereview.stackexchange.com/questions/248817/common-unit-testing-code-follow-up)), CMake and make on the latest version of Fedora Linux. I have also tested building on the latest version of Ubuntu. I had to decrease the minimum CMake version on Ubuntu to make it work.

My original development environment was/is Visual Studio 2019 on Windows 10 Pro, however, to make it easier to get reviews and to create a portable system and application I have developed this build system as well.  

It is possible that I could have used CMake for the entire build system, but one of the requirements for this system is that each unit test can build as a separate unit test as well as being combined into other unit tests for regression testing purposes. Each unit test needs to stand on its own because I am using the unit tests to debug the core code, as
well as unit test it. Using only CMake created only one object and binary tree and that was not the intention.  

The unit tests themselves are not automated yet, that is the next step in the project. There are currently 2 unit tests that have been completed, the lexical analyzer and the parser. All the other unit tests are an empty shell at this point.  

## Requirements:
1.  Build on any system that supports the original Borne Shell and CMake.  
2.  Build the unit tests as individual unit tests and as a single unit test that runs all the previous unit tests.  
3.  Use regression testing in each progressive unit test to make sure the new code doesn’t break the previous functionality.  
4.  Build the primary application after all the unit tests have been built.  

## What I want out of this review:  
1.  I have tested the build on Fedora and Ubuntu, I would appreciate if someone test the build on Mac OSX, my Mac died 3 years ago.  
2.  It’s been a long time since I’ve written shell scripts (at least 6 years and really much longer than that for complex shell scripts).  
    1.  Do my shell scripts follow best practices?  
    2.  How can I improve them?  
    3.  Do you see any portability problems with them?  
3.  I’ve never written CMake scripts before, all suggestions will be helpful.  
4.  It may be that this last request is off-topic, but how could I build this on Windows 10 using the scripts and CMake? That would make the build system truly portable.  

You can review only the shell scripts or only the CMake code if you prefer. The shell scripts are first follow by 3 CMakeLists.txt files.  

## Build Directory Structure and Build Files  

    VMWithEditor  
        buildAll.sh  
        buildClean.sh  
    
        VMWithEditor/VMWithEditor:
            buildDebug.sh
            buildRelease.sh
            CMakeLists.txt
    
            VMWithEditor/VMWithEditor/UnitTests:
                buildAllDebug.sh
                buildAllRelease.sh
    
                VMWithEditor/VMWithEditor/UnitTests/CommandLine_UnitTest/CommandLine_UnitTest:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/Common_UnitTest_Code:
                    CodeReview.md
                    unit_test_logging.c
                    UTL_unit_test_logging.h
    
                VMWithEditor/VMWithEditor/UnitTests/ControlConsole_UnitTest/ControlConsole_UnitTest:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/Editor_UnitTest/Editor_UnitTest:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/HRF_UnitTest/HRF_UnitTest:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/Parser_Unit_Test/Parser_Unit_Test:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/RunAllUnitTests/RunAllUnitTests:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/State_Machine_Unit_Test/State_Machine_Unit_Test:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt
    
                VMWithEditor/VMWithEditor/UnitTests/VirtualMachine_UnitTest/VirtualMachine_UnitTest:
                    buildDebug.sh
                    buildRelease.sh
                    CMakeLists.txt

# The Code  
I am presenting the shell scripts first and then the CMakeLists.txt files.

## Top Shell Script Level Code  

### VMWithEditor/buildAll.sh

    #! /usr/bin/sh
    #
    # Build the input version of the Virtual MAchine and all the unit tests
    # Stop on any build errors.
    #
    if [ -z "$1" ] ; then
        echo "Usage: build.sh BUILDTYPE where BUILDTYPE is Debug or Release."
        exit 1
    elif [ "$1" != 'Debug' ] && [ "$1" != 'Release' ] ; then
        printf "\n unknow build type %s \n" "$1"
        exit 1
    fi
    #
    # Build the necessary variables
    #
    BUILDTYPE="$1"
    UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
    SHELLFILE="buildAll${BUILDTYPE}.sh";
    VMSHELLFILE="build${BUILDTYPE}.sh";
    FULLSPECSHELLFILE="${UNITTESTDIRECTORY}/${SHELLFILE}";
    LOGFILE="build${BUILDTYPE}log.txt"
    #
    # Execute the build scripts
    #
    # Build The Unit Tests
    #
    if [ -d "${UNITTESTDIRECTORY}" ] ; then
        if [ -f "${FULLSPECSHELLFILE}" ] ; then
            echo "Building $UNITTESTDIRECTORY";
    	cd "${UNITTESTDIRECTORY}" || exit
            ./"${SHELLFILE}" > "${LOGFILE}" 2>&1 
            retVal=$?
            if [ $retVal -ne 0 ]; then
                echo "Unit Test Build Failed!"
                exit $retVal
            fi
            cd ../ || exit
        fi
    #
    # Build the Virtual Machine with Editor
    #
        if [ -f "./buildDebug.sh" ] ; then
            ./"${VMSHELLFILE}" > "${LOGFILE}" 2>&1
            retVal=$?
            if [ ${retVal} -ne 0 ]; then
                echo "Virtual Machine With Editor Build Failed!"
                echo "Check logs for details"
                exit ${retVal}
            else
                printf "%s Version Virtual Machine With Editor Build and Unit Test Build Completed!\n" "${BUILDTYPE}"
                exit 0
            fi
        fi
    fi

### VMWithEditor/buildClean.sh  

    #! /usr/bin/bash
    #
    # Build the release version of the Virtual Machine and all the unit tests
    # Stop on any build errors.
    #
    UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
    if [ -d "$UNITTESTDIRECTORY" ] ; then
        cd "$UNITTESTDIRECTORY" || exit
        make clean
        retVal=$?
        if [ $retVal -ne 0 ]; then
            exit $retVal
        fi
        cd ../ || exit
        make clean
    fi

## Middle Layer Shell Scripts  
The 2 following shell scripts are in the UnitTests directory:  

### buildAllDebug.sh  

    #! /usr/bin/bash
    
    # Build the debug version of all the unit tests
    # Stop on any build errors.
    
    for i in * 
    do
        if [ -d $i ] ; then
            TESTDIRECTORY="$i/$i"
            SHELLFILE="$TESTDIRECTORY/buildDebug.sh";
            if [ -f $SHELLFILE ] ; then
                echo "Building $TESTDIRECTORY";
    	    cd "$TESTDIRECTORY"
                ./buildDebug.sh >& buildDebuglog.txt
                retVal=$?
                if [ $retVal -ne 0 ]; then
                    exit $retVal
                fi
                cd ../..
            fi
        fi
    done;
    
### buildAllRelease.sh  

    #! /usr/bin/bash
    
    # Build the debug version of all the unit tests
    # Stop on any build errors.
    
    for i in * 
    do
        if [ -d $i ] ; then
            TESTDIRECTORY="$i/$i"
            SHELLFILE="$TESTDIRECTORY/buildRelease.sh";
            if [ -f $SHELLFILE ] ; then
                echo "Building $TESTDIRECTORY";
    	    cd "$TESTDIRECTORY"
                ./buildRelease.sh >& buildReleaselog.txt
                retVal=$?
                if [ $retVal -ne 0 ]; then
                    exit $retVal
                fi
                cd ../..
            fi
        fi
    done;
    

## Lowest Level Shell Scripts  
The following 2 shell scripts are in all the unit test directories where cmake is executed, the first builds a debugable version the second builds an optimized release version.

### buildDebug.sh  
    
    #! /bin/sh
    
    # Creat a Debug build directory and then build the target within the Debug directory
    # Stop on any build errors and stop the parent process.
    
    mkdir Debug
    cd Debug || exit
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    retVal=$?
    if [ $retVal -ne 0 ]; then
        printf "\n\ncmake failed %s!\n\n" "$retVal"
        exit $retVal
    fi
    make VERBOSE=1
    retVal=$?
    if [ $retVal -ne 0 ]; then
        printf "\n\nmake failed! %s\n\n" "$retVal"
        exit $retVal
    fi

### buildRelease.sh  

    #! /bin/sh
    
    # Creat a Release build directory and then build the target within the Release directory
    # Stop on any build errors and stop the parent process.
    
    mkdir Release
    cd Release || exit
    cmake -DCMAKE_BUILD_TYPE=Release ..
    retVal=$?
    if [ $retVal -ne 0 ]; then
        printf "\n\ncmake failed %s!\n\n" "$retVal"
        exit $retVal
    fi
    make
    retVal=$?
    if [ $retVal -ne 0 ]; then
        printf "\n\nmake failed! %s\n\n" "$retVal"
        exit $retVal
    fi

# The CMake Files:  
There are 2.3 unit tests that actually test the existing code and one unit test that includes all the other unit tests which is working to the extent that the two existing unit tests work (testing is successful for all three tests). The first 2 CMake files presented are the lexical analyzer unit test and the parser unit test. The lexical analyzer unit test is fully complete and was used to debug the lexical analyzer. The parser unit test is complete, it executes the lexical analyzer unit tests prior to executing the parser unit tests. The parser unit test was used to debug the parser code in the main project.  

## The Lexical Analyzer Unit Test CMakeLists.txt file:  

    cmake_minimum_required(VERSION 3.16.1)
    
    set(EXECUTABLE_NAME "Lexical_Analyzer_Unit_Test.exe")
    
    project(${EXECUTABLE_NAME} LANGUAGES C VERSION 1.0)
    
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(GCC_WARN_COMPILE_FLAGS  " -Wall ")
        set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARN_COMPILE_FLAGS}")
    endif()
    
    set(VM_SRC_DIR "../../..")
    set(COMMON_TEST_DIR "../../Common_UnitTest_Code")
    
    add_executable(${EXECUTABLE_NAME} internal_character_transition_unit_tests.c internal_sytax_state_tests.c lexical_analyzer_test_data.c lexical_analyzer_unit_test_main.c lexical_analyzer_unit_test_utilities.c ${VM_SRC_DIR}/error_reporting.c ${VM_SRC_DIR}/lexical_analyzer.c ${VM_SRC_DIR}/safe_string_functions.c ${COMMON_TEST_DIR}/unit_test_logging.c)
    
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED True)
    
    configure_file(VMWithEditorConfig.h.in VMWithEditorConfig.h)
    
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC UNIT_TESTING)
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC LEXICAL_UNIT_TEST_ONLY)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
    target_include_directories(${EXECUTABLE_NAME} PRIVATE "${VM_SRC_DIR}")
    target_include_directories(${EXECUTABLE_NAME} PRIVATE "${COMMON_TEST_DIR}")
    
    
## The Parser Unit Test CMakeLists.txt file:  

    cmake_minimum_required(VERSION 3.16.1)
    
    set(EXECUTABLE_NAME "Parser_Unit_Test.exe")
    
    project(${EXECUTABLE_NAME} LANGUAGES C VERSION 1.0)
    
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
    

## The RunAllUnitTests CMakeLists.txt file:  
This file is the most complex of all the CMakeLists.txt files. It includes code from 7 other unit tests.  

    cmake_minimum_required(VERSION 3.16.1)
    
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
    
    
