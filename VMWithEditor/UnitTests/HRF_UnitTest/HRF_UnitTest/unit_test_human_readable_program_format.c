#ifndef UNIT_TEST_HUMAN_READABLE_PROGRAM_FORMAT_C
#define UNIT_TEST_HUMAN_READABLE_PROGRAM_FORMAT_C
/* This file is includes the file HRF_Human_Readable_Program_Format.c because there
 * is unit testing of static functions and variables. 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "default_program.h"
#include "HRF_human_readable_program_format.h"
#include "unit_test_human_readable_program_format.h"
#include "UTL_unit_test_logging.h"

#include "human_readable_program_format.c"

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

bool unit_test_hrf_duplicate_program(size_t test_step)
{
	bool passed = true;
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_hrf_duplicate_program", passed, UTL_NEGATIVE_PATH,
		test_step == 0, false);

	UTL_log_test_not_implemented(log_data);
	this_test_passed = log_data->status;

	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_hrf_create_program_step(size_t test_step)
{
	bool passed = true;

	UTL_Test_Log_Data *log_data = UTL_new_log_data(
		"unit_test_hrf_create_program_step", passed, UTL_POSITIVE_PATH,
		test_step == 0, true);

	UTL_log_start_test_path(log_data, NULL);

	size_t test_program_size = 0;
	HRF_Human_Readable_Program_Format* test_program = default_program(&test_program_size);

	HRF_Program_Step_Node* test_head = NULL;
	HRF_Program_Step_Node* test_tail = NULL;
	size_t step_count = 0;
	for ( ; step_count < test_program_size; step_count++)
	{
		if (!test_tail)
		{
			test_tail = HRF_create_program_step(&test_program[step_count]);
			test_head = test_tail;
		}
		else
		{
			test_tail->next_step = HRF_create_program_step(&test_program[step_count]);
			test_tail = test_tail->next_step;
		}
		// Now that an attempted allocation has been made to test_tail test for success
		if (!test_tail)
		{
			log_data->status = false;
			UTL_log_test_status_each_step(log_data);
			return log_data->status;
		}
	}

	step_count = 0;
	test_tail = test_head;
	while (test_tail)
	{
		bool this_test_passed = true;
		this_test_passed = (test_tail->opcode_and_operand.opcode ==
			test_program[step_count].opcode && test_tail->opcode_and_operand.operand
			== test_program[step_count].operand);
		if (!this_test_passed)
		{
			passed = false;
		}
		step_count++;
		test_tail = test_tail->next_step;
	}

	log_data->status = passed;
	UTL_log_test_status_each_step(log_data);

	HRF_delete_linked_list_of_program_steps(test_head);
	free(test_program);
	
	UTL_log_end_test_path(log_data, NULL);
	free(log_data);

	return passed;
}

bool unit_test_hrf_convert_array_program_to_linked_list(size_t test_step)
{
	bool passed = true;
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_hrf_convert_array_program_to_linked_list", passed,
		UTL_NEGATIVE_PATH, (!test_step), true);

	UTL_log_test_not_implemented(log_data);
	this_test_passed = log_data->status;

	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_hrf_convert_link_list_program_to_array(size_t test_step)
{
	bool passed = true;
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_hrf_convert_link_list_program_to_array", passed, 
		UTL_NEGATIVE_PATH, (!test_step), true);

	UTL_log_test_not_implemented(log_data);
	this_test_passed = log_data->status;

	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_all_human_readable_format()
{
	bool passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_all_human_readable_format", passed,
		UTL_NEGATIVE_PATH, false, true);

	UTL_log_test_not_implemented(log_data);
	passed = log_data->status;

	return passed;
}

#endif // !UNIT_TEST_HUMAN_READABLE_PROGRAM_FORMAT_C
