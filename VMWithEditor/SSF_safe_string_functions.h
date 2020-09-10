/*
 * SSF_safe_string_functions.h
 *
 * Provides safe versions of string functions such as strcat().
 *
 * Rather than relying on strdup() which will be included in the C2x standard,
 * but has not been a C standard up to this point, this program supplies its
 * own version of strdup as well as a strdup for unsigned characters.
 *
 * All global symbols provided by this module start with SSF_ to identiy the
 * source of the functions.
 */

#ifndef SSF_SAFE_STRING_FUNCTIONS_H
#define SSF_SAFE_STRING_FUNCTIONS_H

#include "vmwitheditor.h"

void SSF_report_strdup_failure(char* function_name, char* allocated_string);

/*
 * Basic strdup function that won't have name space collisions with strdup()
 * when it becomes part of the C standard.
 */
char* SSF_strdup(const char* string_to_copy);

/*
 * Provide an unsigned char* version of strdup().
 */
unsigned char* SSF_ucstrdup(Const_U_Char* string_to_copy);

/*
 * Provides a safe version of strcat that makes sure the destination string
 * will not overflow.
 */
char* SSF_strcat(char* destination, char* source, size_t destination_size);

#endif	// SSF_SAFE_STRING_FUNCTIONS_H
