#ifndef UNIT_TEST_LOGGING_H
#define UNIT_TEST_LOGGING_H
#include <stdio.h>
#include <stdbool.h>

typedef struct test_log_data
{
	const char* function_name;
	char* path;
	bool status;
	bool stand_alone;
} Test_Log_Data;

extern FILE* unit_test_log_file;

bool init_unit_tests(const char* log_file_name);
void report_create_and_init_test_log_data_memory_failure(const char* function_name);
void log_test_status_each_step(const char* function_name, const bool status, const char* path, const bool stand_alone);
void init_test_log_data(Test_Log_Data* log_data, const char* function_name, const bool status, char* path, const bool stand_alone);
Test_Log_Data* create_and_init_test_log_data(const char* function_name, const bool status, char* path, const bool stand_alone);
void log_test_status_each_step2(const Test_Log_Data* test_data_to_log);
void log_start_positive_path(const char* function_name);
void log_start_positive_path2(const Test_Log_Data* log_data);
void log_start_test_path(const Test_Log_Data* log_data);
void log_end_test_path(const Test_Log_Data* log_data);
void log_end_positive_path(const char* function_name);
void log_end_positive_path2(const Test_Log_Data* log_data);
void log_start_negative_path(const char* function_name);
void log_end_negative_path(const char* function_name);
void log_generic_message(const char *log_message);
void close_unit_tests(void);

#endif // !UNIT_TEST_LOGGING_H

