Copy of [this code review](https://codereview.stackexchange.com/questions/248561/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-c/248611#248611) for the record.

# `typedef struct` can reuse the name of struct

I see you often do something like:

```
typedef struct foo_bar {
    ...
} Foo_Bar;
```

It's a bit weird to use lower_case for the struct name and Upper_Case for the typedef. You can reuse the same name as the struct:

```
typedef struct foo_bar {
    ...
} foo_bar;
```

It's also common to append `_t` to the typedef'ed name so it is easier to identify it as a type name instead of a variable or function name, although the `_t` suffix is reserved by at least POSIX 1003.1.

# No need to use `extern` for function declarations

The keyword `extern` is only necessary to declare variables without defining them, for function declarations there is no need, you can write for example the following in a header file:

```
bool init_vm_error_reporting(char* error_log_file_name);
```

# Use `const` where appropriate

It seems like you avoided using `const` everywhere. Using it might allow the compiler to better optimize your code, and it will be able to report an error if you ever accidentily do write to a variable that shouldn't be changed. So for example:

```
bool init_vm_error_reporting(const char* error_log_file_name);
```

You can also use it for struct members:

```
typedef struct test_log_data
{
    const char* function_name;
    bool status;
    const char* path;
    bool stand_alone;
} test_log_data;
```

# Optimize struct layout

The C standard mandates that the members of a struct appear in the same order in memory as they are declared. But this can result into gaps because of alignment restrictions. The above struct can be better layed out as follows:

```
typedef struct test_log_data
{
    const char* function_name;
    const char* path;
    bool status;
    bool stand_alone;
} test_log_data;
```

This saves 8 bytes on 64-bit architectures. In this particular case, it probably won't have a significant impact, but if structs get larger or you use a lot of them, you will reduce the amount of memory (bandwidth) used, and will less likely cause cache misses.

# You can close `stderr` and `stdout`

It is perfectly fine to call `fclose(stdout)` and `fclose(stderr)`, so the checks in `disengage_error_reporting()` and `close_unit_tests()` are not necessary.

# Simplify `log_test_is_positive_path()`

It looks like you can replace this whole function with:

```
static bool log_test_is_positive_path(Test_Log_Data* log_data)
{
    return !strcasecmp(log_data, "POSITIVE");
}
```

Or if you can't use the POSIX [`strcasecmp()`][1] function, Windows provides [`_stricmp()`][2].

But maybe it is better to ensure the filename itself is always upper case, so you can just use `strcmp()`?

# Avoid spending too much code on error handling unrelated to the unit tests

When there is an error internally in the unit tests, like when allocating memory for some string, don't waste lots of lines of code producing nice error messages and exitting gracefully. I particular like the BSD functions like [`err()`][3] for this, but to stay within the C standard, I recommend handling errors using [`perror()`][4] and `abort()` like so:

```
test_log_data *create_and_init_test_log_data(const char* function_name, bool status, const char* path, bool stand_alone)
{
    test_log_data* log_data = calloc(1, sizeof(*log_data));
    if (!log_data)
        perror("calloc()"), abort();

    init_test_log_data(log_data, function_name, status, path, stand_alone);
    return log_data;
}
```


  [1]: https://linux.die.net/man/3/strcasecmp
  [2]: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stricmp-wcsicmp-mbsicmp-stricmp-l-wcsicmp-l-mbsicmp-l?view=vs-2019
  [3]: https://linux.die.net/man/3/err
  [4]: https://en.cppreference.com/w/c/io/perror