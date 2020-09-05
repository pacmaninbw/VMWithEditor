/*
 * opcode.c
 */
#ifndef OPCODE_C
#define OPCODE_C

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "error_reporting.h"
#include "opcode.h"

OPCODE translate_string_to_opcode(const unsigned char* string_opcode)
{
	char* local_s_opcode = (char *) string_opcode;
	for (size_t i = 0; i < (size_t)LASTOPCODE; i++)
	{
		if (!strcmp(local_s_opcode, opcode_translator[i].opcode_string_translation))
		{
			return opcode_translator[i].opcode;
		}
	}

	fprintf(error_out_file, "Opcode translation error: %s is not a legal opcode name.\n", local_s_opcode);
	return (OPCODE)-1;
}

char* translate_opcode_to_string(const OPCODE opcode)
{
	return opcode_translator[(size_t)opcode].opcode_string_translation;
}

#endif	// OPCODE_C
