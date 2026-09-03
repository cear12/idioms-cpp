# Shortening Long Template Names

## Intent

Make deeply-nested template instantiations readable and maintainable by
naming them, instead of spelling the full instantiation out at every use.

## Motivation

A type like `std::unordered_map<std::string, std::vector<std::function<void(const std::string&, int)>>>`
is unreadable and error-prone to repeat. C++ offers several tools of
increasing power for giving such types short, meaningful names.

## Solution

Four techniques, roughly in order of preference for modern C++:

1. **`typedef`** - the C++98 way of naming one specific instantiation.
2. **`using` alias** - a more readable equivalent of `typedef` for one
   specific instantiation (`using StringMap = ...;`).
3. **Alias templates** (`template<typename T> using Vector = std::vector<T>;`)
   - parameterized aliases that themselves take template arguments, with
   no C++98 equivalent.
4. **Local aliases** inside a class or function scope, keeping the
   shorthand's visibility limited to where it's actually used.

## Known Uses

Virtually all non-trivial template-heavy C++ code, from standard-library
implementations to application code using containers of containers of
callables.

## References

- [More C++ Idioms - Shortening Long Template Names](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Shortening_Long_Template_Type_Names)
