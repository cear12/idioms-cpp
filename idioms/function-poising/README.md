# Function Poisoning

## Intent

Hide or disable calls to a deprecated or unsafe function, forcing callers
onto a safer replacement, and have the compiler enforce it rather than
relying on a comment or code review.

## Motivation

C and pre-C++11 libraries commonly expose factory functions returning raw
pointers (`T*`) that the caller must remember to release manually. Modern
C++ prefers RAII wrappers (`std::unique_ptr` with a custom deleter) for
automatic lifetime management. Simply documenting "please use the wrapper
instead" doesn't stop anyone from calling the raw function; poisoning the
symbol turns an accidental raw call into a compile error.

## Solution

1. Define a wrapper that calls the raw factory and immediately hands the
   result to an RAII type with an appropriate deleter.
2. Mark the raw factory function as "poisoned" with `#pragma GCC poison`,
   which makes any subsequent *use* of that identifier a compile error
   anywhere later in the translation unit -- including in code the poisoning
   header didn't itself write.

## Notes

`#pragma GCC poison` is a GCC/Clang extension, not standard C++; it has no
effect (and is silently ignored) on other compilers. This is why the
poisoning happens after the wrapper's own definition, which is the last
place the raw name is legitimately needed.

## Known Uses

- Forcing migration off deprecated or unsafe C APIs (`gets`, raw
  `malloc`/`free` pairing, legacy factory functions) onto RAII-friendly
  wrappers within one codebase.

## References

- [GCC pragmas - poison](https://gcc.gnu.org/onlinedocs/cpp/Pragmas.html)
