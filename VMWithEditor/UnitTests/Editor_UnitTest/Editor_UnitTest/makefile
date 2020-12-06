
RM := rm -rf

# All paths are relative to the current directory to allow this to build on any
# system. This assumes that the directory structure in GitHub is maintained.
OBJ_DIR := ./obj
BIN_DIR := ./bin
VS_BUILD_DIRS := ./Debug ./Release # For make clean only
VM_SRC_DIR := ../../..
LOCAL_TEST_DIR := ./
COMMON_TEST_DIR := ../../Common_UnitTest_Code

DIRS := $(OBJ_DIR) $(BIN_DIR)

# The executable changes for each directory, the 2 following variables are
# standard in all the make files
EXECUTABLE := Editor_Unit_Test.exe
BINARY_EXECUTABLE := $(BIN_DIR)/$(EXECUTABLE)
LINK_EXECUTABLE := ./$(EXECUTABLE)

OBJS := $(addprefix $(OBJ_DIR)/, Editor_UnitTest_Main.o error_reporting.o \
 unit_test_logging.o arg_flags.o safe_string_functions.o \
)

VM_DIR_SOURCES := $(addprefix $(VM_SRC_DIR)/, error_reporting.c  \
 safe_string_functions.c  arg_flags.c)

LOCAL_TEST_SOURCES := (addprefix $(LOCAL_TEST_DIR), Editor_UnitTest_Main.c)

COMMON_TEST_SOURCES := $(addprefix $(COMMON_TEST_DIR), unit_test_logging.c)

SOURCES := $(VM_TEST_SOURCES) $(COMMON_TEST_SOURCES) $(LOCAL_TEST_SOURCES)

# define the flags used by the compiler
INCLUDE_DIRS := -I$(LOCAL_TEST_DIR) -I$(COMMON_TEST_DIR) -I$(VM_SRC_DIR)
DEFINES := -DUNIT_TESTING -DEDITOR_UNIT_TEST_ONLY
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

$(VM_SRC_DIR)/%.h:
$(VM_TEST_DIR)/%.h:
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
