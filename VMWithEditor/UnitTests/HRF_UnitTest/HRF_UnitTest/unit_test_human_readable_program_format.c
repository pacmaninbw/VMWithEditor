/* This file is included by human_readable_program_format.c because there is unit testing of static
 * functions and variables. Because this this file is included by human_readable_program_format.c
 * include files are not necessary.
 */

#ifdef UNIT_TEST_TEMPLATE
static bool unit_test_template(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	bool this_test_passed = true;

	show_test_status_each_step("template", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}
#endif

static void show_test_status_each_step(char* function_name, bool status, char *path, bool stand_alone)
{
	if (stand_alone)
	{
		fprintf(unit_test_log_file, "%s(): %s Path %s\n", function_name, path, (status)? "Passed" : "Failed");
	}
}

static bool unit_test_print_syntax_errors(char* file_name, unsigned test_step)
{
	bool passed = true;
	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
	char* test_file_name = NULL;
	char* text_line = "Unit Testing print_syntax_errors()";
	bool this_test_passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	size_t line_number = 1;

	test_file_name = (!file_name || !test_step)? "No File Name" : file_name;

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
	this_test_passed = print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char *) text_line);
	show_test_status_each_step("unit_test_print_syntax_errors", this_test_passed, "Positive", stand_alone_test);
	passed = (!this_test_passed)? false : passed;
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
	show_test_status_each_step("unit_test_print_syntax_errors", this_test_passed, "Negative", stand_alone_test);
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

static bool single_syntax_test(bool stand_alone_test, Syntax_Strings_For_Testing test_data, char * test_file_name)
{
	bool this_test_passed = true;
	Program_Step_Node* test_head = NULL;
	size_t prog_size = 1;

	char* test_path = test_data.path_is_positive ? "Positive" : "Negative";
	test_head = hrf_check_line_syntax_return_program_step_if_valid((unsigned char *)test_data.string_to_check, &prog_size, test_file_name);
	this_test_passed = test_data.path_is_positive ?
		(test_head && test_head->opcode_and_operand.opcode == HALT) :
		(!test_head);
	show_test_status_each_step("unit_test_check_line_syntax_return_program_step_if_valid", this_test_passed, test_path, stand_alone_test);
	free(test_head);
	fprintf(unit_test_log_file, "\n");

	return this_test_passed;
}

static bool unit_test_check_line_syntax_return_program_step_if_valid(char *file_name, unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	char *test_file_name = NULL;
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
	for ( ; test_count < END_HRF_SYNTAX_POSITIVE_TESTING && passed; test_count++)
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

bool unit_test_hrf_duplicate_program(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nUnit test: unit_test_hrf_duplicate_program NOT IMPLEMENTED\n");
	}

	show_test_status_each_step("unit_test_hrf_duplicate_program", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}

bool unit_test_hrf_create_program_step(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing in unit_test_hrf_create_program_step.\n");
	}

	size_t test_program_size = 0;
	Human_Readable_Program_Format* test_program = default_program(&test_program_size);

	Program_Step_Node* test_head = NULL;
	Program_Step_Node* test_tail = NULL;
	size_t step_count = 0;
	for ( ; step_count < test_program_size; step_count++)
	{
		if (!test_tail)
		{
			test_tail = create_program_step(&test_program[step_count]);
			if (!test_tail)
			{
				show_test_status_each_step("unit_test_hrf_create_program_step", false, "Positive", stand_alone_test);
				return false;
			}
			test_head = test_tail;
		}
		else
		{
			test_tail->next_step = create_program_step(&test_program[step_count]);
			test_tail = test_tail->next_step;
			if (!test_tail)
			{
				show_test_status_each_step("unit_test_hrf_create_program_step", false, "Positive", stand_alone_test);
				return false;
			}
		}
	}

	step_count = 0;
	test_tail = test_head;
	while (test_tail)
	{
		bool this_test_passed = true;
		this_test_passed = (test_tail->opcode_and_operand.opcode == test_program[step_count].opcode && test_tail->opcode_and_operand.operand == test_program[step_count].operand);
		if (!this_test_passed)
		{
			passed = false;
		}
		step_count++;
		test_tail = test_tail->next_step;
	}

	show_test_status_each_step("unit_test_hrf_create_program_step", passed, "Positive", stand_alone_test);

	delete_linked_list_of_program_steps(test_head);
	free(test_program);
	
	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nEndinging POSITIVE PATH testing in unit_test_hrf_create_program_step.\n");
	}

	return passed;
}

bool unit_test_hrf_convert_array_program_to_linked_list(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nUnit test: unit_test_hrf_convert_array_program_to_linked_list NOT IMPLEMENTED\n");
	}

	show_test_status_each_step("unit_test_hrf_convert_array_program_to_linked_list", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}

bool unit_test_hrf_convert_link_list_program_to_array(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nUnit test: unit_test_hrf_convert_link_list_program_to_array NOT IMPLEMENTED\n");
	}

	show_test_status_each_step("unit_test_hrf_convert_link_list_program_to_array", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}

bool unit_test_hrf_writing_to_file(char *file_name, unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	bool this_test_passed = true;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nUnit test: unit_test_hrf_writing_to_file NOT IMPLEMENTED\n");
	}

	show_test_status_each_step("unit_test_hrf_writing_to_file", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}

bool unit_test_hrf_reading_from_file(char* file_name, unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	bool this_test_passed = true;


	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\n\nStarting Unit Test for read_program_from_file()\n\n");

		this_test_passed = unit_test_print_syntax_errors(file_name, test_step);
		passed = (!this_test_passed) ? false : passed;
		if (this_test_passed)
		{
			this_test_passed = unit_test_check_line_syntax_return_program_step_if_valid(file_name, test_step);
			passed = (!this_test_passed) ? false : passed;
		}
	}

	show_test_status_each_step("unit_test_hrf_reading_from_file", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\n\nEnding Unit Test for read_program_from_file()\n\n");
	}

	return passed;
}

bool unit_test_all_human_readable_format()
{
	bool passed = true;

	fprintf(unit_test_log_file, "\nUnit test: unit_test_all_human_readable_format NOT IMPLEMENTED\n");

	return passed;
}

void write_to_test_log_file(char *buffer)
{
	fprintf(unit_test_log_file, "%s", buffer);
}

bool init_hrf_unit_tests(char* log_file_name)
{
	if (log_file_name)
	{
		unit_test_log_file = fopen(log_file_name, "w");
		if (!unit_test_log_file)
		{
			fprintf(error_out_file, "Can't open %s for output\n", log_file_name);
			return false;
		}
		error_out_file = unit_test_log_file;
	}
	else
	{
		unit_test_log_file = stdout;
		error_out_file = stderr;
	}

	return true;
}

void close_hrf_unit_tests(void)
{
	if (unit_test_log_file != stdout)
	{
		fclose(unit_test_log_file);
	}
}
