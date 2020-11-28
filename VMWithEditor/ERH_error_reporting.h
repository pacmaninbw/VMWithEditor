/*
 * ERH_error_reporting.h
 */

#ifndef ERH_ERROR_REPORTING_H
#define ERH_ERROR_REPORTING_H

#include <stdbool.h>
#include <stdio.h>

#define ERH_ERROR_BUFFER_SIZE 2048

extern FILE* ERH_error_out_file;

bool ERH_error_reporting_is_initialized(void);
bool ERH_init_vm_error_reporting(const char* error_log_file_name);
void ERH_disengage_error_reporting(void);
void ERH_report_error_generic(const char* error_message);
void ERH_report_error_output_fopen_failed(const char* file_name);
void ERH_report_error_input_fopen_failed(const char* file_name);
void ERH_va_report_error_fprintf(const char* format, ...);
void ERH_use_perror_when_errno(const char* error_message);

#endif	/* !ERH_ERROR_REPORTING_H */
