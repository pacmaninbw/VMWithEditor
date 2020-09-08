/*
 * control_console.c
 */

#ifndef CONTROL_CONSOLE_C
#define CONTROL_CONSOLE_C

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_console_functions.h"
#include "control_console.h"
#include "ERH_error_reporting.h"
#include "file_io_vm.h"
#ifdef UNIT_TESTING
#include "UTL_unit_test_logging.h"
#endif
#include "VMH_virtual_machine.h"

static void show_master_menu(void)
{

}

bool console_edit_and_run_program(HRF_Human_Readable_Program_Format* program)
{
	bool successful = true;

	return successful;
}

bool console_edit_and_run_program_file(const char* file_name)
{
	bool successful = true;

	return successful;
}

bool save_program_to_file(const HRF_Human_Readable_Program_Format* program, const size_t program_size, const char* file_name)
{
	bool successful = true;

	if (!program || program_size == 0)
	{
		successful = false;
		if (!program)
		{
			ERH_va_report_error_fprintf("There is no program to save to file %s.\n", file_name);
		}

		if (program_size < 1)
		{
			ERH_va_report_error_fprintf("There is no program contents to save to file %s.\n", file_name);
		}
	}

	if (successful)
	{
		FILE* out_file = fopen(file_name, "w");
		if (!out_file)
		{
			successful = false;
			ERH_va_report_error_fprintf("Unable to open file %s for writing\n.", file_name);
		}
		else
		{
			successful = write_program_to_file(program, program_size, out_file);
			fclose(out_file);
		}
	}

	return successful;
}

bool initialize_control_console(ARGF_ARG_FLAGS_PTR command_line_arguments)
{
	bool successful = true;


	return successful;
}

#endif // !CONTROL_CONSOLE_C
