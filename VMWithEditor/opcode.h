#ifndef OPCODE_H
#define OPCODE_H

typedef enum opcodes {
	HALT = 0x00,
	FIRST_OPCODE = HALT,
	PUSH = 0x01,
	POP = 0x02,
	STORE = 0x03,
	LOAD = 0x04,
	ADD = 0x05,
	SUBTRACT = 0x06,
	MULTIPLY = 0x07,
	DIVIDE = 0x08,
	OUTPUTCHAR = 0x09,
	INPUTCHAR = 0x0A,
	LASTOPCODE = INPUTCHAR		// Add new opcode before LASTOPCODE and change LASTOPCODE to the new last opcode
} OPCODE;

/*
 *When Adding or changing opcodes make sure to change the code in opcode.c as well.
 */

typedef struct opcode_translation_unit
{
	OPCODE opcode;
	char* opcode_string_translation;
} OPCODE_TRANSLATION_UNIT;

#ifdef OPCODE_C
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
#else
extern OPCODE_TRANSLATION_UNIT opcode_translator[];
#endif

#define OPCODE_TRANSLATOR_ARRAY_SIZE (10 + 1) // sizeof(opcode_translator) / sizeof(*opcode_translator)

char* translate_opcode_to_string(const OPCODE opcode);
OPCODE translate_string_to_opcode(const unsigned char* string_opcode);

#endif
