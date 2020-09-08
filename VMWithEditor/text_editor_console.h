/*
 * text_editor_console.c
 *
 * This file contains the public interfaces to the user interface functions for
 * the console text editor the allows the user to create and edit virtual
 * machine programs.
 */

#ifndef TEXT_EDITOR_CONSOLE_H
#define TEXT_EDITOR_CONSOLE_H

#include "OPC_opcode.h"
#include "HRF_human_readable_program_format.h"
#include "ARGF_arg_flags.h"

bool initialize_text_editor(const ARGF_ARG_FLAGS_PTR command_line_arguments);

#endif
