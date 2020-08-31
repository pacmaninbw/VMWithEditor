#ifndef COMMON_UNIT_TEST_LOGIC_H
#define COMMON_UNIT_TEST_LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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

bool init_vm_error_reporting(char* error_log_file_name);
#ifndef REDUCED_VM_AND_HRF_DEPENDENCIES
Human_Readable_Program_Format* default_program(size_t* program_size);
#endif

#ifndef strdup
#ifdef _MSC_VER
#if _MSC_VER > 1920
#define strdup _strdup
#endif
#else
#define strdup mystrdup      
#endif
#endif

char* mystrdup(const char* string_to_copy);
unsigned char* ucstrdup(const unsigned char* string_to_copy);
void disengage_error_reporting(void);
bool init_unit_tests(char* log_file_name);
void report_error_generic(char* error_message);
void report_create_and_init_test_log_data_memory_failure(char* function_name);
void log_test_status_each_step(char* function_name, bool status, char* path, bool stand_alone);
void init_test_log_data(Test_Log_Data* log_data, char* function_name, bool status, char* path, bool stand_alone);
Test_Log_Data* create_and_init_test_log_data(char* function_name, bool status, char* path, bool stand_alone);
void log_test_status_each_step2(Test_Log_Data* test_data_to_log);
void log_start_positive_path(char* function_name);
void log_start_positive_path2(Test_Log_Data* log_data);
void log_start_test_path(Test_Log_Data* log_data);
void log_end_test_path(Test_Log_Data* log_data);
void log_end_positive_path(char* function_name);
void log_end_positive_path2(Test_Log_Data* log_data);
void log_start_negative_path(char* function_name);
void log_end_negative_path(char* function_name);
void log_generic_message(char *log_message);
void close_unit_tests(void);

#endif // !COMMON_UNIT_TEST_LOGIC_H

