# Implicit Conversions

## Intent

Show how non-`explicit` converting constructors and conversion operators
let user-defined types participate in implicit conversions the same way
built-in types do — and how `explicit` selectively suppresses that.

## Motivation

A type with a single-argument, non-`explicit` constructor is implicitly
convertible *from* its argument type; a type with a non-`explicit`
`operator T()` is implicitly convertible *to* `T`. Both are useful (they
let user types plug into existing APIs), but both can also silently permit
conversions the author never intended. Comparing an implicit chain against
an `explicit`-guarded one side by side makes the trade-off concrete.

## Solution

`A` converts implicitly both ways (`std::string -> A -> int`), so passing
a string literal to a function expecting `int` compiles via two chained
user-defined-adjacent conversions. `B`'s constructor is marked `explicit`,
so the same kind of call is rejected at compile time and must be spelled
out with `static_cast`.

## Known Uses

- Wrapper/"strong typedef" types that intentionally interoperate with a
  primitive type (implicit) versus ones that must not be confused with it
  (explicit), e.g. distinguishing `Meters` from `Feet`.

## References

- [cppreference - converting constructors](https://en.cppreference.com/w/cpp/language/converting_constructor)
- [cppreference - explicit specifier](https://en.cppreference.com/w/cpp/language/explicit)
