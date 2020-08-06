//#include "common_program_logic.h"
#include "virtual_machine.h"
#include <stdio.h>
#include <stdlib.h>

FILE* error_out_file = NULL;

bool init_vm_error_reporting(char* error_log_file_name)
{
	bool status_is_good = true;

	if (error_log_file_name)
	{
		error_out_file = fopen(error_log_file_name, "w");
		if (!error_out_file)
		{
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


Human_Readable_Program_Format* default_program(size_t* program_size)
{
	Human_Readable_Program_Format program[] =
	{
		{PUSH, 0x0A},
		{PUSH, 0x43},
		{PUSH, 0x42},
		{PUSH, 0x41},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{HALT, 0x00}
	};

	size_t progsize = sizeof(program) / sizeof(*program);

	Human_Readable_Program_Format* copy_of_program = duplicate_program(program, progsize);
	if (copy_of_program)
	{
		*program_size = progsize;
	}

	return copy_of_program;
}


