// do/while(0) Macro idiom: wrapping a multi-statement macro body in
// do{...}while(0) makes the whole macro call behave like one statement,
// so it plugs safely into an unbraced if/else. See README.md for the
// broken alternative this avoids (a bare {...} block plus a trailing ';'
// leaves a dangling else) -- it isn't compiled here on purpose, since it
// doesn't compile.
#include <iostream>

#define LOG_PAIR(a, b)               \
    do {                             \
        std::cout << "a=" << (a);    \
        std::cout << " b=" << (b);   \
        std::cout << "\n";           \
    } while (0)

int main() {
    bool verbose = true;

    // LOG_PAIR(1, 2); expands to a single statement, so this unbraced
    // if/else is well-formed -- it would NOT be if LOG_PAIR's body were a
    // bare {...} block instead of do{...}while(0).
    if (verbose)
        LOG_PAIR(1, 2);
    else
        std::cout << "quiet\n";

    for (int i = 0; i < 3; ++i)
        LOG_PAIR(i, i * i);

    return 0;
}
