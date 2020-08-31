# Answers to your questions

> I learned C a long time ago from K&R “The C Programming Language” Version 1 (pre C89/C90).

I started with K&R C's second revision, but that doesn't mean I didn't keep up with the changes over time. C99 brought many useful improvements that I happily use every day. Your code looks C99 as well, since you are using `bool` and `//` comments.

> 1. Other than compiling this `–O3` what can I do to optimize this code?

Try to do as much as possible at compile time instead of run time. For example, instead of having `get_or_create_next_states()`, it seems to me you can create a static array, like so:

```
static Syntax_State_Transition next_states[] = {
    [START_STATE] = {START_STATE, {ENTER_OPCODE_STATE, ERROR_STATE, ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, START_STATE, DONE_STATE, ERROR_STATE}},
    [ENTER_OPCODE_STATE] = {...},
    ...
};
```

The above uses C99 designated initializers. If you don't want to use C99, you can omit the designations, but then you have to remember the correct order.

> 2. Are there any features in the more modern versions of C that could reduce the amount of code? There are currently more 1300 lines of commented code to test the 376 lines of commented code in lexical_analyzer.c and lexical_analyzer.h.

There are a some things that could reduce a few lines of code. For example, when logging messages, you write:

```
sprintf(buffer, "\nSome message, %s\n\n", some_variable);
log_generic_message(buffer);
```

Apart from `sprintf()` being unsafe, you can make `log_generic_message()` a [variadic function][1] that takes a format strings and a variable number of arguments, like so:

```
void log_generic_message(const char *format, ...)
{
    char buffer[...];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);

    ...
}
```

This way, you can just write:

```
log_generic_message("\nSome message, %s\n\n", some_variable);
```

You can also use [`__attribute__((format(...)))`][2] to tell the compiler that you expect a `printf`-like format string, and it can then give the same warnings it will give if you have mismatching conversion specifiers and arguments. Of course support for function attributes might vary between compilers and cannot be used portably, unless you add some checks for it and `#ifdef` it out when the compiler doesn't support it.

There's a `memset()` that can be replaced using an array initializer:

```
unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
```

Can be rewritten as:

```
unsigned syntax_check_list[SYNTAX_CHECK_COUNT] = {0};
```

> 3. Is there archaic C usage that is not customary to use anymore?

Not that I see.

> 4. Are the unit tests missing any test cases, especially edge cases?

I'm not sure.

> 5. Are there any memory leaks?

Not that I see.

> 6. Is the code readable?

Well, mostly. But I would personally have used a lexer generator like [flex][3], so I can write the lexer in a higher level language, and not have to deal with writing the code myself. Even though the language you are implementing is very simple, the lexer you wrote is already quite large, and if the language would get more complex, your lexer will quickly become unmaintainable, I'm afraid.

> 7. I don’t like the fact that I need to include the unit test files in lexical_analyzer.c do you see any way around this?

Yes, do it the other way around: in `internal_sytax_state_tests.c`, add `#include "lexical_analyzer.c"`. Alternatively, if you don't want to `#include` .c files into each other, then you have to find some way to remove the `static` from functions that you want to be able to unit test. A typical way to do that is:

```
#ifdef UNIT_TESTING
#define STATIC
#else
#define STATIC static
#endif

...

STATIC bool is_legal_in_hex_number(unsigned char input) {
    ...
}
```

Then when building the unit test, you can link the unit testing code with a version of `lexical_analyzer.c` built with `UNIT_TESTING` defined.

> 8. Is the language too complex?

The language is not complex at all, but as you see you already had to write a lot of code to parse it. That's why lexer and parser generators have been created.

# Use of `_strdup()`

The function `strdup()` is not in any C standard, but it is in POSIX.1-2001. As mentioned by @chux-ReinstateMonica, the C standard reserves identifiers starting with `str`, so Microsoft decided to not violate that rule and declare `_strdup()` instead. What I typically do in my own projects that need to be compatible with a certain standard, and where I want to use some commonly available convenience function that is not present in the standards I can safely use in my projects, is to add some check for the presence of the desired function, and if it's not present, either add an alias to a similar function or just write a drop-in replacement. So for example, you could write:

```
#ifndef HAVE_STRDUP
#ifdef HAVE__STRDUP
#define strdup(x) _strdup(x)
#else
static char *strdup(const char *x) {
    size_t len = strlen(x) + 1;
    char *s = malloc(len);
    if (s)
        memcpy(s, x, len);
    return s;
}
#endif
```

Then either have a build tool like autoconf figure out which functions are available and `#define HAVE_...` somewhere, or replace `#ifdef HAVE_STRDUP` by some other way to check for the availability of that function.

This code review was originally posted on the [stackexchange Code Review website](https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a/248581#248581).

  [1]: https://en.cppreference.com/w/c/variadic
  [2]: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-format-function-attribute
  [3]: https://en.wikipedia.org/wiki/Flex_lexical_analyser