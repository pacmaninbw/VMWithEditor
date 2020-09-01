#ifndef LEXICAL_ANALYZER_UNIT_TEST_UTILITIES_H
#define LEXICAL_ANALYZER_UNIT_TEST_UTILITIES_H

#include <stdint.h>
#include "lexical_analyzer.h"

const char* state_name_for_printing(const Syntax_State state);
const char* transition_names(const State_Transition_Characters transition);

#endif	// LEXICAL_ANALYZER_UNIT_TEST_UTILITIES_H
