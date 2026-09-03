# nullptr Emulation (pre-C++11)

## Intent

Give `0`/`NULL` a distinct, type-safe null-pointer value in C++03, where
`NULL` is just `0` or `((void*)0)` and happily (and dangerously) converts
to `int`, `bool`, and every pointer type alike.

## Motivation

Before C++11 added `nullptr` as a real core-language keyword, `NULL` was
only a macro, almost always defined as the integer literal `0`. That made
`f(NULL)` ambiguous or silently wrong whenever `f` was overloaded on both
a pointer type and an integral type -- the call would bind to the integer
overload, not the pointer one, which is rarely what anyone intended.
Bjarne Stroustrup and Herb Sutter's proposal N2431 popularized a
library-only emulation: a global object of a special type that converts
implicitly to *any* pointer type (member or non-member) but refuses to
convert to any integral type, closing that hole without a language change.

## Solution

A class whose `operator T*()` and `operator T C::*()` conversion operators
are unconstrained templates (so it converts to any pointer type), and
which explicitly has no conversion to any integral type; a single
`const` global instance of that class stands in for a real null-pointer
literal.

## Historical note

This is presented here purely as a historical curiosity and a teaching
example of a well-designed implicit-conversion class. Real code targeting
C++11 or later should simply use the language keyword `nullptr` -- indeed,
`nullptr` is now reserved, so this emulation's original name for its
global object can no longer even be used (see `example.cpp`, which names
it `my_nullptr` instead).

## References

- [N2431 - A Name for the Null Pointer: nullptr](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf)
