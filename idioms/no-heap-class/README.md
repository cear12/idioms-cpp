# No-Heap Class

## Intent

Force objects of a class to live only on the stack (or as subobjects of
other objects), by making it a compile-time error to `new` them onto the
heap.

## Motivation

Some types are meant to be short-lived, scope-bound guards or wrappers
(locks, RAII markers) where heap allocation would be a misuse -- forgetting
to `delete` a heap-allocated guard defeats its entire purpose. Rather than
relying on a code-review convention, the class itself can refuse to be
heap-allocated, turning a misuse into a compile error.

## Solution

Declare (but do not necessarily define) `operator new` and
`operator new[]` as `protected` (or `private`) members. Any attempt to
`new NoHeap` or `new NoHeap[]` from outside the class becomes a compile
error (inaccessible member), because those forms of `new`-expression are
required to name an accessible `operator new`.

## Notes

This blocks *direct* heap allocation of the type, but not every route to
heap-adjacent lifetime: `std::make_shared<NoHeap>()` still calls
`operator new` on `NoHeap`'s behalf (also blocked); embedding a `NoHeap`
as a member or base of a type that is itself heap-allocated is unaffected
(a `NoHeap` subobject still lives on the stack relative to nothing -- it
lives wherever its enclosing object lives, heap included), since only
allocating a free-standing `NoHeap` is what's actually restricted.

## Known Uses

- Scope guards, RAII lock wrappers, and other types whose entire
  contract depends on deterministic, stack-bound lifetime.

## References

- [More C++ Idioms - No-Throw](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms) (related family of "restrict how this type may be used" idioms)
