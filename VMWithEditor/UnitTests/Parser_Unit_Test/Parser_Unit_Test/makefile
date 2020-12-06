
RM := rm -rf

# All paths are relative to the current directory to allow this to build on any
# system. This assumes that the directory structure in GitHub is maintained.
OBJ_DIR := ./obj
BIN_DIR := ./bin
VS_BUILD_DIRS := ./Debug ./Release	# For make clean only
VM_SRC_DIR := ../../..
LOCAL_TEST_DIR := ./
COMMON_TEST_DIR := ../../Common_UnitTest_Code
LEXICAL_TEST_DIR := ../../State_Machine_Unit_Test/State_Machine_Unit_Test

DIRS := $(OBJ_DIR) $(BIN_DIR)

# The executable changes for each directory, the 2 following variables are
# standard in all the make files
EXECUTABLE := Parser_Unit_Test.exe
BINARY_EXECUTABLE := $(BIN_DIR)/$(EXECUTABLE)
LINK_EXECUTABLE := ./$(EXECUTABLE)

OBJS := $(addprefix $(OBJ_DIR)/, error_reporting.o unit_test_logging.o opcode.o \
 parser.o human_readable_program_format.o lexical_analyzer.o virtual_machine.o \
 internal_character_transition_unit_tests.o internal_parser_tests.o \
 internal_sytax_state_tests.o lexical_analyzer_test_data.o \
 lexical_analyzer_unit_test_main.o lexical_analyzer_unit_test_utilities.o \
 parser_unit_test_main.o parser_unit_test.o safe_string_functions.o \
)

VM_DIR_SOURCES := $(addprefix $(VM_SRC_DIR)/, error_reporting.c human_readable_program_format.c \
lexical_analyzer.c opcode.c parser.c safe_string_functions.c  virtual_machine.c)

LEXICAL_TEST_SOURCES := $(addprefix $(LEXICAL_TEST_DIR)/, internal_character_transition_unit_tests.c \
 internal_sytax_state_tests.c lexical_analyzer_test_data.c lexical_analyzer_unit_test_main.c \
 lexical_analyzer_unit_test_utilities.c)

LOCAL_TEST_SOURCES := $(addprefix $(LOCAL_TEST_DIR)/, internal_parser_tests.c \
 parser_unit_test_main.c parser_unit_test.c) 

COMMON_TEST_SOURCES := $(addprefix $(COMMON_TEST_DIR), unit_test_logging.c)

SOURCES := $(VM_DIR_SOURCES) $(COMMON_TEST_SOURCES) $(LEXICAL_TEST_SOURCES) $(LOCAL_TEST_SOURCES)

# define the flags used by the compiler
INCLUDE_DIRS := -I$(LOCAL_TEST_DIR) -I$(COMMON_TEST_DIR) -I$(LEXICAL_TEST_DIR) -I$(VM_SRC_DIR)
DEFINES := -DUNIT_TESTING -DPARSER_UNIT_TEST_ONLY
C_STANDARD := -std=c99
OPTIMIZATION := -g
WARNINGS := -Wall -Wno-unused-function
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

$(OBJ_DIR)/%.o: $(LEXICAL_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(LOCAL_TEST_DIR)/%.c
	$(Compile_Source)

$(OBJ_DIR)/%.o: $(COMMON_TEST_DIR)/%.c
	$(Compile_Source)

# Specific source to object rules
$(OBJ_DIR)/unit_test_logging.o: $(COMMON_TEST_DIR)/unit_test_logging.c \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/error_reporting.o: $(VM_SRC_DIR)/error_reporting.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/opcode.o: $(VM_SRC_DIR)/opcode.c $(VM_SRC_DIR)/OPC_opcode.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/vmwitheditor.h

$(OBJ_DIR)/parser.o: $(VM_SRC_DIR)/parser.c $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(VM_SRC_DIR)/parser.h $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/HRF_human_readable_program_format.h \
 $(VM_SRC_DIR)/LAH_lexical_analyzer.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/VMH_virtual_machine.h $(COMMON_TEST_DIR)/UTL_unit_test_logging.h

$(OBJ_DIR)/human_readable_program_format.o: $(VM_SRC_DIR)/human_readable_program_format.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/HRF_human_readable_program_format.h \
 $(VM_SRC_DIR)/LAH_lexical_analyzer.h $(VM_SRC_DIR)/VMH_virtual_machine.h

$(OBJ_DIR)/safe_string_functions.o: $(VM_SRC_DIR)/safe_string_functions.c \
 $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/lexical_analyzer.o: $(VM_SRC_DIR)/lexical_analyzer.c \
 $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/LAH_lexical_analyzer.h

$(OBJ_DIR)/virtual_machine.o: $(VM_SRC_DIR)/virtual_machine.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/VMH_virtual_machine.h

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

$(OBJ_DIR)/parser_unit_test_main.o: $(LOCAL_TEST_DIR)/parser_unit_test_main.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/LAH_lexical_analyzer.h \
 $(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_main.h \
 $(LOCAL_TEST_DIR)/internal_parser_tests.h $(VM_SRC_DIR)/parser.h \
 $(LOCAL_TEST_DIR)/parser_unit_test.h $(COMMON_TEST_DIR)/UTL_unit_test_logging.h

$(OBJ_DIR)/parser_unit_test.o: $(LOCAL_TEST_DIR)/parser_unit_test.c \
 $(VM_SRC_DIR)/parser.h $(VM_SRC_DIR)/ERH_error_reporting.h \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h \
 $(VM_SRC_DIR)/HRF_human_readable_program_format.h $(VM_SRC_DIR)/OPC_opcode.h

$(OBJ_DIR)/internal_parser_tests.o: $(LOCAL_TEST_DIR)/internal_parser_tests.c $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/HRF_human_readable_program_format.h $(VM_SRC_DIR)/LAH_lexical_analyzer.h $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(LOCAL_TEST_DIR)/internal_parser_tests.h $(VM_SRC_DIR)/OPC_opcode.h

$(VM_SRC_DIR)/%.h:

$(LEXICAL_TEST_DIR)/%.h:

$(LOCAL_TEST_DIR)/%.h:

$(COMMON_TEST_DIR)/%.h:

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
