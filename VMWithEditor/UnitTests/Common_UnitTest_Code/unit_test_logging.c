#ifndef UNIT_TEST_LOGGING_C
#define UNIT_TEST_LOGGING_C

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe_string_functions.h"
#include "unit_test_logging.h"
#include "error_reporting.h"

FILE* UTL_unit_test_log_file = NULL;

bool UTL_init_unit_tests(const char* log_file_name)
{
	if (log_file_name)
	{
		UTL_unit_test_log_file = fopen(log_file_name, "w");
		if (!UTL_unit_test_log_file)
		{
			fprintf(ERH_error_out_file, "Can't open %s for output\n", log_file_name);
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
	if (UTL_unit_test_log_file != stdout)
	{
		fclose(UTL_unit_test_log_file);
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

void UTL_log_test_status_each_step2(const UTL_Test_Log_Data *log_data)
{
	if (log_data->stand_alone)
	{
		fprintf(UTL_unit_test_log_file, "%s(): %s Path %s\n", log_data->function_name,
			log_data->path == UTL_POSITIVE_PATH ? "Positive" : "Negative",
			(log_data->status) ? "Passed" : "Failed");
	}
}

void UTL_log_start_positive_path(const char* function_name)
{
	fprintf(UTL_unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
		function_name);
}

void UTL_log_start_positive_path2(const UTL_Test_Log_Data *log_data)
{
	fprintf(UTL_unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
		log_data->function_name);
}

void UTL_log_end_positive_path(const char* function_name)
{
	fprintf(UTL_unit_test_log_file, "\nEnding POSITIVE PATH testing for %s\n", function_name);
}

void UTL_log_end_positive_path2(const UTL_Test_Log_Data* log_data)
{
	fprintf(UTL_unit_test_log_file, "\nEnding POSITIVE PATH testing for %s, POSITIVE PATH  %s \n",
		log_data->function_name, log_data->status? "PASSED" : "FAILED");
}

void UTL_log_start_negative_path(const char* function_name)
{
	fprintf(UTL_unit_test_log_file, "\nStarting NEGATIVE PATH testing for %s\n\n", function_name);
}

void UTL_log_end_negative_path(const char* function_name)
{
	fprintf(UTL_unit_test_log_file, "\nEnding NEGATIVE PATH testing for %s\n", function_name);
	fflush(UTL_unit_test_log_file);		// Current unit test is done flush the output.
}

void UTL_log_start_test_path(const UTL_Test_Log_Data* log_data)
{
	UTL_va_log_fprintf("\nStarting %s PATH testing for %s\n\n",
		log_data->path == UTL_POSITIVE_PATH ? "POSITIVE" : "NEGATIVE",
		log_data->function_name);
}

void UTL_log_end_test_path(const UTL_Test_Log_Data *log_data)
{
	UTL_va_log_fprintf("\nEnding %s PATH testing for %s, Path %s\n",
		log_data->path == UTL_POSITIVE_PATH ? "POSITIVE" : "NEGATIVE",
		log_data->function_name, log_data->status ? "PASSED" : "FAILED");

	if (log_data->path == UTL_NEGATIVE_PATH)
	{
		fflush(UTL_unit_test_log_file);		// Current unit test is done flush the output.
	}
}

void UTL_init_test_log_data(UTL_Test_Log_Data* log_data, const char *function_name, const bool status, const UTL_Path_State path, bool stand_alone)
{
	log_data->function_name = function_name;
	log_data->status = status;
	log_data->path = path;
	log_data->stand_alone = stand_alone;
}

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

UTL_Test_Log_Data *UTL_create_and_init_test_log_data(const char* function_name, const bool status, const UTL_Path_State path, const bool stand_alone)
{
	UTL_Test_Log_Data* log_data = calloc(1, sizeof(*log_data));
	if (log_data)
	{
		UTL_init_test_log_data(log_data, function_name, status, path, stand_alone);
	}
	else
	{
		fprintf(ERH_error_out_file, "In %s calloc() failed\n", "create_and_init_test_log_data");
	}

	return log_data;
}

// provides common error report for memory allocation error.
void UTL_report_create_and_init_test_log_data_memory_failure(const char *function_name)
{
	fprintf(ERH_error_out_file, "In function %s, Memory allocation failed in create_and_init_test_log_data\n", function_name);
}

#endif	// !UNIT_TEST_LOGGING_C
