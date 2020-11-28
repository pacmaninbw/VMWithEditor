/*
 * UTL_unit_test_logging.h
 * 
 * Common logging functions for all unit tests.
 * All symbolic constants, enums, and functions provided by this module shall
 * start with UTL.
 */
#ifndef UTL_UNIT_TEST_LOGGING_H
#define UTL_UNIT_TEST_LOGGING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define UTL_LOG_BUFFER_SIZE 2048
#define UTL_FWRITE_BUFFER_SIZE 16384

/*
 * Use these enums to indicate if the current testing includes errors or not,
 * tests containing errors should use the UTL_NEGATIVE_PATH.
 */
typedef enum utl_path_state
{
	UTL_POSITIVE_PATH,
	UTL_NEGATIVE_PATH
} UTL_Path_State;

/*
 * This structure should be created for every unit test function for logging purposes
 */
typedef struct test_log_data
{
	const char* function_name;
	UTL_Path_State path;
	size_t indent_count;
	bool status;
	bool stand_alone;
	bool internal_test;
	bool use_indent;
} UTL_Test_Log_Data;

/*
 * Unit test reporting functions.
 */
void UTL_log_test_status_each_step(const UTL_Test_Log_Data* test_data_to_log);
void UTL_log_start_unit_test(UTL_Test_Log_Data* log_data, const char *addendum);
void UTL_log_end_unit_test(UTL_Test_Log_Data* log_data, const char* addendum);
void UTL_log_start_test_path(UTL_Test_Log_Data* log_data, const char* addendum);
void UTL_log_end_test_path(UTL_Test_Log_Data* log_data, const char* addendum);
void UTL_log_high_level_test_result(const UTL_Test_Log_Data* log_data,
	const size_t test_id);
void UTL_log_high_level_start_test(const UTL_Test_Log_Data* log_data,
	const size_t test_id);
void UTL_log_test_not_implemented(UTL_Test_Log_Data* log_data);
/*
 * Only use the following function if there are special needs for the test, if
 * the code is repeated many times add an additional reporting function above.
 */
void UTL_va_test_log_formatted_output(const UTL_Test_Log_Data* log_data, bool print_header,
	const char* format, ...);

void UTL_report_create_and_init_test_log_data_memory_failure(const char* function_name);
UTL_Test_Log_Data* UTL_new_log_data(const char* function_name, const bool status,
	const UTL_Path_State path, const bool stand_alone, const bool internal_test);

/*
 * Unit test activation and deactivations called from unit test main functions.
 * These functions should only be called once per unit test program execution.
 */
bool UTL_init_unit_tests(const char* log_file_name);
void UTL_close_unit_tests(void);

#endif /* !UTL_UNIT_TEST_LOGGING_H */

