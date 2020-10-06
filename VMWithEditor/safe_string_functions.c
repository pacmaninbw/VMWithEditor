/*
 * safe_string_functions.c
 *
 * Rather than relying on strdup() which will be included in the C2x standard,
 * but has not been a C standard up to this point, this program supplies its
 * own version of strdup as well as a strdup for unsigned characters.
 */

#ifndef SAFE_STRING_FUNCTIONS_C
#define SAFE_STRING_FUNCTIONS_C

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vmwitheditor.h"
#include "SSF_safe_string_functions.h"
#include "ERH_error_reporting.h"

void SSF_report_strdup_failure(char *function_name, char* allocated_string)
{
	char buffer[ERH_ERROR_BUFFER_SIZE];
	snprintf(buffer, sizeof(buffer), "in %s: SSF_strdup() returned NULL for %s",
		function_name, allocated_string);

	ERH_use_perror_when_errno(buffer);
}

/* This function was heavily influenced by this code review:
 * https://codereview.stackexchange.com/questions/248817/common-unit-testing-code-follow-up/249103#249103 
 * and by the comments and answers to this question on stackoverflow
 * https://stackoverflow.com/questions/63732752/is-this-a-portable-strdup
 */
char* SSF_strdup(const char* string_to_copy)
{
	if (!string_to_copy)
	{
#ifdef EINVAL
		errno = EINVAL;
#endif
		return NULL;
	}

	size_t length = strlen(string_to_copy);
	++length;
	char* return_string = malloc(length);
	if (return_string)
	{
		memcpy(return_string, string_to_copy, length);
	}
	else
	{
#ifdef ENOMEM
		errno = ENOMEM;
#else
		;
#endif
	}

	return return_string;
}

/* 
 * Do any necessary casting in only one place.
 */
unsigned char* SSF_ucstrdup(Const_U_Char* string_to_copy)
{
	unsigned char* return_string = (unsigned char* )SSF_strdup((const char *)string_to_copy);

	return return_string;
}

char* SSF_strcat(char* destination, char* source, size_t destination_size)
{
	if (destination)
	{
		destination[0] = 0;
	}

	if (!destination || !source || !(destination_size > 0))
	{
#ifdef EINVAL
		errno = EINVAL;
#endif
		ERH_use_perror_when_errno("Invalid argument in SSF_strcat.");
	}
	else
	{
		if ((strlen(destination) + strlen(source) + 1) < destination_size)
		{
			strcat(destination, source);
		}
		else
		{
#ifdef EINVAL
			errno = EINVAL;
#endif
			ERH_use_perror_when_errno("In SSF_strcat destination is not large enough to concatenate source.");
		}
	}

	return destination;
}

char* SSF_strcpy(char* destination, char* source, size_t destination_size)
{
	if (destination)
	{
		destination[0] = 0;
	}

	if (!destination || !source || !(destination_size > 0))
	{
#ifdef EINVAL
		errno = EINVAL;
#endif
		ERH_use_perror_when_errno("Invalid argument in SSF_strcpy.");
	}
	else
	{
		if (strlen(source) + 1 <= destination_size)
		{
			strcpy(destination, source);
		}
		else
		{
#ifdef EINVAL
			errno = EINVAL;
#endif
			ERH_use_perror_when_errno("In SSF_strcpy destination is shorter than source.");
		}
	}

	return destination;
}

#endif	// !SAFE_STRING_FUNCTIONS_C
