# Downcasting Techniques

## Intent

Safely convert a base-class pointer/reference back to a specific derived
type, comparing the range of standard and hand-rolled techniques for doing
so and their trade-offs.

## Motivation

`static_cast` down a class hierarchy is unchecked -- if the object isn't
actually of the target type, it's undefined behavior. `dynamic_cast` is
checked and safe, but requires RTTI (which some environments disable for
size or determinism reasons) and has runtime cost proportional to
hierarchy depth. Between (and around) those two extremes are several
idioms that trade some of `dynamic_cast`'s convenience for independence
from RTTI.

## The techniques

1. **`dynamic_cast`.** No special class design needed; requires RTTI.
2. **Virtual "is-a" function.** `Base::IsDerived()` returns null by
   default, `Derived::IsDerived()` returns `this`. No RTTI, easy to get
   right, but needs one virtual function per class that can be a downcast
   target.
3. **Type tags + `static_cast`.** Each class stores an enum tag set by its
   constructor; `down_cast<Target>(ptr)` compares `ptr->type()` against
   `Target::TAG` before casting. No RTTI, but the tag enum must be
   extended for every new class, which doesn't scale well and is easy to
   get out of sync.
4. **Manual RTTI via per-type virtual accessors.** `Base` declares one
   `asX()` virtual function per known derived type, defaulted to return
   `nullptr`; each `Derived::asX()` returns `this`. See the sibling
   `manual-rtti` idiom for a different (open, single-accessor) style of
   hand-rolled RTTI, and for a fuller discussion of when giving up
   `dynamic_cast` is worth it.
5. **A `constexpr`-branching `as_cast<To, From>`.** Uses
   `dynamic_cast` when the source type is actually polymorphic, falls back
   to `static_cast` when it's known statically convertible, and yields
   `nullptr` for anything else -- chosen entirely at compile time via
   `if constexpr`.

## References

- [More C++ Idioms - Type Safe Enum](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Safe_Enum)
- [cppreference - dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast)
