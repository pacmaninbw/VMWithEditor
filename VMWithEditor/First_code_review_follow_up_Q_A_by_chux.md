[Answer][1] to first follow up question by user chux:

`mystrdup()` has a short-coming: to be *nix-like, I'd expect detecting cases that may set `errno`.

IMO, use `malloc()` and copy the _null character_ too.

From [When is it a good idea to use strdup (vs malloc / strcpy)](https://stackoverflow.com/a/39694254/2410359)

    #include <errno.h>
    #include <stdlib.h>
    
    char *mystrdup(const char *s) {
      // Optional test, s should point to a string
      if (s == NULL) { 
        #ifdef EINVAL
          // For systems that support this "invalid argument" errno
          errno = EINVAL;
        #ednif
        return NULL;  
      }
      size_t siz = strlen(s) + 1;
      char *y = malloc(siz);
      if (y != NULL) {
        memcpy(y, s, siz);
      } else {
        #ifdef ENOMEM
          // For systems that support this "out-of-memory" errno
          errno = ENOMEM;
        #else
          ;
        #endif
      }
      return y;
    }

[1]: https://codereview.stackexchange.com/questions/248817/common-unit-testing-code-follow-up/249103#249103