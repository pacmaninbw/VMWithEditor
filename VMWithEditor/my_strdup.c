/*
 * my_strdup.c
 *
 * Rather than relying on strdup() which will be included in the C2x standard,
 * but has not been a C standard up to this point, this program supplies its
 * own version of strdup as well as a strdup for unsigned characters.
 */

#ifndef MY_STRDUP_C
#define MY_STRDUP_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "my_strdup.h"

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

#endif	// !MY_STRDUP_C
