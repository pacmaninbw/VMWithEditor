/*
 * opcode.c
 */
#ifndef OPCODE_C
#define OPCODE_C

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "vmwitheditor.h"
#include "ERH_error_reporting.h"
#include "OPC_opcode.h"

OPC_OPCODE OPC_translate_string_to_opcode(Const_U_Char* string_opcode)
{
	char* local_s_opcode = (char *) string_opcode;
	if (local_s_opcode)
	{
		for (size_t i = 0; i <= (size_t)OPC_LASTOPCODE; i++)
		{
			if (!strcmp(local_s_opcode, opcode_translator[i].opcode_string_translation))
			{
				return opcode_translator[i].opcode;
			}
		}

		ERH_va_report_error_fprintf("Opcode translation error: %s is not a legal opcode name.\n", local_s_opcode);
	}
	else
	{
		ERH_va_report_error_fprintf("Opcode translation error: Opcode string is NULL.\n");
	}
	return (OPC_OPCODE)-1;
}

char* OPC_translate_opcode_to_string(const OPC_OPCODE opcode)
{
	return opcode_translator[(size_t)opcode].opcode_string_translation;
}

#endif	// OPCODE_C
