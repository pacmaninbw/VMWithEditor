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
static char* UTL_fwrite_buffer = NULL;

#define MAX_INDENT_BUFFER 64
#define INDENT_TAB_SIZE 4
#define WRITE_BUFFER_SIZE 4096

typedef enum UTL_Start_End_Item_Type
{
	UTL_UNIT_TEST,
	UTL_TEST_PATH
} UTL_Start_End_Item_Type;

static void indent_text_line(UTL_Test_Log_Data *log_data, char *out_buffer,
	size_t out_buffer_size)
{
	char indent_buffer[MAX_INDENT_BUFFER] = { 0 };
	if (log_data->use_indent && log_data->indent_count)
	{
		size_t indent_count = log_data->indent_count;
		if ((INDENT_TAB_SIZE * indent_count + 1) < MAX_INDENT_BUFFER)
		{
			memset(indent_buffer, ' ', INDENT_TAB_SIZE * indent_count);
			indent_buffer[INDENT_TAB_SIZE * indent_count] = (char)0;
		}
		else
		{
			memset(indent_buffer, ' ', MAX_INDENT_BUFFER);
			indent_buffer[MAX_INDENT_BUFFER - 1] = (char)0;
		}
	}

	SSF_strcat(out_buffer, indent_buffer, out_buffer_size);
}

static const char* get_path_string(const UTL_Test_Log_Data* log_data, const bool all_capitals)
{
	const char* path_string = NULL;

	if (all_capitals)
	{
		path_string = (log_data->path == UTL_POSITIVE_PATH) ? "POSITIVE" : "NEGATIVE";
	}
	else
	{
		path_string = (log_data->path == UTL_POSITIVE_PATH) ? "Positive" : "Negative";
	}

	return path_string;
}

static const char* get_status_string(const UTL_Test_Log_Data* log_data, const bool all_capitals)
{
	const char* status_string = NULL;

	if (all_capitals)
	{
		status_string = (log_data->status) ? "PASSED" : "FAILED";
	}
	else
	{
		status_string = (log_data->status) ? "Passed" : "Failed";
	}

	return status_string;
}

static void write_indented_line_to_log_file(UTL_Test_Log_Data* log_data,
	char *out_buffer, size_t prepend_newline_count, size_t append_newline_count)
{
	char write_buffer[WRITE_BUFFER_SIZE] = { 0 };

	for (size_t i = 0; i < prepend_newline_count; i++)
	{
		SSF_strcat(write_buffer, "\n", WRITE_BUFFER_SIZE);
	}
	indent_text_line(log_data, write_buffer, sizeof(write_buffer));

	if (strlen(write_buffer) + strlen(out_buffer) + 1 < WRITE_BUFFER_SIZE)
	{
		SSF_strcat(write_buffer, out_buffer, sizeof(write_buffer));

		for (size_t i = 0; i < append_newline_count; i++)
		{
			SSF_strcat(write_buffer, "\n", WRITE_BUFFER_SIZE);
		}

		fwrite(write_buffer, sizeof(*write_buffer), strlen(write_buffer),
			UTL_unit_test_log_file);
	}
	else
	{
		fwrite(out_buffer, sizeof(*out_buffer), strlen(out_buffer),
			UTL_unit_test_log_file);
	}
}

/*
 * No indentation necessary. This will alway be out put.
 */
void UTL_log_high_level_test_result(const UTL_Test_Log_Data* log_data,
	const size_t test_id)
{
	fprintf(UTL_unit_test_log_file,"\nUnit Test %zd: %s : %s\n\n", test_id,
		log_data->function_name, get_status_string(log_data, false));
}

void UTL_log_high_level_start_test(const UTL_Test_Log_Data* log_data,
	const size_t test_id)
{
	fprintf(UTL_unit_test_log_file, "Unit Test %zd: Starting %s\n\n", test_id,
		log_data->function_name);
}

void UTL_log_test_not_implemented(UTL_Test_Log_Data* log_data)
{
	char out_buffer[WRITE_BUFFER_SIZE] = { 0 };
	snprintf(out_buffer, sizeof(out_buffer), "Unit test: %s NOT IMPLEMENTED\n",
		log_data->function_name);
	size_t prepend_newline_count = 1;
	size_t append_newline_count = 0;

	write_indented_line_to_log_file(log_data, out_buffer, prepend_newline_count,
		append_newline_count);

	log_data->status = false;
}

static void generic_start_test_log_item(UTL_Test_Log_Data* log_data,
	const char* additional_data, UTL_Start_End_Item_Type test_type)
{
	char out_buffer[WRITE_BUFFER_SIZE] = { 0 };
	const char* addendum = (additional_data) ? additional_data : "";
	size_t prepend_newline_count = 0;
	size_t append_newline_count = 0;

	if (test_type == UTL_UNIT_TEST)
	{
		snprintf(out_buffer, sizeof(out_buffer), "STARTING%sunit test for %s %s\n\n",
			log_data->internal_test ? " internal " : " ",
			log_data->function_name, addendum);
	}
	else
	{
		snprintf(out_buffer, sizeof(out_buffer), "Starting %s PATH testing for %s %s\n\n",
			get_path_string(log_data, true), log_data->function_name, addendum);
		prepend_newline_count = 1;
	}

	write_indented_line_to_log_file(log_data, out_buffer, prepend_newline_count,
		append_newline_count);

	if (log_data->use_indent)
	{
		log_data->indent_count++;
	}
}

void UTL_log_start_unit_test(UTL_Test_Log_Data* log_data,
	const char *additional_data)
{
	if (log_data->internal_test && !log_data->stand_alone)
	{
		return;
	}

	generic_start_test_log_item(log_data, additional_data, UTL_UNIT_TEST);
}

void UTL_log_start_test_path(UTL_Test_Log_Data* log_data,
	const char* additional_data)
{
	// A test path is internal by definition
	if (!log_data->stand_alone)
	{
		return;
	}

	generic_start_test_log_item(log_data, additional_data, UTL_TEST_PATH);
}

static void generic_end_test_log_item(UTL_Test_Log_Data* log_data,
	const char* additional_data, UTL_Start_End_Item_Type test_type)
{
	if (log_data->use_indent && log_data->indent_count)
	{
		log_data->indent_count--;
	}

	char out_buffer[WRITE_BUFFER_SIZE] = { 0 };
	const char* addendum = (additional_data) ? additional_data : "";
	size_t prepend_newline_count = 1;
	size_t append_newline_count = 0;

	if (test_type == UTL_UNIT_TEST)
	{
		snprintf(out_buffer, sizeof(out_buffer),
			"ENDING unit test for %s: test %s %s\n\n",
			log_data->function_name, get_status_string(log_data, true),
			addendum);
	}
	else
	{
		const char* path = get_path_string(log_data, true);
		snprintf(out_buffer, sizeof(out_buffer),
			"Ending %s PATH testing for %s, %s PATH  %s %s\n\n",
			path, log_data->function_name, path,
			get_status_string(log_data, true), addendum);
	}

	write_indented_line_to_log_file(log_data, out_buffer, prepend_newline_count,
		append_newline_count);
}

void UTL_log_end_test_path(UTL_Test_Log_Data *log_data,
	const char* additional_data)
{
	if (!log_data->stand_alone)
	{
		return;
	}

	generic_end_test_log_item(log_data, additional_data, UTL_TEST_PATH);
}

void UTL_log_end_unit_test(UTL_Test_Log_Data* log_data,
	const char* additional_data)
{
	if (log_data->internal_test && !log_data->stand_alone)
	{
		return;
	}

	generic_end_test_log_item(log_data, additional_data, UTL_UNIT_TEST);

	fflush(UTL_unit_test_log_file);		// Current unit test is done flush the output.
}

void UTL_log_test_status_each_step(const UTL_Test_Log_Data* log_data)
{
	if (!log_data->stand_alone)
	{
		return;
	}

	char out_buffer[WRITE_BUFFER_SIZE];
	size_t prepend_newline_count = 0;
	size_t append_newline_count = 0;

	snprintf(out_buffer, sizeof(out_buffer), "%s(): %s Path %s\n", log_data->function_name,
		get_path_string(log_data, false), get_status_string(log_data, false));

	write_indented_line_to_log_file((UTL_Test_Log_Data*)log_data, out_buffer, 
		prepend_newline_count, append_newline_count);
}

// provides common error report for memory allocation error.
void UTL_report_create_and_init_test_log_data_memory_failure(const char *function_name)
{
	char buffer[ERH_ERROR_BUFFER_SIZE];
	snprintf(buffer, sizeof(buffer),
		"In function %s, Memory allocation failed in create_and_init_test_log_data\n",
		function_name);
	ERH_use_perror_when_errno(buffer);
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
		else
		{
			UTL_fwrite_buffer = malloc(UTL_FWRITE_BUFFER_SIZE);
			if (!UTL_fwrite_buffer)
			{
				char *malloc_error =
					"In UTL_init_unit_tests malloc() of UTL_fwrite_buffer failed";
				ERH_use_perror_when_errno(malloc_error);
				return false;
			}
			if (setvbuf(UTL_unit_test_log_file, NULL, _IOFBF, UTL_FWRITE_BUFFER_SIZE)) {
				char* setvbuf_error = "In UTL_init_unit_tests setvbuf() of UTL_fwrite_buffer failed";
				ERH_use_perror_when_errno(setvbuf_error);
				return false;
			}
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
		free(UTL_fwrite_buffer);
	}
}

UTL_Test_Log_Data* UTL_new_log_data(const char* function_name,
	const bool status, const UTL_Path_State path, const bool stand_alone,
	const bool internal_test)
{
	UTL_Test_Log_Data* log_data = calloc(1, sizeof(*log_data));
	if (log_data)
	{
		log_data->function_name = function_name;
		log_data->status = status;
		log_data->path = path;
		log_data->stand_alone = stand_alone;
		log_data->internal_test = internal_test;
		log_data->use_indent = true;
	}
	else
	{
		char buffer[ERH_ERROR_BUFFER_SIZE];
		snprintf(buffer, sizeof(buffer), "In %s calloc() failed", "create_and_init_test_log_data");
		ERH_use_perror_when_errno(buffer);
	}

	return log_data;
}

void UTL_va_test_log_formatted_output(const UTL_Test_Log_Data* log_data, bool print_header, const char* format, ...)
{
	bool stand_alone = true;
	if (log_data)
	{
		stand_alone = log_data->stand_alone;
		if (print_header && stand_alone) {
			fprintf(UTL_unit_test_log_file, "Unit test function %s: %s path: ",
				log_data->function_name, get_path_string(log_data, true));
		}
	}

	if (stand_alone)
	{
		va_list args;
		va_start(args, format);
		vfprintf(UTL_unit_test_log_file, format, args);
		va_end(args);
	}
}

#endif	// !UNIT_TEST_LOGGING_C
