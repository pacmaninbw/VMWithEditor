#include <stdio.h>
#include <stdlib.h>
#include "common_program_logic.h"
#include "arg_flags.h"

static ARG_FLAGS_PTR construct_arguments()
{
	ARG_FLAGS_PTR arguments = calloc(sizeof(*arguments), 1);
	if (!arguments)
	{
		fprintf(error_out_file, "In construct_arguments() calloc() failed.\n");
	}

	return arguments;
}

static void parse_command_line(ARG_FLAGS_PTR arguments, int argc, char **argv)
{

}

ARG_FLAGS_PTR parse_argc_argv(int argc, char **argv)
{
	ARG_FLAGS_PTR arguments = construct_arguments();
	if (arguments)
	{
		if (argc == 1)
		{
			arguments->use_console_text = true;
			arguments->use_default_program = true;
		}
		else
		{
			parse_command_line(arguments, argc, argv);
		}
	}

	return arguments;
}

ARG_FLAGS_PTR delete_arguments(ARG_FLAGS_PTR arguments)
{
	free(arguments);
	return NULL;
}