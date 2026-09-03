# Overload Set for std::visit

## Intent

Build an ad-hoc visitor for `std::variant` out of a set of lambdas, one per
alternative, instead of writing a named visitor class or a chain of
`if (std::holds_alternative<T>(...))` checks.

## Motivation

`std::visit` needs a single callable whose `operator()` is overloaded for
every alternative in the variant. Writing a dedicated struct for every
one-off visit is heavyweight for what's often a handful of short lambdas.
The `overload` helper class-template aggregates any number of lambdas into
one type that inherits all of their `operator()`s via `using Ts::operator()...`,
so a `std::visit` call site can just list the lambdas inline.

## Solution

```cpp
template<class... Ts> struct Overload : Ts... { using Ts::operator()...; };
template<class... Ts> Overload(Ts...) -> Overload<Ts...>; // CTAD guide
```

Each lambda becomes a base class contributing one `operator()` overload;
class template argument deduction (the deduction guide) lets callers write
`Overload{lambda1, lambda2, ...}` without spelling out the template
arguments.

## Known Uses

- Exhaustive, type-safe handling of every alternative of a
  `std::variant`-based sum type (a common source of variant use is parsing
  or event handling, where each alternative needs different logic).

## References

- [cppreference - std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)
- This exact `overload` helper appears in the cppreference example for
  `std::visit` and is sometimes called "the overload pattern".
