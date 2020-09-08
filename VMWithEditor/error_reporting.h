/*
 * error_reporting.c
 */

#ifndef ERROR_REPORTING_H
#define ERROR_REPORTING_H

#include <stdbool.h>
#include <stdio.h>

#define ERH_ERROR_BUFFER_SIZE 2048

extern FILE* ERH_error_out_file;

bool ERH_init_vm_error_reporting(const char* error_log_file_name);
void ERH_disengage_error_reporting(void);
void ERH_report_error_generic(const char* error_message);
void ERH_report_error_output_fopen_failed(const char* file_name);
void ERH_report_error_input_fopen_failed(const char* file_name);
void ERH_va_report_error_fprintf(const char* format, ...);

#endif	// !ERROR_REPORTING_H
