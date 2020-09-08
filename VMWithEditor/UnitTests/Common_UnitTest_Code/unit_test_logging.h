/*
 * unit_test_logging.h
 * 
 * Common logging functions for all unit tests.
 * All symbolic constants, enums, and functions provided by this module shall
 * start with UTL.
 */
#ifndef UNIT_TEST_LOGGING_H
#define UNIT_TEST_LOGGING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define UTL_LOG_BUFFER_SIZE 2048

typedef enum utl_path_state
{
	UTL_POSITIVE_PATH,
	UTL_NEGATIVE_PATH
} UTL_Path_State;

typedef struct test_log_data
{
	const char* function_name;
	UTL_Path_State path;
	bool status;
	bool stand_alone;
} UTL_Test_Log_Data;

extern FILE* UTL_unit_test_log_file;

bool UTL_init_unit_tests(const char* log_file_name);
void UTL_report_create_and_init_test_log_data_memory_failure(const char* function_name);
void UTL_init_test_log_data(UTL_Test_Log_Data* log_data, const char* function_name,
	const bool status, const UTL_Path_State path, const bool stand_alone);
UTL_Test_Log_Data* UTL_create_and_init_test_log_data(const char* function_name,
	const bool status, const UTL_Path_State path, const bool stand_alone);
void UTL_log_test_status_each_step2(const UTL_Test_Log_Data* test_data_to_log);
void UTL_log_start_unit_test(const UTL_Test_Log_Data* log_data);
void UTL_log_start_positive_path(const char* function_name);
void UTL_log_start_positive_path2(const UTL_Test_Log_Data* log_data);
void UTL_log_start_test_path(const UTL_Test_Log_Data* log_data);
void UTL_log_end_unit_test(const UTL_Test_Log_Data* log_data);
void UTL_log_end_test_path(const UTL_Test_Log_Data* log_data);
void UTL_log_end_positive_path(const char* function_name);
void UTL_log_end_positive_path2(const UTL_Test_Log_Data* log_data);
void UTL_log_start_negative_path(const char* function_name);
void UTL_log_end_negative_path(const char* function_name);
void UTL_va_log_fprintf(const char* format, ...);
void UTL_va_test_log_fprintf(const UTL_Test_Log_Data* log_data, const char* format, ...);
void UTL_close_unit_tests(void);

#endif // !UNIT_TEST_LOGGING_H

