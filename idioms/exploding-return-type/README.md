# Exploding Return Type

## Intent

Return either a valid value or an error from a function while keeping the
call site's ordinary-looking assignment syntax — the error only "explodes"
(throws) at the point the caller actually tries to use the value.

## Motivation

Before `std::expected` (C++23) or hand-rolled `Result<T, E>` types, one way
to combine "give me a T, or tell me why not" without changing every call
site to check a boolean is a wrapper that is implicitly convertible to `T`.
Converting a failed instance is where the exception is thrown, so success
paths stay exactly as terse as returning `T` directly.

## Solution

`exploding_return<T>` stores either a `T` or an error string, and defines
`operator T()`. A normal-looking `T x = f();` triggers that conversion
operator; if the wrapper holds an error, the conversion throws instead of
producing a `T`.

## Known Uses

- Legacy-friendly error signaling that must interoperate with code written
  against plain `T`-returning APIs.
- A stepping stone when migrating an exception-free API towards
  `std::expected`/`tl::expected`-style explicit result types.

## Related Idioms

- `std::optional<T>` / `std::expected<T, E>` are the modern, explicit
  alternatives that avoid the implicit, easy-to-forget conversion this
  idiom relies on.

## References

- [More C++ Idioms - Exploding Return Type](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Exploding_Return_Type)
