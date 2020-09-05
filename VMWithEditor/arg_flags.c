/*
 * arg_flags.c
 *
 * Parse command line arguments and return a struct with all necessary flags set.
 */

#ifndef ARG_FLAGS_C
#define ARG_FLAGS_C

#include <stdio.h>
#include <stdlib.h>

#include "arg_flags.h"
#include "error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif

static ARG_FLAGS_PTR construct_arguments()
{
	ARG_FLAGS_PTR arguments = calloc(sizeof(*arguments), 1);
	if (!arguments)
	{
		fprintf(error_out_file, "In construct_arguments() calloc() failed.\n");
	}

	return arguments;
}

static void parse_command_line(ARG_FLAGS_PTR arguments, const int argc, const char **argv)
{

}

ARG_FLAGS_PTR construct_arg_flags_from_argc_argv(const int argc, const char **argv)
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

#endif	// ARG_FLAGS_C
