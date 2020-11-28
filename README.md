Under Construction!

## VWWithEditor
Virtual Machine With Editor

The code in this repository is the basis for several up coming question on [Code Review](https://codereview.stackexchange.com/). The original concept is based on [this question on Code Review](https://codereview.stackexchange.com/questions/244566/an-attempt-at-a-toy-vm). Some things that I didn't mention in the review:  
 - The program wasn’t modularized enough (the code was tightly coupled between modules).  
 - The vm struct allowed too much access to the internals of the virtual machine.  
 - there should be a user friendly method to create programs to run on that virtual machine, programming in hex may be easy for some, but not so easy for others.
 - Either command line arguments need to be allowed for input, output and other essential items or a command interface such as a command console needs to be added.
 - The entire program was really a unit test.

The unit test for the lexical analyzer (state machine) was reviewed on [code review](https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a). The code reviews have been added to the proper unit test directories in this repository. The code reviewed will stay unchanged in the `Before_First_Code_Review` branch.

## Additions to the Original Program  
This program adds:  
 - A menu driven Command Interface Console (text based although a GUI could be provided).  
 - An Assembly style language loosely based on JSON.  
 - A menu driven Editor Interface Console (text based).  
 - The ability to read the Assembly style language from a text file.  
 - The ability to write the Assembly style language to a text file.  
 - A command line processing capability to bypass the Command Interface Console.  

## Unit Tests  
Unit tests have been developed or are being developed as stand alone programs for each major portion of the program, there will also be a unit test to run all of the portions of the code together. The unit tests are not currently utilizing any unit test framework.  
 - Command Console unit test  
 - Lexical Analyzer unit test (Completed)  
 - Parser unit test (Windows 10 Visual Studio 2019 version is complete Linux gcc version still undergoing testing).  
 - File input and output unit test
 - Editor Console unit test
 - Command Line unit test.  
 - Virtual Machine execution unit test

## Modifications to the Original Program  
The parts of the original program contained in `vm.h` and `vm.c` have been rewritten to change coupling from a `data` coupling to accessor functions, removing any specific knowledge about the virtual machine in other parts of the program. 

## All Development Environments
C programming language - C99
Modular Design

## Linux Development Environment
Two unit test directories currently have Linux makefiles. The current Linux build environment is Linux on Windows using CentOS 8.1, the compiler is gcc. The standard has been switched from C90 to C99.

## My Original Development Environment.
 - Visual Studion 2019 Professional.  
 - Windows 10 Professional.  

## Building the Executables
Both Visual Studio 2019 and Linux builds are supported. Microsoft Visual Studio solution files and project files as well as CMakeLists.txt files are provided.

