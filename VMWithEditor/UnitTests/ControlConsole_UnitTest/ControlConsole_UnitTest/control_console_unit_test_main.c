
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "common_unit_test_logic.h"
#include "control_console_unit_test_main.h"

bool run_all_control_console_unit_tests(void)
{
    bool all_tests_passed = true;

    printf("Hello World!\n");

    return all_tests_passed;
}

#ifndef ALL_UNIT_TESTING
int main()
{
    int unit_test_status = EXIT_SUCCESS;

    if (!run_all_control_console_unit_tests())
    {
        unit_test_status = EXIT_FAILURE;
    }

    return unit_test_status;
}
#endif


