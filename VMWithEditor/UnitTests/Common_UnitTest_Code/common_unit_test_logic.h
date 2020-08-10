#ifndef COMMON_UNIT_TEST_LOGIC_H
#define COMMON_UNIT_TEST_LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include "human_readable_program_format.h"

extern FILE* error_out_file;
extern FILE* unit_test_log_file;

extern bool init_vm_error_reporting(char* error_log_file_name);
extern Human_Readable_Program_Format* default_program(size_t* program_size);
extern void disengage_error_reporting(void);
extern bool init_unit_tests(char* log_file_name);
extern void close_unit_tests(void);

#endif // !COMMON_UNIT_TEST_LOGIC_H

