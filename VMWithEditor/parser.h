#ifndef PARSER_H
#define PARSER_H

#include "human_readable_program_format.h"
#include <stdint.h>

Program_Step_Node* parser(unsigned char* text_line, size_t* line_number, char* file_name);

#endif
