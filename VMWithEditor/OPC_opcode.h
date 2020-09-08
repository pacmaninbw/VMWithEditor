/*
 * OPC_opcode.h
 *
 * Provides the opcodes and opcode translation service APis.
 */

#ifndef OPC_OPCODE_H
#define OPC_OPCODE_H

typedef enum opcodes {
	OPC_HALT = 0x00,
	OPC_FIRST_OPCODE = OPC_HALT,
	OPC_PUSH = 0x01,
	OPC_POP = 0x02,
	OPC_STORE = 0x03,
	OPC_LOAD = 0x04,
	OPC_ADD = 0x05,
	OPC_SUBTRACT = 0x06,
	OPC_MULTIPLY = 0x07,
	OPC_DIVIDE = 0x08,
	OPC_OUTPUTCHAR = 0x09,
	OPC_INPUTCHAR = 0x0A,
	OPC_LASTOPCODE = OPC_INPUTCHAR		// Add new opcode before LASTOPCODE and change LASTOPCODE to the new last opcode
} OPC_OPCODE;

/*
 *When Adding or changing opcodes make sure to change the code in opcode.c as well.
 */

typedef struct opcode_translation_unit
{
	OPC_OPCODE opcode;
	char* opcode_string_translation;
} OPC_OPCODE_TRANSLATION_UNIT;

#ifdef OPCODE_C
OPC_OPCODE_TRANSLATION_UNIT opcode_translator[] =
{
	{OPC_HALT, "HALT"},
	{OPC_PUSH, "PUSH"},
	{OPC_POP, "POP"},
	{OPC_STORE, "STORE"},
	{OPC_LOAD, "LOAD"},
	{OPC_ADD, "ADD"},
	{OPC_SUBTRACT, "SUBTRACT"},
	{OPC_MULTIPLY, "MULTIPLY"},
	{OPC_DIVIDE, "DIVIDE"},
	{OPC_OUTPUTCHAR, "OUTPUTCHAR"},
	{OPC_INPUTCHAR, "INPUTCHAR"}
};
#else
extern OPC_OPCODE_TRANSLATION_UNIT opcode_translator[];
#endif

#define OPC_OPCODE_TRANSLATOR_ARRAY_SIZE (10 + 1) // sizeof(opcode_translator) / sizeof(*opcode_translator)

char* OPC_translate_opcode_to_string(const OPC_OPCODE opcode);
OPC_OPCODE OPC_translate_string_to_opcode(const unsigned char* string_opcode);

#endif	// !OPC_OPCODE_H
