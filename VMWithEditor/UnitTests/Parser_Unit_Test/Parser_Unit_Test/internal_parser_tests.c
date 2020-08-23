/* This file is included by parser.c because there is unit testing of static
 * functions and variables. Because this this file is included by parser.c
 * include files are not necessary.
 */



static bool unit_test_print_syntax_errors(char* file_name, unsigned test_step)
{
	bool passed = true;
	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
	char* test_file_name = NULL;
	char* text_line = "Unit Testing print_syntax_errors()";
	bool this_test_passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	size_t line_number = 1;

	test_file_name = (!file_name || !test_step) ? "No File Name" : file_name;

	fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing for unit_test_print_syntax_errors\n");
	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		if (i < ILLEGALOPCODE)
		{
			syntax_check_list[i] = 1;
		}
		else
		{
			syntax_check_list[i] = 0;
		}
	}
	this_test_passed = print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	log_test_status_each_step("unit_test_print_syntax_errors", this_test_passed, "Positive", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;
	fprintf(unit_test_log_file, "\nEnding POSITIVE PATH testing for unit_test_print_syntax_errors\n");

	fprintf(unit_test_log_file, "\nStarting NEGATIVE PATH testing for unit_test_print_syntax_errors\n");
	/*
	 * The Ultimate negative test path, every check should fail.
	 */
	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		if (i < ILLEGALOPCODE)
		{
			syntax_check_list[i] = 0;
		}
		else
		{
			syntax_check_list[i] = 1;
		}
	}
	this_test_passed = !print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	log_test_status_each_step("unit_test_print_syntax_errors", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;
	fprintf(unit_test_log_file, "\nEndting NEGATIVE PATH testing for unit_test_print_syntax_errors\n");

	return passed;
}

typedef struct syntax_strings_for_testing
{
	char* string_to_check;
	bool path_is_positive;
} Syntax_Strings_For_Testing;

#define END_HRF_SYNTAX_POSITIVE_TESTING	2		// Change this if more positive tests are added below.

static Syntax_Strings_For_Testing* init_syntax_test_data(size_t* test_data_size)
{
	Syntax_Strings_For_Testing check_line_syntax_test_data[] =
	{
		{"{HALT, 0x0}", true},
		{"{HALT, 0x0},", true},
		{"{HALT 0x0},", false},					// missing comma
		{"{LOAD, 0x0}, {HALT, 0x0},", false},	// 1 per line
		{"HALT, 0x0},", false},					// missing opening brace
		{"{HALT, 0x0,", false},					// missing closing brace
		{"{, 0x0}", false},						// missing opcode
		{"{ 0x0}", false},						// missing opcoe and comma
		{"{HALT, }"},							// missing operand
		{"{HALT}"}								// missing comma and operand
	};

	*test_data_size = sizeof(check_line_syntax_test_data) / sizeof(*check_line_syntax_test_data);

	Syntax_Strings_For_Testing* test_data = calloc(*test_data_size, sizeof(*test_data));
	if (test_data)
	{
		for (size_t i = 0; i < *test_data_size; i++)
		{
			test_data[i] = check_line_syntax_test_data[i];
		}
	}
	else
	{
		fprintf(error_out_file, "Calloc failed in init_syntax_test_data()\n");
	}

	return test_data;
}

static bool single_syntax_test(bool stand_alone_test, Syntax_Strings_For_Testing test_data, char* test_file_name)
{
	bool this_test_passed = true;
	Program_Step_Node* test_head = NULL;
	size_t prog_size = 1;

	char* test_path = test_data.path_is_positive ? "Positive" : "Negative";
	test_head = parser((unsigned char*)test_data.string_to_check, &prog_size, test_file_name);
	this_test_passed = test_data.path_is_positive ?
		(test_head && test_head->opcode_and_operand.opcode == HALT) :
		(!test_head);
	log_test_status_each_step("unit_test_check_line_syntax_return_program_step_if_valid", this_test_passed, test_path, stand_alone_test);
	free(test_head);
	fprintf(unit_test_log_file, "\n");

	return this_test_passed;
}

static bool unit_test_parser(char* file_name, unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	char* test_file_name = NULL;
	size_t test_data_size = 0;
	Syntax_Strings_For_Testing* test_data = init_syntax_test_data(&test_data_size);
	if (!test_data)
	{
		return false;
	}

	test_file_name = (!file_name || !test_step) ? "No File Name" : file_name;
	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\n\nStarting Unit Test for check_line_syntax_return_program_step_if_valid()\n");
		fprintf(unit_test_log_file, "\n\tStarting POSITIVE PATH Test for check_line_syntax_return_program_step_if_valid()\n\n");
	}

	size_t test_count = 0;
	for (; test_count < END_HRF_SYNTAX_POSITIVE_TESTING && passed; test_count++)
	{
		passed = single_syntax_test(stand_alone_test, test_data[test_count], test_file_name);
	}

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\n\tEnding POSITIVE PATH Test for check_line_syntax_return_program_step_if_valid()\n");
		fprintf(unit_test_log_file, "\n\tStarting NEGATIVE PATH Test for check_line_syntax_return_program_step_if_valid()\n\n");
	}

	for (; test_count < test_data_size && passed; test_count++)
	{
		passed = single_syntax_test(stand_alone_test, test_data[test_count], test_file_name);
	}

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\n\tEnding NEGATIVE PATH Test for check_line_syntax_return_program_step_if_valid()\n");
		fprintf(unit_test_log_file, "\nEnding Unit Test for check_line_syntax_return_program_step_if_valid()\n\n");
	}

	return passed;
}