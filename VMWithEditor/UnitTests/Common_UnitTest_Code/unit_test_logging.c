#include "my_strdup.h"
#include "unit_test_logging.h"
#include "error_reporting.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* unit_test_log_file = NULL;


bool init_unit_tests(const char* log_file_name)
{
	if (log_file_name)
	{
		unit_test_log_file = fopen(log_file_name, "w");
		if (!unit_test_log_file)
		{
			fprintf(error_out_file, "Can't open %s for output\n", log_file_name);
			return false;
		}
		error_out_file = unit_test_log_file;
	}
	else
	{
		unit_test_log_file = stdout;
		error_out_file = stderr;
	}

	return true;
}

void close_unit_tests(void)
{
	if (unit_test_log_file != stdout)
	{
		fclose(unit_test_log_file);
	}
}

static bool log_test_is_positive_path(const Test_Log_Data* log_data)
{
	bool is_positive = true;

	if (!log_data->path)
	{
		fprintf(error_out_file, "Programmer error: log_data->path is NULL in log_test_is_positive_path()\n");
		return false;
	}

	char* string_to_test = my_strdup(log_data->path);
	if (!string_to_test)
	{
		fprintf(error_out_file, "Memory Allocation error: strdup() failed in log_test_is_positive_path()\n");
		fprintf(error_out_file, "Exiting program.\n");
		exit(EXIT_FAILURE);
	}

	char* stt_ptr = string_to_test;
	while (*stt_ptr)
	{
		*stt_ptr = (char) toupper(*stt_ptr);
		stt_ptr++;
	}

	is_positive = (strcmp(string_to_test, "POSITIVE") == 0);
	free(string_to_test);

	return is_positive;
}

void log_test_status_each_step(const char* function_name, const bool status, const char* path, const bool stand_alone)
{
	if (stand_alone)
	{
		fprintf(unit_test_log_file, "%s(): %s Path %s\n", function_name, path,
			(status) ? "Passed" : "Failed");
	}
}

void log_test_status_each_step2(const Test_Log_Data *test_data_to_log)
{
	if (test_data_to_log->stand_alone)
	{
		fprintf(unit_test_log_file, "%s(): %s Path %s\n", test_data_to_log->function_name,
			test_data_to_log->path, (test_data_to_log->status) ? "Passed" : "Failed");
	}
}

void log_start_positive_path(const char* function_name)
{
	fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
		function_name);
}

void log_start_positive_path2(const Test_Log_Data *log_data)
{
	fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
		log_data->function_name);
}

void log_end_positive_path(const char* function_name)
{
	fprintf(unit_test_log_file, "\nEnding POSITIVE PATH testing for %s\n", function_name);
}

void log_end_positive_path2(const Test_Log_Data* log_data)
{
	fprintf(unit_test_log_file, "\nEnding POSITIVE PATH testing for %s, POSITIVE PATH  %s \n",
		log_data->function_name, log_data->status? "PASSED" : "FAILED");
}

void log_start_negative_path(const char* function_name)
{
	fprintf(unit_test_log_file, "\nStarting NEGATIVE PATH testing for %s\n\n", function_name);
}

void log_end_negative_path(const char* function_name)
{
	fprintf(unit_test_log_file, "\nEnding NEGATIVE PATH testing for %s\n", function_name);
	fflush(unit_test_log_file);		// Current unit test is done flush the output.
}

void log_start_test_path(const Test_Log_Data* log_data)
{
	bool is_positive = log_test_is_positive_path(log_data);

	fprintf(unit_test_log_file, "\nStarting %s PATH testing for %s\n\n",
		is_positive ? "POSITIVE" : "NEGATIVE", log_data->function_name);
}

void log_end_test_path(const Test_Log_Data *log_data)
{
	bool is_positive = log_test_is_positive_path(log_data);

	fprintf(unit_test_log_file, "\nEnding %s PATH testing for %s, Path %s\n",
		is_positive ? "POSITIVE" : "NEGATIVE", log_data->function_name,
		log_data->status ? "PASSED" : "FAILED");

	if (!is_positive)
	{
		fflush(unit_test_log_file);		// Current unit test is done flush the output.
	}
}

void log_generic_message(const char* log_message)
{
	fprintf(unit_test_log_file, log_message);
}

void init_test_log_data(Test_Log_Data* log_data, const char *function_name, const bool status, char *path, bool stand_alone)
{
	log_data->function_name = function_name;
	log_data->status = status;
	log_data->path = path;
	log_data->stand_alone = stand_alone;
}

Test_Log_Data *create_and_init_test_log_data(const char* function_name, const bool status, char* path, const bool stand_alone)
{
	Test_Log_Data* log_data = calloc(1, sizeof(*log_data));
	if (log_data)
	{
		init_test_log_data(log_data, function_name, status, path, stand_alone);
	}
	else
	{
		fprintf(error_out_file, "In %s calloc() failed\n", "create_and_init_test_log_data");
	}

	return log_data;
}

// provides common error report for memory allocation error.
void report_create_and_init_test_log_data_memory_failure(const char *function_name)
{
	fprintf(error_out_file, "In function %s, Memory allocation failed in create_and_init_test_log_data\n", function_name);
}
