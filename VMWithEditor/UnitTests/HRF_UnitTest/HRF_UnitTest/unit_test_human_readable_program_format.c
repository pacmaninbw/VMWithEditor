/* This file is includes the file HRF_Human_Readable_Program_Format.c because there
 * is unit testing of static functions and variables. 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "default_program.h"
#include "HRF_Human_Readable_Program_Format.h"
#include "unit_test_human_readable_program_format.h"
#include "UTL_unit_test_logging.h"

#include "Human_Readable_Program_Format.c"

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
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data("unit_test_hrf_duplicate_program", passed, UTL_NEGATIVE_PATH, stand_alone_test);

	if (stand_alone_test)
	{
		UTL_va_log_fprintf("\nUnit test: unit_test_hrf_duplicate_program NOT IMPLEMENTED\n");
	}

	UTL_log_test_status_each_step2(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_hrf_create_program_step(size_t test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);

	UTL_Test_Log_Data *log_data = UTL_create_and_init_test_log_data("unit_test_hrf_create_program_step", passed, UTL_POSITIVE_PATH, stand_alone_test);

	if (log_data->stand_alone)
	{
		UTL_log_start_positive_path(log_data->function_name);
	}

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
			if (!test_tail)
			{
				log_data->status = false;
				UTL_log_test_status_each_step2(log_data);
				return false;
			}
			test_head = test_tail;
		}
		else
		{
			test_tail->next_step = HRF_create_program_step(&test_program[step_count]);
			test_tail = test_tail->next_step;
			if (!test_tail)
			{
				log_data->status = false;
				UTL_log_test_status_each_step2(log_data);
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
	UTL_log_test_status_each_step2(log_data);

	HRF_delete_linked_list_of_program_steps(test_head);
	free(test_program);
	
	if (log_data->stand_alone)
	{
		UTL_log_end_positive_path(log_data->function_name);
	}
	free(log_data);

	return passed;
}

bool unit_test_hrf_convert_array_program_to_linked_list(size_t test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data("unit_test_hrf_convert_array_program_to_linked_list", passed, UTL_NEGATIVE_PATH, (!test_step));

	if (stand_alone_test)
	{
		UTL_va_log_fprintf("\nUnit test: unit_test_hrf_convert_array_program_to_linked_list NOT IMPLEMENTED\n");
	}

	UTL_log_test_status_each_step2(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_hrf_convert_link_list_program_to_array(size_t test_step)
{
	bool passed = true;
	bool stand_alone_test = (!test_step);
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data("unit_test_hrf_convert_link_list_program_to_array", passed, UTL_NEGATIVE_PATH, (!test_step));

	if (stand_alone_test)
	{
		UTL_va_log_fprintf("\nUnit test: unit_test_hrf_convert_link_list_program_to_array NOT IMPLEMENTED\n");
	}

	UTL_log_test_status_each_step2(log_data);
	passed = (!this_test_passed) ? false : passed;

	free(log_data);

	return passed;
}

bool unit_test_all_human_readable_format()
{
	bool passed = true;

	UTL_va_log_fprintf("\nUnit test: unit_test_all_human_readable_format NOT IMPLEMENTED\n");

	return passed;
}

