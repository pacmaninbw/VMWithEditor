#ifndef CONTROL_CONSOLE_UNIT_TEST_C_
#define CONTROL_CONSOLE_UNIT_TEST_C_

/*
 * control_console_unit_test_main.c
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ERH_error_reporting.h"
#include "UTL_unit_test_logging.h"
#include "control_console_unit_test_main.h"

bool run_all_control_console_unit_tests(void)
{
    bool all_tests_passed = true;

    printf("Hello World!\n");

    return all_tests_passed;
}

/*
 * The Control Console Unit Test is included in other unit tests such as the
 * RunAllUnitTests,the main() function is only needed when the Control Console
 * unit test is built as a stand alone application.
 */
#ifdef CONTROL_CONSOLE_UNIT_TESTING
int main()
{
    int unit_test_status = EXIT_SUCCESS;
    ERH_error_out_file = stderr;
    const size_t unit_test_test_step = 1;

    if (!ERH_init_vm_error_reporting(NULL) ||
        !UTL_init_unit_tests("control_console_unit_test_log.txt"))
    {
        return EXIT_FAILURE;
    }

    if (!run_all_control_console_unit_tests())
    {
        unit_test_status = EXIT_FAILURE;
    }

    UTL_close_unit_tests();
    ERH_disengage_error_reporting();

    return unit_test_status;
}
#endif

#endif // !CONTROL_CONSOLE_UNIT_TEST_C_
