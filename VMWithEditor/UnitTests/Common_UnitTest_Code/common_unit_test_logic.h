#ifndef COMMON_UNIT_TEST_LOGIC_H
#define COMMON_UNIT_TEST_LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#ifndef REDUCED_VM_AND_HRF_DEPENDENCIES
#include "human_readable_program_format.h"
#endif

typedef struct test_log_data
{
	char* function_name;
	bool status;
	char* path;
	bool stand_alone;
} Test_Log_Data;

extern FILE* error_out_file;
extern FILE* unit_test_log_file;

extern bool init_vm_error_reporting(char* error_log_file_name);
#ifndef REDUCED_VM_AND_HRF_DEPENDENCIES
extern Human_Readable_Program_Format* default_program(size_t* program_size);
#endif
extern void disengage_error_reporting(void);
extern bool init_unit_tests(char* log_file_name);
extern void report_error_generic(char* error_message);
extern void report_create_and_init_test_log_data_memory_failure(char* function_name);
extern void log_test_status_each_step(char* function_name, bool status, char* path, bool stand_alone);
extern void init_test_log_data(Test_Log_Data* log_data, char* function_name, bool status, char* path, bool stand_alone);
extern Test_Log_Data* create_and_init_test_log_data(char* function_name, bool status, char* path, bool stand_alone);
extern void log_test_status_each_step2(Test_Log_Data* test_data_to_log);
extern void log_start_positive_path(char* function_name);
extern void log_start_positive_path2(Test_Log_Data* log_data);
extern void log_start_test_path(Test_Log_Data* log_data);
extern void log_end_test_path(Test_Log_Data* log_data);
extern void log_end_positive_path(char* function_name);
extern void log_end_positive_path2(Test_Log_Data* log_data);
extern void log_start_negative_path(char* function_name);
extern void log_end_negative_path(char* function_name);
extern void log_generic_message(char *log_message);
extern void close_unit_tests(void);

#endif // !COMMON_UNIT_TEST_LOGIC_H

