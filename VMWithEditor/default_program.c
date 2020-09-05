/* default_program.c
 *
 * At least for the time this will be the default program to run until the editor
 * and the control consol are finished. This program will be used throughout the
 * unit tests. This is the original program in the original code by the author of
 * the first question.
 * https://codereview.stackexchange.com/questions/244566/an-attempt-at-a-toy-vm/244573
 */

#ifndef DEFAULT_PROGRAM_C
#define DEFAULT_PROGRAM_C

#include <stdint.h>
#include "human_readable_program_format.h"
#include "default_program.h"

Human_Readable_Program_Format* default_program(size_t* program_size)
{
	Human_Readable_Program_Format program[] =
	{
		{PUSH, 0x0A},
		{PUSH, 0x43},
		{PUSH, 0x42},
		{PUSH, 0x41},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{OUTPUTCHAR, 0x00},
		{POP, 0x00},
		{HALT, 0x00}
	};

	size_t progsize = sizeof(program) / sizeof(*program);

	Human_Readable_Program_Format* copy_of_program = duplicate_program(program, progsize);
	if (copy_of_program)
	{
		*program_size = progsize;
	}

	return copy_of_program;
}



#endif	// DEFAULT_PROGRAM_C