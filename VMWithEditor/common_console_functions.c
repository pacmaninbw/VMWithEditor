#include "common_program_logic.h"
#include "common_console_functions.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_user_selection(size_t menu_minimum, size_t menu_maximum)
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
			fprintf(error_out_file, "Please enter a number between %d and %d\n", (int)menu_minimum, (int)menu_maximum);
		}
	}

	return selection_size;
}
