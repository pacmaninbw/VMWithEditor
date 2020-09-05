/*
 * error_reporting.c
 */

#ifndef ERROR_REPORTING_H
#define ERROR_REPORTING_H

#include <stdbool.h>
#include <stdio.h>

extern FILE* error_out_file;

bool init_vm_error_reporting(const char* error_log_file_name);
void disengage_error_reporting(void);
void report_error_generic(const char* error_message);

#endif	// !ERROR_REPORTING_H
