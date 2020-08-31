Small review.

**Minor**

Simplify.  Instead of 

    unsigned char* input = (unsigned char*)"0123456789ABCDEFXabcdefx";  // size is currently 24
    #define MAX_INPUT_CHARACTERS    24
    State_Transition_Characters expected_transition[MAX_INPUT_CHARACTERS];
    ...
    #undef MAX_INPUT_CHARACTERS

Consider

    unsigned char input[] = "0123456789ABCDEFXabcdefx";
    State_Transition_Characters expected_transition[sizeof input - 1];

**Is the code readable?**

I'd make more use of pointers to `const` to help convey the idea referenced data does not change.

The above review can be found on the [stackexchange code review websites](https://codereview.stackexchange.com/questions/248560/hand-coded-state-driven-lexical-analyzer-in-c-with-unit-test-part-b/248604#248604).