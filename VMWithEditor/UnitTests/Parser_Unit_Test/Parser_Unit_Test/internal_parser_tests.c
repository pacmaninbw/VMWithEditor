/*
 * internal_parser_tests.c
 *
 * This file includes parser.c because there is unit testing of static
 * functions and variables. Because this this file is included by parser.c
 * include files are not necessary.
 */

#ifndef INTERNAL_PARSER_TESTS_C
#define INTERNAL_PARSER_TESTS_C

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ERH_error_reporting.h"
#include "HRF_human_readable_program_format.h"
#include "LAH_lexical_analyzer.h"
#include "UTL_unit_test_logging.h"
#include "internal_parser_tests.h"
#include "OPC_opcode.h"

/*
 * Include parser.c so we can test the internal static functions in the file.
 */
#include "parser.c"

/*
 * This struct is used to test both get_opcode_or_operand_string() (GOOS) and
 * get_legal_opcode_or_oparand() (GLOO).
 */
typedef struct {
	UTL_Path_State test_path;
	LAH_Syntax_State state;
	size_t expected_opcode_or_operand;
	size_t source_string_length;
	char* source_test_string;
	char* expected_string;
} GOOS_And_GLOO_Test_Data;

static GOOS_And_GLOO_Test_Data goos_and_gloo_test_data[] = 
{
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_HALT, sizeof("HALT"), "HALT", "HALT"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_PUSH, sizeof("PUSH"), "PUSH", "PUSH"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_POP, sizeof("POP"), "POP", "POP"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_STORE, sizeof("STORE"), "STORE", "STORE"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_LOAD, sizeof("LOAD"), "LOAD", "LOAD"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_ADD, sizeof("ADD"), "ADD", "ADD"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_SUBTRACT, sizeof("SUBTRACT"), "SUBTRACT", "SUBTRACT"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_MULTIPLY, sizeof("MULTIPLY"), "MULTIPLY", "MULTIPLY"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_DIVIDE, sizeof("DIVIDE"), "DIVIDE", "DIVIDE"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_OUTPUTCHAR, sizeof("OUTPUTCHAR"), "OUTPUTCHAR", "OUTPUTCHAR"},
	{UTL_POSITIVE_PATH, LAH_OPCODE_STATE, OPC_INPUTCHAR, sizeof("INPUTCHAR"), "INPUTCHAR", "INPUTCHAR"},
	{UTL_POSITIVE_PATH, LAH_OPERAND_STATE, 0x00, sizeof("0x00"), "0x00", "0x00"},		// Minimum legal value
	{UTL_POSITIVE_PATH, LAH_OPERAND_STATE, 0xFFFFFF, sizeof("0xFFFFFF"), "0xFFFFFF", "0xFFFFFF"},	// Maximum legal value
	{UTL_POSITIVE_PATH, LAH_OPERAND_STATE, 0xFFFFFE, sizeof("0xFFFFFE"), "0xFFFFFE", "0xFFFFFE"},
	{UTL_POSITIVE_PATH, LAH_OPERAND_STATE, 0x01, sizeof("0x01"), "0x01", "0x01"},
	{UTL_POSITIVE_PATH, LAH_OPERAND_STATE, 0x000FFF, sizeof("0x000FFF"), "0x000FFF", "0x000FFF"},
/* 
 *End Positive Test Path
 */
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, (size_t)-1, sizeof("HEALT"), "HEALT", "HEALT"},	// Passes GOOS fails GLOO, invalid opcode
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, OPC_HALT, 0, "HEALT", NULL},						// Fails Both GOOS and GLOO string length zero
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, (size_t)-1, sizeof("HEALT"), "HEALT", "HALT"},	// Passes GOOS fails GLOO, invalid opcode
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, (size_t)-1, sizeof("HEALT"), NULL, NULL}, 		// Fails Both GOOS and GLOO NULL input string
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, (size_t)-1, 0, NULL, NULL}, 						// Fails Both GOOS and GLOO
	{UTL_NEGATIVE_PATH, LAH_OPCODE_STATE, (size_t)-1, sizeof("0x00"), "0x00", "0x00"},		// Passes GOOS fails GLOO, invalid opcode
	{UTL_NEGATIVE_PATH, LAH_OPERAND_STATE, (size_t)-1, sizeof("HALT"), "HALT", "HALT"},		// Passes GOOS fails GLOO, not numeric
	{UTL_NEGATIVE_PATH, LAH_OPERAND_STATE, 0xFFFFFFFF, sizeof("0xFFFFFFFF"), "0xFFFFFFFF", "0xFFFFFFFF"},	// Maximum legal value exceded
	{UTL_NEGATIVE_PATH, LAH_START_STATE, (size_t)-1, sizeof("0x000FFF"), "0x000FFF", "0x000FFF"},		// Passes GOOS fails GLOO, invalid state
};

static const size_t get_ops_test_count = sizeof(goos_and_gloo_test_data) /
	sizeof(*goos_and_gloo_test_data);

static bool execute_single_get_opcode_or_operand_string_test(UTL_Test_Log_Data* log_data,
	GOOS_And_GLOO_Test_Data test_data)
{
	unsigned char* test_string = get_opcode_or_operand_string((Const_U_Char *)
		test_data.source_test_string, test_data.source_string_length);
	if (test_string && test_data.expected_string)
	{
		if (strncmp((char *)test_string, test_data.expected_string,
			test_data.source_string_length))
		{
			log_data->status = false;
		}
	}
	else
	{
		if ((!test_string && test_data.expected_string) || (test_string && !test_data.expected_string))
		{
			log_data->status = false;
		}
	}

	UTL_log_test_status_each_step(log_data);

	return log_data->status;
}

static bool unit_test_get_opcode_or_operand_string(UTL_Test_Log_Data* log_data)
{
	bool passed = true;
	bool this_path_passed = true;
	log_data->function_name = "unit_test_get_opcode_or_operand_string";
	log_data->status = passed;
	log_data->path = UTL_POSITIVE_PATH;
	UTL_Path_State current_path = UTL_POSITIVE_PATH;

	UTL_log_start_unit_test(log_data, NULL);
	UTL_log_start_test_path(log_data, NULL);

	for (size_t test_count = 0; test_count < get_ops_test_count; test_count++)
	{
		log_data->status = true;
		if (current_path != goos_and_gloo_test_data[test_count].test_path)
		{
			log_data->status = this_path_passed;
			UTL_log_end_test_path(log_data, NULL);
			current_path = goos_and_gloo_test_data[test_count].test_path;
			log_data->path = current_path;
			UTL_log_start_test_path(log_data, NULL);
			this_path_passed = true;
		}
		log_data->status = execute_single_get_opcode_or_operand_string_test(log_data,
			goos_and_gloo_test_data[test_count]);
		if (this_path_passed && !log_data->status)
		{
			this_path_passed = log_data->status;
		}
		if (passed && !log_data->status)
		{
			passed = log_data->status;
		}
	}

	log_data->status = this_path_passed;
	UTL_log_end_test_path(log_data, NULL);

	log_data->status = passed;
	UTL_log_end_unit_test(log_data, NULL);

	return passed;
}

static bool execute_single_get_legal_opcode_or_operand_test(UTL_Test_Log_Data* log_data,
	GOOS_And_GLOO_Test_Data test_data)
{
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));

	if (test_data.state == LAH_OPCODE_STATE)
	{
		OPC_OPCODE opcode = get_legal_opcode_or_oparand((Const_U_Char*)
			test_data.source_test_string, test_data.source_string_length,
			syntax_check_list, LAH_OPCODE_STATE);
			log_data->status = opcode == (OPC_OPCODE)test_data.expected_opcode_or_operand;
	}
	else
	{
		HRF_OperandType operand = get_legal_opcode_or_oparand((Const_U_Char*)test_data.source_test_string, test_data.source_string_length, syntax_check_list, test_data.state);
		log_data->status = operand == test_data.expected_opcode_or_operand;
	}

	UTL_log_test_status_each_step(log_data);

	return log_data->status;
}

static bool unit_test_get_legal_opcode_or_oparand(UTL_Test_Log_Data* log_data)
{
	bool passed = true;
	bool this_path_passed = true;
	log_data->function_name = "unit_test_get_legal_opcode_or_oparand";
	log_data->status = passed;
	log_data->path = UTL_POSITIVE_PATH;
	UTL_Path_State current_path = UTL_POSITIVE_PATH;

	UTL_log_start_unit_test(log_data, NULL);
	UTL_log_start_test_path(log_data, NULL);

	for (size_t test_count = 0; test_count < get_ops_test_count; test_count++)
	{
		log_data->status = true;
		if (current_path != goos_and_gloo_test_data[test_count].test_path)
		{
			log_data->status = this_path_passed;
			UTL_log_end_test_path(log_data, NULL);
			current_path = goos_and_gloo_test_data[test_count].test_path;
			log_data->path = current_path;
			UTL_log_start_test_path(log_data, NULL);
			this_path_passed = true;
		}
		log_data->status = execute_single_get_legal_opcode_or_operand_test(log_data, goos_and_gloo_test_data[test_count]);
		if (this_path_passed && !log_data->status)
		{
			this_path_passed = log_data->status;
		}
		if (passed && !log_data->status)
		{
			passed = log_data->status;
		}
	}

	UTL_log_end_test_path(log_data, NULL);


	UTL_log_test_not_implemented(log_data);
	log_data->status = false;

	UTL_log_end_unit_test(log_data, NULL);

	return passed;
}

static bool unit_test_print_syntax_errors(UTL_Test_Log_Data* log_data)
{
	bool passed = true;
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	char* test_file_name = "No File Name";
	char* text_line = "Unit Testing print_syntax_errors()";
	bool this_test_passed = true;
	log_data->function_name = "unit_test_print_syntax_errors";
	log_data->status = passed;
	log_data->path = UTL_POSITIVE_PATH;
	size_t line_number = 1;

	UTL_log_start_unit_test(log_data, NULL);
	UTL_log_start_test_path(log_data, NULL);

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i < LAH_ILLEGALOPCODE)
		{
			syntax_check_list[i] = 1;
		}
		else
		{
			syntax_check_list[i] = 0;
		}
	}

	this_test_passed = print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;
	UTL_log_end_test_path(log_data, NULL);

	log_data->path = UTL_NEGATIVE_PATH;
	UTL_log_start_test_path(log_data, NULL);
	/*
	 * The Ultimate negative test path, every check should fail.
	 */
	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i < LAH_ILLEGALOPCODE)
		{
			syntax_check_list[i] = 0;
		}
		else
		{
			syntax_check_list[i] = 1;
		}
	}

	this_test_passed = !print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;
	log_data->status = this_test_passed;
	UTL_log_end_test_path(log_data, NULL);

	return passed;
}

typedef bool (*parser_unit_test_function)(UTL_Test_Log_Data* log_data);

typedef struct unit_test_functions_and_args
{
	char* test_name;
	parser_unit_test_function func;
} Parser_Unit_Test_Functions;


bool run_all_internal_parser_unit_tests(UTL_Test_Log_Data* log_data)
{
	bool all_tests_passed = true;

	Parser_Unit_Test_Functions unit_tests[] =
	{
		{"unit_test_get_opcode_or_operand_string",
			unit_test_get_opcode_or_operand_string},
		{"unit_test_get_legal_opcode_or_oparand",
			unit_test_get_legal_opcode_or_oparand},
		{"unit_test_print_syntax_errors",
			unit_test_print_syntax_errors},
	};
	size_t test_max = (sizeof(unit_tests) / sizeof(*unit_tests));

	UTL_log_start_unit_test(log_data, NULL);

	const char* overall_log_function_name = log_data->function_name;
	for (size_t test_count = 0; test_count < test_max; test_count++)
	{
		bool test_passed = unit_tests[test_count].func(log_data);
		log_data->function_name = unit_tests[test_count].test_name;
		log_data->status = test_passed;
		UTL_log_high_level_test_result(log_data, test_count + 1);
		// if one test already failed we are good
		if (all_tests_passed)
		{
			all_tests_passed = test_passed;
		}
	}

	log_data->function_name = overall_log_function_name;
	log_data->status = all_tests_passed;
	UTL_log_end_unit_test(log_data, NULL);

	return all_tests_passed;
}

#endif // !INTERNAL_PARSER_TESTS_C
