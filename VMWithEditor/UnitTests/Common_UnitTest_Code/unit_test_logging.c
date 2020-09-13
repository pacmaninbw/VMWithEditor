#ifndef UNIT_TEST_LOGGING_C
#define UNIT_TEST_LOGGING_C

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SSF_safe_string_functions.h"
#include "UTL_unit_test_logging.h"
#include "ERH_error_reporting.h"

static FILE* UTL_unit_test_log_file = NULL;

void UTL_va_log_fprintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(UTL_unit_test_log_file, format, args);
	va_end(args);

	fflush(ERH_error_out_file);
}

void UTL_va_test_log_fprintf(const UTL_Test_Log_Data* log_data, const char* format, ...)
{
	if (log_data->stand_alone)
	{
		fprintf(UTL_unit_test_log_file, "Unit test function %s: %s path: ",
			log_data->function_name, log_data->path == UTL_POSITIVE_PATH ?
			"POSITIVE" : "NEGATIVE");
		va_list args;
		va_start(args, format);
		vfprintf(UTL_unit_test_log_file, format, args);
		va_end(args);
	}
}

void UTL_log_start_unit_test(const UTL_Test_Log_Data* log_data)
{
	fprintf(UTL_unit_test_log_file, "STARTING unit test for %s\n\n", log_data->function_name);
}

void UTL_log_end_unit_test(const UTL_Test_Log_Data* log_data)
{
	fprintf(UTL_unit_test_log_file, "\nENDING unit test for %s: test %s\n\n", log_data->function_name, log_data->status ? "PASSED" : "FAILED");
	fflush(UTL_unit_test_log_file);		// Current unit test is done flush the output.
}

void UTL_log_test_status_each_step(const UTL_Test_Log_Data *log_data)
{
	if (log_data->stand_alone)
	{
		fprintf(UTL_unit_test_log_file, "%s(): %s Path %s\n", log_data->function_name,
			log_data->path == UTL_POSITIVE_PATH ? "Positive" : "Negative",
			(log_data->status) ? "Passed" : "Failed");
	}
}

void UTL_log_start_test_path(const UTL_Test_Log_Data* log_data)
{
	if (log_data->stand_alone)
	{
		fprintf(UTL_unit_test_log_file, "\nStarting %s PATH testing for %s\n\n",
			log_data->path == UTL_POSITIVE_PATH ? "POSITIVE" : "NEGATIVE",
			log_data->function_name);
	}
}

void UTL_log_end_test_path(const UTL_Test_Log_Data *log_data)
{
	char* path = log_data->path == UTL_POSITIVE_PATH ? "POSITIVE" : "NEGATIVE";

	if (log_data->stand_alone)
	{
		fprintf(UTL_unit_test_log_file, "\nEnding %s PATH testing for %s, %s PATH  %s\n\n",
			path, log_data->function_name, path, log_data->status ? "PASSED" : "FAILED");
	}

	if (log_data->path == UTL_NEGATIVE_PATH)
	{
		fflush(UTL_unit_test_log_file);		// Current unit test is done flush the output.
	}
}

// provides common error report for memory allocation error.
void UTL_report_create_and_init_test_log_data_memory_failure(const char *function_name)
{
	char buffer[ERH_ERROR_BUFFER_SIZE];
	snprintf(buffer, sizeof(buffer), "In function %s, Memory allocation failed in create_and_init_test_log_data\n", function_name);
	if (errno)
	{
		perror(buffer);
	}
	else
	{
		fprintf(ERH_error_out_file, "%s\n", buffer);
	}
}

bool UTL_init_unit_tests(const char* log_file_name)
{
	if (!ERH_error_reporting_is_initialized())
	{
		ERH_va_report_error_fprintf("Error reporting has not been initialized\n");
		return false;
	}

	if (log_file_name)
	{
		UTL_unit_test_log_file = fopen(log_file_name, "w");
		if (!UTL_unit_test_log_file)
		{
			ERH_report_error_output_fopen_failed(log_file_name);
			return false;
		}
		ERH_error_out_file = UTL_unit_test_log_file;
	}
	else
	{
		UTL_unit_test_log_file = stdout;
		ERH_error_out_file = stderr;
	}

	return true;
}

void UTL_close_unit_tests(void)
{
	if (UTL_unit_test_log_file != stdout && UTL_unit_test_log_file != stderr)
	{
		fclose(UTL_unit_test_log_file);
	}
}

static void UTL_init_test_log_data(UTL_Test_Log_Data* log_data, const char* function_name,
	const bool status, const UTL_Path_State path, bool stand_alone)
{
	log_data->function_name = function_name;
	log_data->status = status;
	log_data->path = path;
	log_data->stand_alone = stand_alone;
}

UTL_Test_Log_Data* UTL_create_and_init_test_log_data(const char* function_name, const bool status, const UTL_Path_State path, const bool stand_alone)
{
	UTL_Test_Log_Data* log_data = calloc(1, sizeof(*log_data));
	if (log_data)
	{
		UTL_init_test_log_data(log_data, function_name, status, path, stand_alone);
	}
	else
	{
		char buffer[ERH_ERROR_BUFFER_SIZE];
		snprintf(buffer, sizeof(buffer), "In %s calloc() failed", "create_and_init_test_log_data");
		if (errno)
		{
			perror(buffer);
		}
		else
		{
			fprintf(ERH_error_out_file, "%s\n", buffer);
		}
	}

	return log_data;
}

#endif	// !UNIT_TEST_LOGGING_C
