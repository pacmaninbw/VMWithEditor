/*
 * virtual_machine.c
 *
 * This file contains an alternate implementation of the virtual machine
 * presented in the original question on code review
 * https://codereview.stackexchange.com/questions/244566/an-attempt-at-a-toy-vm/244573
 * This implementation is an attempt to isolate the code in a more modular form
 * rather than the public structure that was used in the original question.
 */

#ifndef VIRTUAL_MACHINE_C
#define VIRTUAL_MACHINE_C

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporting.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif
#include "virtual_machine.h"

static const size_t STACK_TOP = 1024;
static const OperandType OPERAND_MASK = 0xFFFFFF00;
static const OperandType OPCODE_SHIFT = 8;

#define MEMORY_SIZE 0x1000					// #define because VC 2019 does not accept const in the array declaration below.

static OperandType vmachine_memory[MEMORY_SIZE];
static size_t stack_pointer;
static size_t program_counter;
static bool running;
static OperandType raw;
static OPCODE opcode;
static OperandType param;
static size_t last_program_size;

// To place the public interfaces first create function prototypes for the various private functions.
static bool load_program_into_memory(const Human_Readable_Program_Format program[], const size_t program_size);
static void fetch(void);
static void decode(void);
static void execute(void);
static void halt(void);
static void push(void);
static void pop(void);
static void store(void);
static void load(void);
static void add(void);
static void subtract(void);
static void multiply(void);
static void divide(void);
static void outputchar(void);
static void inputchar(void);

void reset_vm(void)
{
	running = false;
	// In a real computer the memory would not be erased, but to prevent errors it is here.
	if (last_program_size) {
		memset(&vmachine_memory[0], 0, last_program_size * sizeof(vmachine_memory[0]));	// optimization, only clear what was previously programmed.
	}
	else
	{
		memset(&vmachine_memory[0], 0, MEMORY_SIZE * sizeof(vmachine_memory[0]));
	}
	
	program_counter = STACK_TOP;
	stack_pointer = 0;
	opcode = 0;
	raw = 0;
	param = 0;
	last_program_size = 0;
}

typedef void (*Executable_Opcode_Function)() ;

static Executable_Opcode_Function executable_opcodes[OPCODE_TRANSLATOR_ARRAY_SIZE];

/*
 * Public interfaces
 */
bool initialize_virtual_machine(void)
{
	bool successful = true;

	last_program_size = 0;

	// Since there is no guarantee the opcodes will keep the same values, explicitly add each opcode function rather than initializing at compile time.
	// This allows the flexibility to change the value of the opcodes.
	executable_opcodes[HALT] = halt;
	executable_opcodes[PUSH] = push;
	executable_opcodes[POP] = pop;
	executable_opcodes[STORE] = store;
	executable_opcodes[LOAD] = load;
	executable_opcodes[ADD] = add;
	executable_opcodes[SUBTRACT] = subtract;
	executable_opcodes[MULTIPLY] = multiply;
	executable_opcodes[DIVIDE] = divide;
	executable_opcodes[OUTPUTCHAR] = outputchar;
	executable_opcodes[INPUTCHAR] = inputchar;

	reset_vm();

	return successful;
}

bool program_fits_in_memory(size_t program_size)
{
	return (program_size < (MEMORY_SIZE - STACK_TOP));
}

void run_vm(void)
{
	running = true;
	while (running)
	{
		fetch();
		decode();
		execute();
		program_counter++;
	}
}

bool load_and_run_program(const Human_Readable_Program_Format program[],
	const size_t programSize)
{
	bool successful = true;

	if (load_program_into_memory(program, programSize))
	{
		run_vm();
	}
	else
	{
		successful = false;
	}

	return successful;
}

// Allows other modules to get the maximum operand value
size_t get_maximum_operand_value(void)
{
	return OPERAND_MASK >> OPCODE_SHIFT;
}

// Allows other modules to check if the opcode specified is a legal value
bool is_legal_operand(const unsigned operand)
{
	return operand <= get_maximum_operand_value();
}

long translate_text_to_operand_and_validate(const char* string_operand)
{
	long possible_operand = strtol(string_operand, NULL, 0);

	if (!is_legal_operand(possible_operand))
	{
		possible_operand = -1;
	}

	return possible_operand;
}

/* 
 * Private functions that hide the Virtual Machine implementation
*/

// Translate Human_Readable_Program_Format program into virtual machine program and load into memory
static bool load_program_into_memory(const Human_Readable_Program_Format program[],
	const size_t program_size)
{
	bool successful = true;
	bool halt_opcode_found = false;

	if (program_fits_in_memory(program_size))
	{
		OperandType* memory_location = &vmachine_memory[STACK_TOP];
		Human_Readable_Program_Format* program_location = 
			(Human_Readable_Program_Format*)program;

		for (size_t i = 0; i < program_size; i++, memory_location++, program_location++)
		{
			*memory_location = ((program_location->operand << OPCODE_SHIFT) & OPERAND_MASK)
				| (program_location->opcode & 0xff);
			if (program_location->opcode == HALT)
			{
				halt_opcode_found = true;
			}
		}

		last_program_size = program_size;
		if (!halt_opcode_found)
		{
			successful = false;
			ERH_va_report_error_fprintf("While loading the program into memory, no halt "
				"instruction was found.\nTo prevent unknown errors program will not "
				"run, and has been unloaded.\n");
			reset_vm();
		}
	}
	else
	{
		ERH_va_report_error_fprintf("Program too large for virtual machine memory.\n");
		successful = false;
	}

	return successful;
}

static void fetch(void)
{
	raw = vmachine_memory[program_counter];
}

static void decode(void)
{
	/*
	 * style of opcode
	 * 24 bits for parameter
	 * a byte for the opcode
	*/
		opcode = raw & 0xff;
		param = (raw & OPERAND_MASK) >> OPCODE_SHIFT;
}

static void halt(void)
{
	running = false;
	printf("\nHalt\n");
}

static void push(void)
{
	vmachine_memory[++stack_pointer] = param;
}

static void pop(void)
{
	vmachine_memory[stack_pointer--] = 0;
}

static void store(void)
{
	vmachine_memory[vmachine_memory[stack_pointer - 1]] = vmachine_memory[stack_pointer];
}

static void load(void)
{
	vmachine_memory[stack_pointer + 1] = vmachine_memory[vmachine_memory[stack_pointer]];
	++stack_pointer;
}

static void add(void)
{
	vmachine_memory[stack_pointer + 1] = vmachine_memory[stack_pointer] +
		vmachine_memory[stack_pointer - 1];
	++stack_pointer;
}

static void subtract(void)
{
	vmachine_memory[stack_pointer + 1] = vmachine_memory[stack_pointer - 1] -
		vmachine_memory[stack_pointer];
	++stack_pointer;
}

static void multiply(void)
{
	vmachine_memory[stack_pointer + 1] = vmachine_memory[stack_pointer] *
		vmachine_memory[stack_pointer - 1];
	++stack_pointer;
}

static void divide(void)
{
	if (vmachine_memory[stack_pointer] == 0)
	{
		ERH_va_report_error_fprintf("Division by zero attempted at stack location 0x%x, "
			"Virtual Program terminated.\n", (unsigned)stack_pointer);
		running = false;
		return;
	}
	vmachine_memory[stack_pointer + 1] = vmachine_memory[stack_pointer - 1] /
		vmachine_memory[stack_pointer];
	++stack_pointer;
}

static void outputchar(void)
{
	printf("%c", vmachine_memory[stack_pointer]);
}

static void inputchar(void)
{
	vmachine_memory[++stack_pointer] = getchar();
}

static void execute(void) {
	if (opcode < FIRST_OPCODE || opcode > LASTOPCODE)
	{
		ERH_va_report_error_fprintf("Unknown Opcode in execute(). 0x%x program_count = %d, "
			"stack_pointer = %d.\nTerminating Virtual Machine execution.\n",
			opcode, (unsigned)program_counter, (unsigned)stack_pointer);
		running = false;
		return;
	}

	executable_opcodes[opcode]();		// Should be faster than using a switch/case statement 
}

#endif	// !VIRTUAL_MACHINE_C
