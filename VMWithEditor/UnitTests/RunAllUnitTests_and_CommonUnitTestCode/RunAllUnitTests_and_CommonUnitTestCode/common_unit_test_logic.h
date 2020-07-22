#ifndef COMMON_UNIT_TEST_LOGIC_H
#define COMMON_UNIT_TEST_LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include "human_readable_program_format.h"

extern FILE* error_out_file;

extern bool init_vm_error_reporting(char* error_log_file_name);
extern Human_Readable_Program_Format* default_program(size_t* program_size);
extern void disengage_error_reporting(void);

#endif // !COMMON_UNIT_TEST_LOGIC_H

