/*
 * safe_string_functions.c
 *
 * Rather than relying on strdup() which will be included in the C2x standard,
 * but has not been a C standard up to this point, this program supplies its
 * own version of strdup as well as a strdup for unsigned characters.
 */

#ifndef SAFE_STRING_FUNCTIONS_C
#define SAFE_STRING_FUNCTIONS_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "safe_string_functions.h"

char* SSF_strdup(const char* string_to_copy)
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

unsigned char* SSF_ucstrdup(const unsigned char* string_to_copy)
{
	unsigned char* return_string = (unsigned char* )SSF_strdup((const char *)string_to_copy);

	return return_string;
}

char* SSF_strcat(char* destination, char* source, size_t destination_size)
{
	if (strlen(destination) + strlen(source) < destination_size)
	{
		strcat(destination, source);
	}

	return destination;
}

#endif	// !SAFE_STRING_FUNCTIONS_C
