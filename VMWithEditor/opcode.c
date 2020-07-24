#ifdef UNIT_TESTING
#include "common_unit_test_logic.h"
#else
#include "common_program_logic.h"
#endif
#include "opcode.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Can't trust this initialization because the opcodes may change values, but it gives us the proper size for the array.
OPCODE_TRANSLATION_UNIT opcode_translator[] =
{
	{HALT, "HALT"},
	{PUSH, "PUSH"},
	{POP, "POP"},
	{STORE, "STORE"},
	{LOAD, "LOAD"},
	{ADD, "ADD"},
	{SUBTRACT, "SUBTRACT"},
	{MULTIPLY, "MULTIPLY"},
	{DIVIDE, "DIVIDE"},
	{OUTPUTCHAR, "OUTPUTCHAR"},
	{INPUTCHAR, "INPUTCHAR"}
};



// Allows the flexibility to reorder the opcodes in the header file and/or change the opcode values.
void initialize_opcode_translater()
{
	// Since there is no guarantee the opcodes will keep the same values, explicitly add each opcode and string overriding the initializing at compile time.
	// This allows the flexibility to change the value of the opcodes.
	opcode_translator[HALT] = (OPCODE_TRANSLATION_UNIT) { HALT, "HALT" };
	opcode_translator[PUSH] = (OPCODE_TRANSLATION_UNIT) { PUSH, "PUSH" };
	opcode_translator[POP] = (OPCODE_TRANSLATION_UNIT) { POP, "POP" };
	opcode_translator[STORE] = (OPCODE_TRANSLATION_UNIT) { STORE, "STORE" };
	opcode_translator[LOAD] = (OPCODE_TRANSLATION_UNIT) { LOAD, "LOAD" };
	opcode_translator[ADD] = (OPCODE_TRANSLATION_UNIT) { ADD, "ADD" };
	opcode_translator[SUBTRACT] = (OPCODE_TRANSLATION_UNIT) { SUBTRACT, "SUBTRACT" };
	opcode_translator[MULTIPLY] = (OPCODE_TRANSLATION_UNIT) { MULTIPLY, "MULTIPLY" };
	opcode_translator[DIVIDE] = (OPCODE_TRANSLATION_UNIT) { DIVIDE, "DIVIDE" };
	opcode_translator[OUTPUTCHAR] = (OPCODE_TRANSLATION_UNIT) { OUTPUTCHAR, "OUTPUTCHAR" };
	opcode_translator[INPUTCHAR] = (OPCODE_TRANSLATION_UNIT) { INPUTCHAR, "INPUTCHAR" };
}

OPCODE translate_string_to_opcode(unsigned char* string_opcode)
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

char* translate_opcode_to_string(OPCODE opcode)
{
	return opcode_translator[(size_t)opcode].opcode_string_translation;
}