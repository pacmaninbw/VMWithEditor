`"%z..."`

Avoid UB.

Codes use [`"%zd"`](https://stackoverflow.com/q/32916575/2410359) with `size_t` and `unsigned`.

Use `"%zu"` with `size_t` and `"%u"` with `unsigned`.

**Name space**

(Is the code readable?) `lexical_analyzer.h` introduces types and macros such as `SYNTAX_STATE_MACHINE_H`, `COMMA`, `State_Transition_Characters`, `MAX_OPCODE`, in a inconsistent manner. 

Name collision avoidance is difficult as naming covers too many naming styles.

Consider a common prefix for all, perhaps `lapac_` in `lapac.h`.

**Take care with failed data**

Avoid UB. `report_lexical_analyzer_test_failure()`  

When things fail, avoid assuming too much about _string_ data.   

I recommend printing _string_ with sentinels such as `"<"`, `">"` for clarity as to the start/end of a string which may include _white space_..

Take better string length care by using `snprintf()`, etc., than hoping `char out_buffer[BUFSIZ];` is big enough.

**Simplify verbose code**

(Are there any features in the more modern versions of C that could reduce the amount of code?)

    // Instead of 22 line original, avoid locale dependencies and shorten.
    static bool is_legal_in_hex_number(unsigned char input) {
      return (isxdigit(input) && !isdigit(input)) || (input == 'x' || input == 'X');
    }

It is unclear to me why original `is_legal_in_hex_number(some_0_to_9_digit)` returns `false`.

**Minor**

`state_machine_unit_test_main.h` should include `<stdbool.h>`

`()` around macro equations..

    // #define SYNTAX_STATE_ARRAY_SIZE    9 + 1
    #define SYNTAX_STATE_ARRAY_SIZE    (9 + 1)


The above code review can be found on the [stackexchange Code Review website](https://codereview.stackexchange.com/questions/248559/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-a/248599#248599).