# Manual RTTI

## Intent

Identify an object's concrete dynamic type -- for logging, dispatch, or
safe downcasting -- without relying on the compiler's built-in RTTI
(`typeid`/`dynamic_cast`), by having each class report its own type
information through an ordinary virtual function.

## Motivation

Compiler RTTI is sometimes unavailable (`-fno-rtti`, common in embedded
and game-engine codebases for code-size and determinism reasons) or
simply undesired for a small, closed set of types where a hand-rolled
alternative is cheaper and more explicit. Manual RTTI reimplements the
part of RTTI a given codebase actually needs -- usually just "what kind of
object is this" and "give me a human-readable name for it" -- as ordinary
virtual functions, with no compiler magic involved.

## Solution

A `Shape` base declares `virtual ShapeKind kind() const` (and, if wanted,
a `className()` for logging) that each derived class overrides to report
its own tag. A small trait, `KindOf<T>`, maps each concrete C++ type back
to its tag, so a generic `is_a<T>(shape)` and `manual_cast<T>(ptr)` can be
written once instead of duplicating tag comparisons at every call site.

## Compared to `downcast`'s "manual RTTI" section

The sibling `downcast` idiom's technique 4 covers a *narrower* case of the
same general idea: one virtual `asX()` accessor per potential downcast
target, useful specifically for casting. This idiom covers the more
general case -- a type tag usable for logging, equality-by-dynamic-type,
serialization dispatch, or anything else that wants to know "what is
this", not just casting.

## Trade-offs

Every new concrete type must remember to implement `kind()`/`className()`
and get a `KindOf<T>` specialization -- there's no compiler enforcement
tying them together the way real RTTI is automatically correct for every
class. In exchange: no RTTI dependency, predictable performance (a virtual
call, not a runtime type comparison walk), and full control over what
"identity" means for these types.

## References

- [More C++ Idioms - Manual RTTI type discussions](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)
- [cppreference - typeid / RTTI](https://en.cppreference.com/w/cpp/language/typeid)
