# Type Erasure

## Intent

Store values of any (copyable) type behind a single non-template type,
while still being able to recover the original type safely later.

## Motivation

Templates give compile-time polymorphism, virtual functions give runtime
polymorphism for types that share a common base — but sometimes values of
genuinely unrelated types need to live in the same container (a
heterogeneous vector, an event payload, ...). Type erasure hides each
value's concrete type behind a common non-template interface at the point
it's stored, while keeping enough information (`type_info`) to safely
recover it later.

## Solution

An abstract `IHolder` interface plus a templated `Holder<T> : IHolder` that
actually stores the `T`. The outer `Any` class holds a
`unique_ptr<IHolder>` and exposes a templated `cast<T>()` that checks the
stored `type_info` before downcasting — this is essentially a minimal,
educational reimplementation of `std::any`.

## Known Uses

- `std::any`, `std::function` (erases any callable with a given signature),
  `std::shared_ptr`'s deleter (erases any deleter type).
- Heterogeneous containers, plugin/event systems where the payload type
  isn't known until runtime.

## Related Idioms

- Fake Vtable / hand-rolled vtables use a related technique to erase type
  while avoiding real virtual dispatch.

## References

- [More C++ Idioms - Type Erasure](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure)
- [cppreference - std::any](https://en.cppreference.com/w/cpp/utility/any)
