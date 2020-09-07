#ifndef RUN_ALL_UNIT_TESTS_MAIN_C
#define RUN_ALL_UNIT_TESTS_MAIN_C

/*
 * run_all_unit_tests_main.c
 * This file combines all the stand alone unit tests into a single set of unit tests.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include "error_reporting.h"
#include "lexical_analyzer_unit_test_main.h"
#include "parser_unit_test_main.h"
#include "unit_test_logging.h"

int main()
{
    printf("Hello World!\n");
}

#endif // !RUN_ALL_UNIT_TESTS_MAIN_C
