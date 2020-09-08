/*
 * common_console_functions.c
 */
#ifndef COMMON_CONSOLE_FUNCTIONS_C
#define COMMON_CONSOLE_FUNCTIONS_C

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_console_functions.h"
#include "ERH_error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif

size_t get_user_selection(const size_t menu_minimum, const size_t menu_maximum)
{
	size_t user_selection = 0;
	size_t selection_size = 0;

	while (!selection_size)
	{
		unsigned char user_selection_string[BUFSIZ];

		selection_size = strlen(fgets((char *) user_selection_string, BUFSIZ, stdin));
		if (selection_size)		// If there was input
		{
			for (size_t length = 0; length < selection_size && selection_size > 0; length++)
			{
				if (!isdigit(user_selection_string[length]))
				{
					selection_size = 0;
				}
			}

			user_selection = atoi((const char * )user_selection_string);
			if (user_selection < menu_minimum || user_selection > menu_maximum)
			{
				selection_size = 0;
			}
		}

		if (!selection_size)
		{
			ERH_va_report_error_fprintf("Please enter a number between %d and %d\n", (int)menu_minimum, (int)menu_maximum);
		}
	}

	return selection_size;
}

#endif // !COMMON_CONSOLE_FUNCTIONS_C
