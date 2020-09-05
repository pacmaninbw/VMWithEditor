/*
 * my_strdup.h
 *
 * Rather than relying on strdup() which will be included in the C2x standard,
 * but has not been a C standard up to this point, this program supplies its
 * own version of strdup as well as a strdup for unsigned characters.
 */

#ifndef MY_STRDUP_H
#define MY_STRDUP_H

char* my_strdup(const char* string_to_copy);
unsigned char* ucstrdup(const unsigned char* string_to_copy);

#endif	// MY_STRDUP_H
