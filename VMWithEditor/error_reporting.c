#ifndef ERROR_REPORTING_C
#define ERROR_REPORTING_C

#include "error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif	// UNIT_TESTING
#include <stdio.h>

FILE* error_out_file = NULL;

bool init_vm_error_reporting(const char* error_log_file_name)
{
	bool status_is_good = true;

	if (error_log_file_name)
	{
		error_out_file = fopen(error_log_file_name, "w");
		if (!error_out_file)
		{
#ifdef UNIT_TESTING
			error_out_file = stderr;
#endif	// UNIT_TESTING
			fprintf(error_out_file, "Can't open error output file, %s", "error_log_file_name");
			status_is_good = false;
		}
	}
	else
	{
		error_out_file = stderr;
	}

	return status_is_good;
}

void disengage_error_reporting(void)
{
	if (error_out_file != stderr)
	{
		fclose(error_out_file);
	}
}

void report_error_generic(const char *error_message)
{
	fprintf(error_out_file, "%s\n", error_message);
}

#endif	// !ERROR_REPORTING_C
