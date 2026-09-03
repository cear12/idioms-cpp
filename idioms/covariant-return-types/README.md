# Covariant Return Types

## Intent

Let an overriding virtual function return a more derived pointer/reference
type than the function it overrides, without giving up virtual dispatch or
requiring a cast at the call site.

## Motivation

A naive `clone()` override normally has to return `Shape*`, forcing callers
to `static_cast` the result back down to `Circle*` even though the function
obviously returns a `Circle`. C++ allows an override's return type to differ
from the base's as long as it is a pointer or reference to a class publicly
and unambiguously derived from the base's return type — the "covariant"
relaxation of the otherwise-strict override-signature rule.

## Solution

Declare the base virtual function returning `Base*`; override it in the
derived class returning `Derived*`. The compiler checks the covariance rule
at the override, and any call made **through a pointer/reference of the
derived static type** gets the precise type back with no cast.

Note the emphasis: covariance narrows what the *override* returns, not what
every call expression is typed as. Calling `clone()` through a
`Shape*`/`unique_ptr<Shape>` that happens to point at a `Circle` still gives
a call expression of static type `Shape*` -- the compiler only knows the
static type of the pointer, not that it dynamically points to a `Circle` --
even though `Circle::clone()` is the one that actually runs at runtime and
returns a `Circle*` under the hood. `example.cpp` demonstrates both cases
side by side.

## Known Uses

- Prototype-pattern `clone()` methods, as in `example.cpp`.
- Fluent/builder APIs where a derived builder wants to return `Derived&`
  from a method it overrides that was declared to return `Base&`.

## References

- [More C++ Idioms - Covariant Return Types](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Covariant_Return_Types)
