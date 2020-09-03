#ifndef MY_STRDUP_H
#define MY_STRDUP_H

#include <string.h>

#ifndef strdup
#ifdef _MSC_VER
#if _MSC_VER > 1920
#define strdup _strdup
#endif
#else
#define strdup mystrdup      
#endif
#endif

char* mystrdup(const char* string_to_copy);
unsigned char* ucstrdup(const unsigned char* string_to_copy);

#endif	// MY_STRDUP_H
