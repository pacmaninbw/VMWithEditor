#ifndef UNIT_TEST_HUMAN_READABLE_PROGRAM_FORMAT_H
#define UNIT_TEST_HUMAN_READABLE_PROGRAM_FORMAT_H
// Unit tests that can be called from main.

#include <stdint.h>
#include <stdbool.h>

bool unit_test_hrf_duplicate_program(size_t test_step);
bool unit_test_hrf_create_program_step(size_t test_step);
bool unit_test_hrf_convert_array_program_to_linked_list(size_t test_step);
bool unit_test_hrf_convert_link_list_program_to_array(size_t test_step);
bool unit_test_all_human_readable_format();

#endif
