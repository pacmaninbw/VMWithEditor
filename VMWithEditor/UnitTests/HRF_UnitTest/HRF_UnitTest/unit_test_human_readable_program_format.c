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

bool unit_test_hrf_duplicate_program(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;

	if (stand_alone_test)
	{
		fprintf(unit_test_log_file, "\nUnit test: unit_test_hrf_duplicate_program NOT IMPLEMENTED\n");
	}

	log_test_status_each_step("unit_test_hrf_duplicate_program", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

	return passed;
}

bool unit_test_hrf_create_program_step(unsigned test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);

	Test_Log_Data *log_data = create_and_init_test_log_data("unit_test_state_transition_on_comma", passed, "Positive", test_step == 0);

	if (log_data->stand_alone)
	{
		log_start_positive_path(log_data->function_name);
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
				log_data->status = false;
				log_test_status_each_step2(log_data);
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
				log_data->status = false;
				log_test_status_each_step2(log_data);
				return log_data->status;
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

	log_data->status = passed;
	log_test_status_each_step2(log_data);

	delete_linked_list_of_program_steps(test_head);
	free(test_program);
	
	if (log_data->stand_alone)
	{
		log_end_positive_path(log_data->function_name);
	}
	free(log_data);

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

	log_test_status_each_step("unit_test_hrf_convert_array_program_to_linked_list", this_test_passed, "Negative", stand_alone_test);
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

	log_test_status_each_step("unit_test_hrf_convert_link_list_program_to_array", this_test_passed, "Negative", stand_alone_test);
	passed = (!this_test_passed) ? false : passed;

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

