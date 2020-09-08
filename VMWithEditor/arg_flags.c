/*
 * arg_flags.c
 *
 * Parse command line arguments and return a struct with all necessary flags
 * and data variables set.
 */

#ifndef ARG_FLAGS_C
#define ARG_FLAGS_C

#include <stdio.h>
#include <stdlib.h>

#include "ARGF_arg_flags.h"
#include "ERH_error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif

static ARGF_ARG_FLAGS_PTR construct_arguments()
{
	ARGF_ARG_FLAGS_PTR arguments = calloc(sizeof(*arguments), 1);
	if (!arguments)
	{
		ERH_va_report_error_fprintf("In construct_arguments() calloc() failed.\n");
	}

	return arguments;
}

static void parse_command_line(ARGF_ARG_FLAGS_PTR arguments, const int argc, const char **argv)
{

}

ARGF_ARG_FLAGS_PTR ARGF_construct_arg_flags_from_argc_argv(const int argc, const char **argv)
{
	ARGF_ARG_FLAGS_PTR arguments = construct_arguments();
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

ARGF_ARG_FLAGS_PTR ARGF_delete_arguments(ARGF_ARG_FLAGS_PTR arguments)
{
	free(arguments);
	return NULL;
}

#endif	// ARG_FLAGS_C
