# ZC Exercise 2 Solution

Knowing no context of how the code is being used, I've decided to put on the caller the responsibility to ensure that:
- the lifetimes are OK (New lives as long as the list to which it is being inserted)
- New is not a part of a list already. This is to avoid issue with "what to do with the New's old list?"
- Prev and New are not null pointers (the meaning of "insert" in this case would become very ambiguous)

In the implementation I've included assertion guards against the null pointers in wrong places, these can be removed – depending on what is the approach to null arguments in the code base, and what do we agree is the most convenient behaviour for this particular function.

Also included is one test in `test.c`.

## Build
Require CMake.

Build with
```
cmake .
make
make test
```

Clean with `make clean`.