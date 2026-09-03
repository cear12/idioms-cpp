# SFINAE (Substitution Failure Is Not An Error)

## Intent

Select between alternative function or class template overloads at
compile time, based on whether a particular expression is well-formed for
a given type -- without that ill-formed expression causing a hard compile
error.

## Motivation

When the compiler substitutes a candidate template's arguments during
overload resolution, an invalid substitution in the *immediate context* of
that candidate (an ill-formed type or expression appearing directly in its
signature) silently removes that candidate from the overload set instead
of failing the whole compilation. This lets library code branch on
"does this type support operation X" at compile time, which is the
mechanism `std::enable_if` and countless type-trait libraries are built on.

## Solution

`test_serialize<T>(int)` is only a viable overload if
`std::declval<T>().serialize()` is well-formed; if it's not, substitution
fails for that overload (not an error) and the `test_serialize<T>(...)`
fallback (which always matches) is selected instead. Wrapping the result
in `std::true_type`/`std::false_type` turns "does this compile" into an
ordinary compile-time boolean, usable with `std::enable_if`.

## Known Uses

- Nearly every type-trait in `<type_traits>` is implemented with some form
  of SFINAE (or, in C++20, could be replaced by an equivalent `concept`).
- Detecting whether a type has a particular member function/typedef before
  choosing which code path to instantiate for it.

## Related Idioms

- `if constexpr` and Concepts (C++20) supersede many uses of hand-rolled
  SFINAE with more readable syntax, but SFINAE remains the underlying
  mechanism (and the only option before C++20 / without `if constexpr`
  for C++17).

## References

- [cppreference - SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)
