
RM := rm -rf

# All paths are relative to the current directory to allow this to build on any
# system. This assumes that the directory structure in GitHub is maintained.
OBJ_DIR := ./obj
BIN_DIR := ./bin
VS_BUILD_DIRS := ./Debug ./Release # For make clean only
VM_SRC_DIR := ./
UNIT_TEST_DIR := ./UnitTests
PARSER_TEST_DIR := $(UNIT_TEST_DIR)/Parser_Unit_Test/Parser_Unit_Test
COMMON_TEST_DIR := $(UNIT_TEST_DIR)/Common_UnitTest_Code
LEXICAL_TEST_DIR := $(UNIT_TEST_DIR)/State_Machine_Unit_Test/State_Machine_Unit_Test

DIRS := $(OBJ_DIR) $(BIN_DIR)

EXECUTABLE := VMWithEditor.exe
BINARY_EXECUTABLE := $(BIN_DIR)/$(EXECUTABLE)
LINK_EXECUTABLE := ./$(EXECUTABLE)
TEST_EXECUTABLES :=

OBJS := $(addprefix $(OBJ_DIR)/, arg_flags.o common_console_functions.o \
 control_console.o default_program.o error_reporting.o file_io_vm.o \
 human_readable_program_format.o lexical_analyzer.o main.o  opcode.o parser.o \
 safe_string_functions.o text_editor_console.o virtual_machine.o \
)

SOURCES := arg_flags.c common_console_functions.c \
 control_console.c default_program.c error_reporting.c file_io_vm.c \
 human_readable_program_format.c lexical_analyzer.c main.c  opcode.c parser.c \
 safe_string_functions.c text_editor_console.c virtual_machine.c

INCLUDE_DIRS := -I$(VM_SRC_DIR)
DEFINES := 
C_STANDARD := -std=c99
OPTIMIZATION := -g
WARNINGS := -Wall
CC := gcc
CCFLAGS := $(C_STANDARD) $(DEFINES) $(INCLUDE_DIRS) $(OPTIMIZATION) $(WARNINGS) -c

define Compile_Source =
@echo 'Generating object file for: $<'
$(CC) $(CCFLAGS) -o "$@" "$<"
@echo 'Finished building: $@'
@echo ' '
endef

# All Target
all: $(LINK_EXECUTABLE)

$(OBJ_DIR)/%.o: %.c
	$(Compile_Source)

$(OBJ_DIR)/error_reporting.o: error_reporting.c ERH_error_reporting.h

$(OBJ_DIR)/opcode.o: opcode.c OPC_opcode.h ERH_error_reporting.h vmwitheditor.h

$(OBJ_DIR)/parser.o: parser.c ERH_error_reporting.h  parser.h vmwitheditor.h \
HRF_human_readable_program_format.h LAH_lexical_analyzer.h SSF_safe_string_functions.h \
 VMH_virtual_machine.h

$(OBJ_DIR)/human_readable_program_format.o: human_readable_program_format.c \
 ERH_error_reporting.h HRF_human_readable_program_format.h \
 LAH_lexical_analyzer.h VMH_virtual_machine.h

$(OBJ_DIR)/safe_string_functions.o: safe_string_functions.c \
 vmwitheditor.h SSF_safe_string_functions.h ERH_error_reporting.h

$(OBJ_DIR)/lexical_analyzer.o: lexical_analyzer.c vmwitheditor.h \
 SSF_safe_string_functions.h \
 ERH_error_reporting.h LAH_lexical_analyzer.h

$(OBJ_DIR)/virtual_machine.o: virtual_machine.c  ERH_error_reporting.h \
 VMH_virtual_machine.h

$(OBJ_DIR)/arg_flags.o: arg_flags.c  ERH_error_reporting.h ARGF_arg_flags.h

$(OBJ_DIR)/common_console_functions.o: common_console_functions.c \
 ERH_error_reporting.h common_console_functions.h

:$(OBJ_DIR)/control_console.o: control_console.c ERH_error_reporting.h \
 common_console_functions.h control_console.h file_io_vm.h VMH_virtual_machine.h

$(OBJ_DIR)/default_program.o: default_program.c default_program.h \
 ERH_error_reporting.h HRF_human_readable_program_format.h

$(OBJ_DIR)/main.o: main.c ERH_error_reporting.h ARGF_arg_flags.h control_console.h \
 default_program.h VMH_virtual_machine.h

$(OBJ_DIR)/text_editor_console.o: text_editor_console.c ERH_error_reporting.h \
 VMH_virtual_machine.h common_console_functions.h text_editor_console.h

%.h:

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
