/*
 * error_reporting.c
 * 
 * The code in this file is responsible for reporting all error messages to the
 * proper place. The user has the option in the flags to redirect the error
 * messages to a file, the unit testing takes advantage of this to report errors
 * to the unit test log.
 */
#ifndef ERROR_REPORTING_C
#define ERROR_REPORTING_C

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "ERH_error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif	// UNIT_TESTING

FILE* ERH_error_out_file = NULL;

bool ERH_init_vm_error_reporting(const char* error_log_file_name)
{
	bool status_is_good = true;

	if (error_log_file_name)
	{
		ERH_error_out_file = fopen(error_log_file_name, "w");
		if (!ERH_error_out_file)
		{
#ifdef UNIT_TESTING
			// error_out_file is altered in unit_test_logging.c
			ERH_error_out_file = stderr;
#endif	// UNIT_TESTING
			ERH_report_error_output_fopen_failed(error_log_file_name);
			status_is_good = false;
		}
	}
	else
	{
		ERH_error_out_file = stderr;
	}

	return status_is_good;
}

void ERH_disengage_error_reporting(void)
{
	if (ERH_error_out_file != stderr)
	{
		fclose(ERH_error_out_file);
	}
}

void ERH_report_error_generic(const char *error_message)
{
	fprintf(ERH_error_out_file, "%s\n", error_message);
}

void ERH_report_error_output_fopen_failed(const char* file_name)
{
	char buffer[ERH_ERROR_BUFFER_SIZE];
	vsnprintf(buffer, ERH_ERROR_BUFFER_SIZE, "Can't open output file, %s", (char *)file_name);
	perror(buffer);
}

void ERH_report_error_input_fopen_failed(const char* file_name)
{
	char buffer[ERH_ERROR_BUFFER_SIZE];
	vsnprintf(buffer, ERH_ERROR_BUFFER_SIZE, "Can't open input file, %s", (char*)file_name);
	perror(buffer);
}

void ERH_va_report_error_fprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(ERH_error_out_file, format, args);
	va_end(args);

	fflush(ERH_error_out_file);
}

#endif	// !ERROR_REPORTING_C
