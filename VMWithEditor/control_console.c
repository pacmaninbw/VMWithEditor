#ifdef UNIT_TESTING
#include "common_unit_test_logic.h"
#else
#include "common_program_logic.h"
#endif
#include "common_console_functions.h"
#include "control_console.h"
#include "virtual_machine.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void show_master_menu(void)
{

}

bool console_edit_and_run_program(Human_Readable_Program_Format* program)
{
	bool successful = true;

	return successful;
}

bool console_edit_and_run_program_file(const char* file_name)
{
	bool successful = true;

	return successful;
}

bool save_program_to_file(Human_Readable_Program_Format* program, size_t program_size, const char* file_name)
{
	bool successful = true;

	if (!program || program_size == 0)
	{
		successful = false;
		if (!program)
		{
			fprintf(error_out_file, "There is no program to save to file %s.\n", file_name);
		}
		if (program_size < 1)
		{
			fprintf(error_out_file, "There is no program contents to save to file %s.\n", file_name);
		}
	}

	if (successful)
	{
		FILE* out_file = fopen(file_name, "w");
		if (!out_file)
		{
			successful = false;
			fprintf(error_out_file, "Unable to open file %s for writing\n.", file_name);
		}
		else
		{
			successful = write_program_to_file(program, program_size, out_file);
			fclose(out_file);
		}
	}

	return successful;
}

bool initialize_control_console(ARG_FLAGS_PTR command_line_arguments)
{
	bool successful = true;


	return successful;
}
