/*
 * file_io_vm.c
 *
 * Handles the input and output of text files that contain virtual machine programs.
 */

#ifndef FILE_IO_VM_H
#define FILE_IO_VM_H

#include "human_readable_program_format.h"

bool write_program_to_file(const Human_Readable_Program_Format* program, const size_t program_size, FILE* out_file);
Human_Readable_Program_Format* read_program_from_file(FILE* input_file, size_t* program_size, const char* file_name);

#endif
