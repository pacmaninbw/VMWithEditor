#include "my_strdup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* string_to_copy)
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
	unsigned char* return_string = (unsigned char* )my_strdup((const char *)string_to_copy);

	return return_string;
}

