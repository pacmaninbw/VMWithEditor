
RM := rm -rf

OBJ_DIR := obj

BIN_DIR := bin

SRC_DIR := # src/

VM_SRC_DIR := ../../..

COMMON_TEST_DIR := ../../Common_UnitTest_Code

LEXICAL_TEST_DIR := ./

DIRS := $(OBJ_DIR) $(BIN_DIR)

INCLUDE_DIRS := -I$(COMMON_TEST_DIR) -I$(LEXICAL_TEST_DIR) -I$(VM_SRC_DIR)

EXECUTABLES := $(BIN_DIR)/Lexical_Analyzer_Unit_Test.exe

# All Target
all: $(DIRS) $(EXECUTABLES)

OBJS := $(addprefix $(OBJ_DIR)/, error_reporting.o \
 unit_test_logging.o \
 lexical_analyzer.o internal_character_transition_unit_tests.o \
 internal_sytax_state_tests.o lexical_analyzer_test_data.o \
 lexical_analyzer_unit_test_main.o lexical_analyzer_unit_test_utilities.o \
 safe_string_functions.o \
)

VM_DIR_SOURCES := $(addprefix $(VM_SRC_DIR)/, error_reporting.c \
lexical_analyzer.c safe_string_functions.c )

LEXICAL_TEST_SOURCES := $(addprefix $(LEXICAL_TEST_DIR)/, internal_character_transition_unit_tests.c \
 internal_sytax_state_tests.c lexical_analyzer_test_data.c lexical_analyzer_unit_test_main.c \
 lexical_analyzer_unit_test_utilities.c)


COMMON_TEST_SOURCES := $(addprefix $(COMMON_TEST_DIR), unit_test_logging.c)

SOURCES := $(VM_DIR_SOURCES) $(COMMON_TEST_SOURCES) $(LEXICAL_TEST_SOURCES)

DEFINES := -DUNIT_TESTING -DLEXICAL_UNIT_TEST_ONLY

$(OBJ_DIR)/%.o: $(VM_SRC_DIR)/%.c
	@echo 'Building file: $@ from $<'
	@echo 'Invoking: GCC C++ Compiler'
#	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -O3 -Wall -c -o "$@" "$<" # Production
	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -g3 -Wall -c -o "$@" "$<" # Debug
	@echo 'Finished building: $@'
	@echo ' '

$(OBJ_DIR)/%.o: $(LEXICAL_TEST_DIR)/%.c
	@echo 'Building file: $@ from $<'
	@echo 'Invoking: GCC C++ Compiler'
#	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -O3 -Wall -c -o "$@" "$<" # Production
	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -g3 -Wall -c -o "$@" "$<" # Debug
	@echo 'Finished building: $@'
	@echo ' '

$(OBJ_DIR)/%.o: $(COMMON_TEST_SOURCES)/%.c
	@echo 'Building file: $@ from $<'
	@echo 'Invoking: GCC C++ Compiler'
#	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -O3 -Wall -c -o "$@" "$<" # Production
	gcc -std=c99 $(DEFINES) $(INCLUDE_DIRS) -g3 -Wall -c -o "$@" "$<" # Debug
	@echo 'Finished building: $@'
	@echo ' '

#

$(OBJ_DIR)/error_reporting.o: $(VM_SRC_DIR)/error_reporting.c \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/safe_string_functions.o: $(VM_SRC_DIR)/safe_string_functions.c \
 $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

$(OBJ_DIR)/lexical_analyzer.o: $(VM_SRC_DIR)/lexical_analyzer.c \
 $(VM_SRC_DIR)/vmwitheditor.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h $(VM_SRC_DIR)/LAH_lexical_analyzer.h

$(OBJ_DIR)/unit_test_logging.o: $(COMMON_TEST_DIR)/unit_test_logging.c \
 $(COMMON_TEST_DIR)/UTL_unit_test_logging.h $(VM_SRC_DIR)/SSF_safe_string_functions.h \
 $(VM_SRC_DIR)/ERH_error_reporting.h

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

$(VM_SRC_DIR)/ERH_error_reporting.h:

$(VM_SRC_DIR)/LAH_lexical_analyzer.h:

$(VM_SRC_DIR)/OPC_opcode.h:

$(VM_SRC_DIR)/SSF_safe_string_functions.h:

$(VM_SRC_DIR)/VMH_virtual_machine.h:

$(VM_SRC_DIR)/vmwitheditor.h:

$(LEXICAL_TEST_DIR)/internal_character_transition_unit_tests.h:

$(LEXICAL_TEST_DIR)/internal_sytax_state_tests.h:

$(LEXICAL_TEST_DIR)/lexical_analyzer_test_data.h:

$(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_main.h:

$(LEXICAL_TEST_DIR)/lexical_analyzer_unit_test_utilities.h:

$(EXECUTABLES): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "$(BIN_DIR)/Parser_Unit_Test.exe" $(OBJS)
	ln -s $(EXECUTABLES) .
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(DIRS) $(EXECUTABLES)
	-@echo ' '

MKDIR_P := mkdir -p

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

${BIN_DIR}:
	${MKDIR_P} ${BIN_DIR}

.PHONY: all clean $(DIRS)
.SECONDARY:


