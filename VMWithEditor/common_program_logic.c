#ifdef UNIT_TESTING
#include "common_unit_test_logic.h"
#else
#include "common_program_logic.h"
#endif
#include "virtual_machine.h"
#include <stdio.h>
#include <stdlib.h>

FILE* error_out_file = NULL;

char* mystrdup(const char* string_to_copy)
{
	char* return_string = NULL;
	size_t length = strlen(string_to_copy);
	++length;

	return_string = calloc(length, sizeof(*return_string));
	if (return_string)
	{
		memcpy(return_string, string_to_copy, length - 1);
	}

	return return_string;
}

unsigned char* ucstrdup(const unsigned char* string_to_copy)
{
	unsigned char* return_string = NULL;
	size_t length = strlen((const char*)string_to_copy);
	++length;

	return_string = calloc(length, sizeof(*return_string));
	if (return_string)
	{
		memcpy(return_string, string_to_copy, length - 1);
	}

	return return_string;
}

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


bool execute_original_program_logic(ARG_FLAGS_PTR command_line_args)
{
	bool successful = true;

	size_t program_size = 0;
	Human_Readable_Program_Format* program = default_program(&program_size);
	if (program)
	{
		if (!load_and_run_program(program, program_size))
		{
			fprintf(error_out_file, "In execute_program_logic() load_and_run_program() failed\n");
			successful = false;
		}
		free(program);
	}
	else
	{
		successful = false;
	}

	return successful;
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

bool set_up_command_line_args_and_vm(ARG_FLAGS_PTR* command_line_args, int argc, char* argv[])
{
	bool successful = true;

	*command_line_args = parse_argc_argv(argc, argv);
	if (!command_line_args)
	{
		fprintf(error_out_file, "Due to memory allocation error %s is exiting.\n", argv[0]);
		successful = false;
	}

	if (successful)
	{
		if (!initialize_virtual_machine())
		{
			*command_line_args = delete_arguments(*command_line_args);
			fprintf(error_out_file, "Unable to initialize Virtual Machine, %s is exiting\n", argv[0]);
			successful = false;
		}
	}

	return successful;
}

void report_error_generic(char* error_message)
{
	fprintf(error_out_file, "%s\n", error_message);
}

