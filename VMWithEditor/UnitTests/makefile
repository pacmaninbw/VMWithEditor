
RM := rm -rf

# All paths are relative to the current directory to allow this to build on any
# system. This assumes that the directory structure in GitHub is maintained.
HRF_TEST_DIR := ./HRF_UnitTest/HRF_UnitTest
LEX_TEST_DIR := ./State_Machine_Unit_Test/State_Machine_Unit_Test
PARSER_TEST_DIR := ./Parser_Unit_Test/Parser_Unit_Test
VM_TEST_DIR := ./VirtualMachine_UnitTest/VirtualMachine_UnitTest
CMD_LINE_TEST_DIR := ./CommandLine_UnitTest/CommandLine_UnitTest
EDITOR_TEST_DIR := ./Editor_UnitTest/Editor_UnitTest
CTRL_CONSOLE_TEST_DIR := ./ControlConsole_UnitTest/ControlConsole_UnitTest
ALL_UNIT_TEST_DIR := ./RunAllUnitTests/RunAllUnitTests

HRF_TEST_EXE := $(HRF_TEST_DIR)/HRF_Unit_Test.exe
LEX_TEST_EXE := $(LEX_TEST_DIR)/Lexical_Analyzer_Unit_Test.exe
PARSER_TEST_EXE := $(PARSER_TEST_DIR)/Parser_Unit_Test.exe
VM_TEST_EXE := $(VM_TEST_DIR)/VirtualMachine_UnitTest.exe
CMD_TEST_EXE := $(CMD_LINE_TEST_DIR)/CommandLine_UnitTest.exe
CTRL_CONSOLE_TEST_EXE := $(CTRL_CONSOLE_TEST_DIR)/Control_Console_Unit_Test.exe
EDITOR_TEST_EXE := $(EDITOR_TEST_DIR)/Editor_Unit_Test.exe
ALL_UNIT_TEST_EXE := $(ALL_UNIT_TEST_DIR)/Run_All_Unit_Tests.exe

DIRS := $(OBJ_DIR) $(BIN_DIR)

# The executable changes for each directory, the 2 following variables are
# standard in all the make files
EXECUTABLE := Run_All_Unit_Tests.exe
ALL_TEST_EXES := $(HRF_TEST_EXE) $(LEX_TEST_EXE) $(PARSER_TEST_EXE) \
 $(VM_TEST_EXE) $(CMD_TEST_EXE) $(CTRL_CONSOLE_TEST_EXE) \
 $(EDITOR_TEST_EXE) $(ALL_UNIT_TEST_EXE)

# Common recipe for compiling the code.
define Compile_Source =
@echo 'Generating object file for: $<'
$(CC) $(CCFLAGS) -o "$@" "$<"
@echo 'Finished building: $@'
@echo ' '
endef

# All Target
all: $(ALL_TEST_EXES)

$(ALL_TEST_EXES): 

$(HRF_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(HRF_TEST_DIR)'
	cd $(HRF_TEST_DIR) && $(MAKE) >& makelog.txt

$(LEX_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(LEX_TEST_DIR)'
	cd $(LEX_TEST_DIR) && $(MAKE) >& makelog.txt

$(PARSER_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(PARSER_TEST_DIR)'
	cd $(PARSER_TEST_DIR) && $(MAKE) >& makelog.txt

$(VM_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(VM_TEST_DIR)'
	cd $(VM_TEST_DIR) && $(MAKE) >& makelog.txt

$(CMD_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(CMD_LINE_TEST_DIR)'
	cd $(CMD_LINE_TEST_DIR) && $(MAKE) >& makelog.txt

$(CTRL_CONSOLE_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(CTRL_CONSOLE_TEST_DIR)'
	cd $(CTRL_CONSOLE_TEST_DIR) && $(MAKE) >& makelog.txt

$(EDITOR_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(EDITOR_TEST_DIR)'
	cd $(EDITOR_TEST_DIR) && $(MAKE) >& makelog.txt

$(ALL_UNIT_TEST_EXE):
	@echo 'Making $@'
	@echo 'cd $(ALL_UNIT_TEST_DIR)'
	cd $(ALL_UNIT_TEST_DIR) && $(MAKE) >& makelog.txt

# Other Targets
clean: cleanHRF cleanLEX cleanPARSE cleanVM cleanCMDLINE cleanCTRLCONSOLE \
 cleanEDITOR cleanALLUNITTESTS
	$(RM) makelog.txt buildDebuglog.txt buildReleaselog.txt
	-@echo 'make clean complete'

cleanHRF:
	@echo 'Cleaning HRF Test'
	@echo 'cd $(HRF_TEST_DIR)'
	cd $(HRF_TEST_DIR) && $(MAKE) clean

cleanLEX:
	@echo 'Cleaning Lexical Test'
	@echo 'cd $(LEX_TEST_DIR)'
	cd $(LEX_TEST_DIR) && $(MAKE) clean

cleanPARSE:
	@echo 'Cleaning Parser Test'
	@echo 'cd $(PARSER_TEST_DIR)'
	cd $(PARSER_TEST_DIR) && $(MAKE) clean

cleanVM:
	@echo 'Cleaning Virtual Machine Test'
	@echo 'cd $(VM_TEST_DIR)'
	cd $(VM_TEST_DIR) && $(MAKE) clean

cleanCMDLINE:
	@echo 'Cleaning Commandline Test'
	@echo 'cd $(CMD_LINE_TEST_DIR)'
	cd $(CMD_LINE_TEST_DIR) && $(MAKE) clean

cleanCTRLCONSOLE:
	@echo 'Cleaning Control Console Test'
	@echo 'cd $(CTRL_CONSOLE_TEST_DIR)'
	cd $(CTRL_CONSOLE_TEST_DIR) && $(MAKE) clean

cleanEDITOR:
	@echo 'Cleaning Editor Test'
	@echo 'cd $(EDITOR_TEST_DIR)'
	cd $(EDITOR_TEST_DIR) && $(MAKE) clean

cleanALLUNITTESTS:
	@echo 'Cleaning Run All Unit Tests'
	@echo 'cd $(ALL_UNIT_TEST_DIR)'
	cd $(ALL_UNIT_TEST_DIR) && $(MAKE) clean


.PHONY: all clean $(DIRS)
.SECONDARY:
