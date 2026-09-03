# Universal Pointer

## Intent

Provide one pointer-like type that can hold either an owning pointer (it
destroys what it points to) or a non-owning, observing pointer (it never
destroys anything), so APIs that don't care which one they were handed
can accept both through a single parameter type.

## Motivation

Some APIs are agnostic to ownership -- a function that just wants to look
at a `Widget` doesn't care whether the caller owns it via `unique_ptr`, or
is merely observing one owned elsewhere. Requiring callers to pick a
specific smart pointer type (or a raw pointer) forces every caller onto
one convention. A "universal" pointer accepts a raw pointer or reference
(and observes, without taking ownership) or a `unique_ptr` (and takes over
its ownership), behind one type.

## Solution

Implemented on top of `std::shared_ptr<T>`'s type-erased deleter:
constructing from a raw pointer/reference installs a no-op deleter (so
destroying the `UniversalPtr` never destroys the pointee); constructing
from a `unique_ptr<T, D>` installs a deleter that captures and invokes the
unique_ptr's own deleter `D`, and releases the `unique_ptr` so only the
new `UniversalPtr` is responsible for destruction. An explicit `Owns()`
flag records which case applies, since it's not otherwise observable from
outside.

## Known Uses

- Interfaces meant to accept "a `T`, I don't care who owns it" -- plugin
  and callback registration APIs are a common case.

## Related Idioms

- This is the same core technique (`shared_ptr` + a possibly-no-op
  deleter) `std::shared_ptr`'s *aliasing constructor* and non-owning
  "observer" wrapper patterns use.

## References

- [cppreference - std::shared_ptr deleters](https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr)
