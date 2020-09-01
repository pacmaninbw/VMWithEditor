#ifndef FILE_IO_VM_H
#define FILE_IO_VM_H

#include "human_readable_program_format.h"

bool write_program_to_file(Human_Readable_Program_Format* program, size_t program_size, FILE* out_file);
Human_Readable_Program_Format* read_program_from_file(FILE* input_file, size_t* program_size, char* file_name);

#endif
