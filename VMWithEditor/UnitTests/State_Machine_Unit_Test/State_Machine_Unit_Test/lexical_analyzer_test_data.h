#ifndef LEXICAL_ANALYZER_TEST_DATA_H
#define LEXICAL_ANALYZER_TEST_DATA_H

#include "lexical_analyzer.h"

typedef struct expected_syntax_errors
{
	unsigned error_count;
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
} Expected_Syntax_Errors;

typedef struct lexical_analyzer_test_data
{
	unsigned char** test_program;
	size_t test_program_size;
	Expected_Syntax_Errors* expected_errors;
} Lexical_Analyzer_Test_Data;

extern void deallocate_lexical_test_data(Lexical_Analyzer_Test_Data* deletee);
extern void lexical_analyzer_test_data_allocation_failed(Test_Log_Data* log_data, char* allocating_function,
	char* allocation_function);
extern Lexical_Analyzer_Test_Data* init_positive_path_data_for_lexical_analysis(Test_Log_Data* log_data);
extern Lexical_Analyzer_Test_Data* init_negative_path_data_for_lexical_analysis(Test_Log_Data* log_data);

#endif	// LEXICAL_ANALYZER_TEST_DATA_H