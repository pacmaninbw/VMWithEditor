
RM := rm -rf

# All paths are relative to the current directory to allow this to build on any
# system. This assumes that the directory structure in GitHub is maintained.
OBJ_DIR := ./obj
BIN_DIR := ./bin
VS_BUILD_DIRS := ./Debug ./Release # For make clean only
VM_SRC_DIR := ../../..
LOCAL_TEST_DIR := ./
COMMON_TEST_DIR := ../../Common_UnitTest_Code
LEXICAL_TEST_DIR := ../../State_Machine_Unit_Test/State_Machine_Unit_Test
PARSER_TEST_DIR := ../../Parser_Unit_Test/Parser_Unit_Test
CMD_LINE_TEST_DIR := ../../CommandLine_UnitTest/CommandLine_UnitTest
HRF_TEST_DIR := ../../HRF_UnitTest/HRF_UnitTest

DIRS := $(OBJ_DIR) $(BIN_DIR)

# The executable changes for each directory, the 2 following variables are
# standard in all the make files
EXECUTABLE := Run_All_Unit_Tests.exe
BINARY_EXECUTABLE := $(BIN_DIR)/$(EXECUTABLE)
LINK_EXECUTABLE := ./$(EXECUTABLE)

##### Object Files ####
VM_DIR_OBJS := error_reporting.o  \
 safe_string_functions.o  arg_flags.o file_io_vm.o opcode.o parser.o \
 default_program.o human_readable_program_format.o lexical_analyzer.o \
 virtual_machine.o
LOCAL_TEST_OBJS := run_all_unit_tests_main.o
COMMON_TEST_OBJS := unit_test_logging.o
LEXICAL_TEST_OBJS := lexical_analyzer_unit_test_main.o \
 internal_character_transition_unit_tests.o internal_sytax_state_tests.o \
 lexical_analyzer_test_data.o lexical_analyzer_unit_test_utilities.o
PARSER_TEST_OBJS := parser_unit_test_main.o parser_unit_test.o internal_parser_tests.o
CMD_LINE_TEST_OBJS := command_line_unit_test_main.o
HRF_TEST_OBJS := hrf_unit_test_main.o unit_test_human_readable_program_format.o
OBJS := $(addprefix $(OBJ_DIR)/, $(LOCAL_TEST_OBJS) $(VM_DIR_OBJS) $(COMMON_TEST_OBJS) \
 $(LEXICAL_TEST_OBJS) $(PARSER_TEST_OBJS) $(CMD_LINE_TEST_OBJS) $(HRF_TEST_OBJS) \
)

##### Source Files ####
VM_DIR_SOURCES := $(addprefix $(VM_SRC_DIR)/, error_reporting.c  \
 safe_string_functions.c  arg_flags.c file_io_vm.c opcode.c parser.c \
 default_program.c human_readable_program_format.c lexical_analyzer.c \
 virtual_machine.c \
)
LOCAL_TEST_SOURCES := (addprefix $(LOCAL_TEST_DIR), run_all_unit_tests_main.c)
COMMON_TEST_SOURCES := $(addprefix $(COMMON_TEST_DIR), unit_test_logging.c)
LEXICAL_TEST_SOURCES := $(addprefix $(LEXICAL_TEST_DIR), lexical_analyzer_unit_test_main.c \
 internal_character_transition_unit_tests.c internal_sytax_state_tests.c \
 lexical_analyzer_test_data.c lexical_analyzer_unit_test_utilities.c \
)
PARSER_TEST_SOURCES := $(addprefix $(PARSER_TEST_DIR), parser_unit_test_main.c \
 internal_parser_tests.c parser_unit_test.c \
 )
CMD_LINE_TEST_SOURCES := $(addprefix $(CMD_LINE_TEST_DIR), command_line_unit_test_main.c)
HRF_TEST_SOURCES := $(addprefix $(HRF_TEST_DIR), hrf_unit_test_main.c \
 unit_test_human_readable_program_format.c \
)

SOURCES := $(VM_TEST_SOURCES) $(COMMON_TEST_SOURCES) $(LOCAL_TEST_SOURCES) \
 $(LEXICAL_TEST_SOURCES) $(PARSER_TEST_SOURCES) $(CMD_LINE_TEST_SOURCES) \
 $(HRF_TEST_SOURCES)

# define the flags used by the compiler
INCLUDE_DIRS := -I$(LOCAL_TEST_DIR) -I$(COMMON_TEST_DIR) -I$(VM_SRC_DIR) \
 -I$(LEXICAL_TEST_DIR) -I$(PARSER_TEST_DIR) -I$(CMD_LINE_TEST_DIR) -I$(HRF_TEST_DIR)
DEFINES := -DUNIT_TESTING -DALL_UNIT_TESTING
C_STANDARD := -std=c99
OPTIMIZATION := -g
WARNINGS := -Wall
CC := gcc
CCFLAGS := $(C_STANDARD) $(DEFINES) $(INCLUDE_DIRS) $(OPTIMIZATION) $(WARNINGS) -c

# Common recipe for compiling the code.
define Compile_Source =
@echo 'Generating object file for: $<'
$(CC) $(CCFLAGS) -o "$@" "$<"
@echo 'Finished building: $@'
@echo ' '
endef

# All Target
all: $(LINK_EXECUTABLE)

$(OBJ_DIR)/%.o: $(VM_SRC_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(COMMON_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(LOCAL_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(LEXICAL_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(PARSER_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(CMD_LINE_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(HRF_TEST_DIR)/%.c
	$(Compile_Source)

# Specific source to object rules
$(OBJ_DIR)/Editor_UnitTest_Main.o: $(LOCAL_TEST_DIR)/Editor_UnitTest_Main.c \
 $(LOCAL_TEST_DIR)/Editor_UnitTest_Main.h  $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/ARGF_arg_flags.h

$(OBJ_DIR)/unit_test_logging.o: $(COMMON_TEST_DIR)/unit_test_logging.c \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/error_reporting.o: $(VM_SRC_DIR)/error_reporting.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/safe_string_functions.o: $(VM_SRC_DIR)/safe_string_functions.c \
 $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/arg_flags.o: $(VM_SRC_DIR)/arg_flags.c  $(VM_SRC_DIR)/ERH_error_reporting.h\
 $(VM_SRC_DIR)/ARGF_arg_flags.h

$(OBJ_DIR)/parser_unit_test_main.o: $(PARSER_TEST_DIR)/parser_unit_test_main.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/LAH_lexical_analyzer.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_main.h \
 $(PARSER_TEST_DIR)/internal_parser_tests.h $(VM_SRC_DIR)/parser.h \
 $(PARSER_TEST_DIR)/parser_unit_test.h $(COMMON_TEST_DIR)/UTL_unit_test_logging.h

$(OBJ_DIR)/parser_unit_test.o: $(PARSER_TEST_DIR)/parser_unit_test.c \
 $(VM_SRC_DIR)/parser.h $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/HRF_human_readable_program_format.h $(VM_SRC_DIR)/OPC_opcode.h

$(OBJ_DIR)/internal_parser_tests.o: $(PARSER_TEST_DIR)/internal_parser_tests.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h #\
# $(VM_SRC_DIR)/HRF_human_readable_program_format.h \
# $(VM_SRC_DIR)/LAH_lexical_analyzer.h \   $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
# $(PARSER_TEST_DIR)/internal_parser_tests.h \
# $(VM_SRC_DIR)/OPC_opcode.h

$(OBJ_DIR)/internal_character_transition_unit_tests.o: \
 $(LEXICAL_TEST_DIR)/internal_character_transition_unit_tests.c \
 $(VM_SRC_DIR)/lexical_analyzer.c $(VM_SRC_DIR)/vmwitheditor.h \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(VM_SRC_DIR)/LAH_lexical_analyzer.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_utilities.h \
 $(LEXICAL_TEST_DIR)/internal_character_transition_unit_tests.h

$(OBJ_DIR)/internal_sytax_state_tests.o: $(LEXICAL_TEST_DIR)/internal_sytax_state_tests.c \
 $(VM_SRC_DIR)/lexical_analyzer.c $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/LAH_lexical_analyzer.h \
 $(LEXICAL_TEST_DIR)/internal_character_transition_unit_tests.h \
 $(LEXICAL_TEST_DIR)/internal_sytax_state_tests.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_test_data.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_utilities.h

$(OBJ_DIR)/lexical_analyzer_test_data.o: $(LEXICAL_TEST_DIR)/lexical_analyzer_test_data.c \
 $(VM_SRC_DIR)/SSF_safe_string_functions.h $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(LEXICAL_TEST_DIR)/lexical_analyzer_test_data.h

$(OBJ_DIR)/lexical_analyzer_unit_test_main.o: \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_main.c \
 $(VM_SRC_DIR)/LAH_lexical_analyzer.h $(LEXICAL_TEST_DIR)/internal_sytax_state_tests.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_main.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h 

$(OBJ_DIR)/lexical_analyzer_unit_test_utilities.o: \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_utilities.c \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_utilities.h

$(OBJ_DIR)/hrf_unit_test_main.o: $(HRF_TEST_DIR)/hrf_unit_test_main.c \
 $(HRF_TEST_DIR)/hrf_unit_test_main.h  $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(HRF_TEST_DIR)/unit_test_human_readable_program_format.h \
 $(VM_SRC_DIR)/HRF_human_readable_program_format.h

$(OBJ_DIR)/unit_test_human_readable_program_format.o: \
 $(HRF_TEST_DIR)/unit_test_human_readable_program_format.c \
 $(HRF_TEST_DIR)/unit_test_human_readable_program_format.h \
 $(VM_SRC_DIR)/default_program.h  $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(VM_SRC_DIR)/HRF_human_readable_program_format.h


#Tell make that all the header files exist and where they are
$(VM_SRC_DIR)/%.h:
$(VM_TEST_DIR)/%.h:
$(COMMON_TEST_DIR)/%.h:
$(LEXICAL_TEST_DIR)/%.h:
$(PARSER_TEST_DIR)/%.h:
$(CMD_LINE_TEST_DIR)/%.h:
$(HRF_TEST_DIR)/%.h:

$(LINK_EXECUTABLE): $(DIRS) $(BINARY_EXECUTABLE)
	ln -s $(BINARY_EXECUTABLE) .

$(BINARY_EXECUTABLE): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "$(BINARY_EXECUTABLE)" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LINK_EXECUTABLE) $(BINARY_EXECUTABLE) $(DIRS) $(VS_BUILD_DIRS) makelog.txt
	-@echo ' '

MKDIR_P := mkdir -p

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

${BIN_DIR}:
	${MKDIR_P} ${BIN_DIR}

.PHONY: all clean $(DIRS)
.SECONDARY:
