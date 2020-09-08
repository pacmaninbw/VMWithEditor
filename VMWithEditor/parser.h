/* parser.h
 *
 * The functions in this file are responsible for parsing any files input by
 * the user that contain virtual memory programs.
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include "HRF_human_readable_program_format.h"

HRF_Program_Step_Node* parser(const unsigned char* text_line, size_t* line_number, const char* file_name);

#endif
