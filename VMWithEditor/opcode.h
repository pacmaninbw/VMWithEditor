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

extern OPCODE_TRANSLATION_UNIT opcode_translator[];

#define OPCODE_TRANSLATOR_COUNT 11 // sizeof(opcode_translator) / sizeof(*opcode_translator)

extern void initialize_opcode_translater(void);
extern char* translate_opcode_to_string(OPCODE opcode);
extern OPCODE translate_string_to_opcode(unsigned char* string_opcode);

#endif
