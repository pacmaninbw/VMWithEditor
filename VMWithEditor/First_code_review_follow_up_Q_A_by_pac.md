[Answer](https://codereview.stackexchange.com/questions/248817/common-unit-testing-code-follow-up/248883?noredirect=1#comment488250_248883) by user pacmaninbw.

## The files `common_unit_test_logic.*` are too complex. 
 
The common_unit_test_logic .c file and header file do not follow the [Single Responsibility Principle][1] that states 
> … that every module, class, or function should have responsibility over a single part of the functionality provided by the software, and that responsibility should be entirely encapsulated by that module, class or function.  

This forced unnecessary #ifdef and #ifndef statements into the code. This has been rectified by breaking ` common_unit_test_logic.c` and ` common_unit_test_logic.h` into 3 separate modules, `error_reporting`, `my_strdup`, and ` unit_test_logging`.  

Only the ` unit_test_logging` module is still in the ` Common_UnitTest_Code` directory under the ` UnitTests` directory. The `error_reporting` module and the ` my_strdup` module have both been moved up to the ` VMWithEditor` source code directory so that they can be shared with the primary project as well as the multiple unit test projects. A forth module ` default_program ` was also created for the main program and some of the other unit tests, the code was ifdef’de out of the lexical analyzer unit test.  

Breaking the code up allows greater reuse of each of the modules, but requires additional `#include` statements in many of the files.

## The separated modules:  

**my_strdup.h**  

    #ifndef MY_STRDUP_H
    #define MY_STRDUP_H
    
    #include <string.h>
    
    #ifndef strdup
    #ifdef _MSC_VER
    #if _MSC_VER > 1920
    #define strdup _strdup
    #endif
    #else
    #define strdup mystrdup      
    #endif
    #endif
    
    char* mystrdup(const char* string_to_copy);
    unsigned char* ucstrdup(const unsigned char* string_to_copy);
    
    #endif    // MY_STRDUP_H

**my_strdup.c**  

    #include "my_strdup.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    char* mystrdup(const char* string_to_copy)
    {
        char* return_string = NULL;
        size_t length = strlen(string_to_copy);
        ++length;
    
        return_string = calloc(length, sizeof(*return_string));
        if (return_string)
        {
            memcpy(return_string, string_to_copy, length - 1);
        }
    
        return return_string;
    }
    
    unsigned char* ucstrdup(const unsigned char* string_to_copy)
    {
        unsigned char* return_string = NULL;
        size_t length = strlen((const char*)string_to_copy);
        ++length;
    
        return_string = calloc(length, sizeof(*return_string));
        if (return_string)
        {
            memcpy(return_string, string_to_copy, length - 1);
        }
    
        return return_string;
    }

**error_reporting.h**  

    #ifndef ERROR_REPORTING_H
    #define ERROR_REPORTING_H
    
    #include <stdbool.h>
    #include <stdio.h>
    
    extern FILE* error_out_file;
    
    bool init_vm_error_reporting(const char* error_log_file_name);
    void disengage_error_reporting(void);
    void report_error_generic(const char* error_message);
    
    #endif    // !ERROR_REPORTING_H

**error_reporting.c**  

    #ifndef ERROR_REPORTING_C
    #define ERROR_REPORTING_C
    
    #include "error_reporting.h"
    #ifdef UNIT_TESTING
    #include "unit_test_logging.h"
    #endif    // UNIT_TESTING
    #include <stdio.h>
    
    FILE* error_out_file = NULL;
    
    bool init_vm_error_reporting(const char* error_log_file_name)
    {
        bool status_is_good = true;
    
        if (error_log_file_name)
        {
            error_out_file = fopen(error_log_file_name, "w");
            if (!error_out_file)
            {
    #ifdef UNIT_TESTING
                error_out_file = stderr;
    #endif    // UNIT_TESTING
                fprintf(error_out_file, "Can't open error output file, %s", "error_log_file_name");
                status_is_good = false;
            }
        }
        else
        {
            error_out_file = stderr;
        }
    
        return status_is_good;
    }
    
    void disengage_error_reporting(void)
    {
        if (error_out_file != stderr)
        {
            fclose(error_out_file);
        }
    }
    
    void report_error_generic(const char *error_message)
    {
        fprintf(error_out_file, "%s\n", error_message);
    }
    
    #endif    // !ERROR_REPORTING_C

**default_program.h**  

    #ifndef DEFAULT_PROGRAM_H
    #define DEFAULT_PROGRAM_H
    
    #include "human_readable_program_format.h"
    #include <stdint.h>
    
    Human_Readable_Program_Format* default_program(size_t* program_size);


    #endif    // DEFAULT_PROGRAM_H

**default_program.c**  

    #ifndef DEFAULT_PROGRAM_C
    #define DEFAULT_PROGRAM_C
    
    #include "human_readable_program_format.h"
    #include "default_program.h"
    #include <stdint.h>
    
    Human_Readable_Program_Format* default_program(size_t* program_size)
    {
        Human_Readable_Program_Format program[] =
        {
            {PUSH, 0x0A},
            {PUSH, 0x43},
            {PUSH, 0x42},
            {PUSH, 0x41},
            {OUTPUTCHAR, 0x00},
            {POP, 0x00},
            {OUTPUTCHAR, 0x00},
            {POP, 0x00},
            {OUTPUTCHAR, 0x00},
            {POP, 0x00},
            {HALT, 0x00}
        };
    
        size_t progsize = sizeof(program) / sizeof(*program);
    
        Human_Readable_Program_Format* copy_of_program = duplicate_program(program, progsize);
        if (copy_of_program)
        {
            *program_size = progsize;
        }
    
        return copy_of_program;
    }
    
    #endif    // DEFAULT_PROGRAM_C

**unit_test_logging.h**  

    #ifndef UNIT_TEST_LOGGING_H
    #define UNIT_TEST_LOGGING_H
    #include <stdio.h>
    #include <stdbool.h>
    
    typedef struct test_log_data
    {
        const char* function_name;
        char* path;
        bool status;
        bool stand_alone;
    } Test_Log_Data;
    
    extern FILE* unit_test_log_file;
    
    bool init_unit_tests(const char* log_file_name);
    void report_create_and_init_test_log_data_memory_failure(const char* function_name);
    void log_test_status_each_step(const char* function_name, const bool status, const char* path, const bool stand_alone);
    void init_test_log_data(Test_Log_Data* log_data, const char* function_name, const bool status, char* path, const bool stand_alone);
    Test_Log_Data* create_and_init_test_log_data(const char* function_name, const bool status, char* path, const bool stand_alone);
    void log_test_status_each_step2(const Test_Log_Data* test_data_to_log);
    void log_start_positive_path(const char* function_name);
    void log_start_positive_path2(const Test_Log_Data* log_data);
    void log_start_test_path(const Test_Log_Data* log_data);
    void log_end_test_path(const Test_Log_Data* log_data);
    void log_end_positive_path(const char* function_name);
    void log_end_positive_path2(const Test_Log_Data* log_data);
    void log_start_negative_path(const char* function_name);
    void log_end_negative_path(const char* function_name);
    void log_generic_message(const char *log_message);
    void close_unit_tests(void);

    #endif // !UNIT_TEST_LOGGING_H

**unit_test_logging.c**  

    #include "error_reporting.h"
    #include <ctype.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    FILE* unit_test_log_file = NULL;
    
    
    bool init_unit_tests(const char* log_file_name)
    {
        if (log_file_name)
        {
            unit_test_log_file = fopen(log_file_name, "w");
            if (!unit_test_log_file)
            {
                fprintf(error_out_file, "Can't open %s for output\n", log_file_name);
                return false;
            }
            error_out_file = unit_test_log_file;
        }
        else
        {
            unit_test_log_file = stdout;
            error_out_file = stderr;
        }
    
        return true;
    }
    
    void close_unit_tests(void)
    {
        if (unit_test_log_file != stdout)
        {
            fclose(unit_test_log_file);
        }
    }
    
    static bool log_test_is_positive_path(const Test_Log_Data* log_data)
    {
        bool is_positive = true;
    
        if (!log_data->path)
        {
            fprintf(error_out_file, "Programmer error: log_data->path is NULL in log_test_is_positive_path()\n");
            return false;
        }
    
        char* string_to_test = strdup(log_data->path);
        if (!string_to_test)
        {
            fprintf(error_out_file, "Memory Allocation error: strdup() failed in log_test_is_positive_path()\n");
            fprintf(error_out_file, "Exiting program.\n");
            exit(EXIT_FAILURE);
        }
    
        char* stt_ptr = string_to_test;
        while (*stt_ptr)
        {
            *stt_ptr = (char) toupper(*stt_ptr);
            stt_ptr++;
        }
    
        is_positive = (strcmp(string_to_test, "POSITIVE") == 0);
        free(string_to_test);
    
        return is_positive;
    }
    
    void log_test_status_each_step(const char* function_name, const bool status, const char* path, const bool stand_alone)
    {
        if (stand_alone)
        {
            fprintf(unit_test_log_file, "%s(): %s Path %s\n", function_name, path,
                (status) ? "Passed" : "Failed");
        }
    }
    
    void log_test_status_each_step2(const Test_Log_Data *test_data_to_log)
    {
        if (test_data_to_log->stand_alone)
        {
            fprintf(unit_test_log_file, "%s(): %s Path %s\n", test_data_to_log->function_name,
                test_data_to_log->path, (test_data_to_log->status) ? "Passed" : "Failed");
        }
    }
    
    void log_start_positive_path(const char* function_name)
    {
        fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
            function_name);
    }
    
    void log_start_positive_path2(const Test_Log_Data *log_data)
    {
        fprintf(unit_test_log_file, "\nStarting POSITIVE PATH testing for %s\n\n",
            log_data->function_name);
    }
    
    void log_end_positive_path(const char* function_name)
    {
        fprintf(unit_test_log_file, "\nEnding POSITIVE PATH testing for %s\n", function_name);
    }
    
    void log_end_positive_path2(const Test_Log_Data* log_data)
    {
        fprintf(unit_test_log_file, "\nEnding POSITIVE PATH testing for %s, POSITIVE PATH  %s \n",
            log_data->function_name, log_data->status? "PASSED" : "FAILED");
    }
    
    void log_start_negative_path(const char* function_name)
    {
        fprintf(unit_test_log_file, "\nStarting NEGATIVE PATH testing for %s\n\n", function_name);
    }
    
    void log_end_negative_path(const char* function_name)
    {
        fprintf(unit_test_log_file, "\nEnding NEGATIVE PATH testing for %s\n", function_name);
        fflush(unit_test_log_file);        // Current unit test is done flush the output.
    }
    
    void log_start_test_path(const Test_Log_Data* log_data)
    {
        bool is_positive = log_test_is_positive_path(log_data);
    
        fprintf(unit_test_log_file, "\nStarting %s PATH testing for %s\n\n",
            is_positive ? "POSITIVE" : "NEGATIVE", log_data->function_name);
    }
    
    void log_end_test_path(const Test_Log_Data *log_data)
    {
        bool is_positive = log_test_is_positive_path(log_data);
    
        fprintf(unit_test_log_file, "\nEnding %s PATH testing for %s, Path %s\n",
            is_positive ? "POSITIVE" : "NEGATIVE", log_data->function_name,
            log_data->status ? "PASSED" : "FAILED");
    
        if (!is_positive)
        {
            fflush(unit_test_log_file);        // Current unit test is done flush the output.
        }
    }
    
    void log_generic_message(const char* log_message)
    {
        fprintf(unit_test_log_file, log_message);
    }
    
    void init_test_log_data(Test_Log_Data* log_data, const char *function_name, const bool status, char *path, bool stand_alone)
    {
        log_data->function_name = function_name;
        log_data->status = status;
        log_data->path = path;
        log_data->stand_alone = stand_alone;
    }
    
    Test_Log_Data *create_and_init_test_log_data(const char* function_name, const bool status, char* path, const bool stand_alone)
    {
        Test_Log_Data* log_data = calloc(1, sizeof(*log_data));
        if (log_data)
        {
            init_test_log_data(log_data, function_name, status, path, stand_alone);
        }
        else
        {
            fprintf(error_out_file, "In %s calloc() failed\n", "create_and_init_test_log_data");
        }
    
        return log_data;
    }
    
    // provides common error report for memory allocation error.
    void report_create_and_init_test_log_data_memory_failure(const char *function_name)
    {
        fprintf(error_out_file, "In function %s, Memory allocation failed in create_and_init_test_log_data\n", function_name);
    }

## Update 9/9/2020.  

In response to the [original answer][2] by @chux-ReinstateMonica as well as their comment below, `error_reporting.h` is now `ERH_error_reporting.h`, all global symbols provided by that module start with `ERH_`.  

`lexical_analyzer.h` has been renamed `LAH_lexical_analyzer.h` and all global symbols provided by the lexical analyzer now start with `LAH_`.  

`my_strdup.h` has been renamed `SSF_safe_string_functions.h` and all symbols now start with `SSF_`, additional functions such as `char* SSF_strcat(char* destination, char* source, size_t destination_size);` have been added.  

`unit_test_logging.h` has been renamed `UTL_unit_test_logging.h` with the corresponding name changes to the structs, functions and new enum that replaces the `char* path` variable in the struct.

Similar name changes have been made to at least 3 other files as well.

In response to the [answer by @G.Sliepen][3] 2 variadic function have been added, `void UTL_va_log_fprintf(const char* format, ...);` in `UTL_unit_test_logging.h` and `void ERH_va_report_error_fprintf(const char* format, ...);` in `ERH_error_reporting.h` to reduce the usage of `sprintf()` and any remaining `sprintf()` statements were converted to `snprintf()`.

The programs no longer depend on a `BUFSIZ` from `stdio.h` `ERH_error_reporting.h` provides the constant `ERH_ERROR_BUFFER_SIZE`.


  [1]: https://en.wikipedia.org/wiki/Single_responsibility_principle
  [2]: https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a/248599#248599
  [3]: https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a/248581#248581