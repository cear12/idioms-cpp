# The result_of Technique

## Intent

Determine the return type of an arbitrary callable at compile time, so
generic code can declare "returns whatever `F(Args...)` returns" without
knowing `F` in advance.

## Motivation

Before `decltype` (C++11) and trailing return types, there was no direct
way to name "the return type of calling `f` with these arguments" in a
function template's signature. `std::result_of<F(Args...)>::type` filled
that gap and was, for years, the standard way to write fully generic
wrappers, task queues, and metaprogramming utilities. It's been superseded
but still shows up in older code and libraries.

## Solution

`std::result_of<F(Args...)>::type` (note the unusual syntax: a function
*type* `F(Args...)` as the single template argument) yields the type
`f(args...)` would produce. C++17 deprecated it in favor of the simpler
`std::invoke_result_t<F, Args...>` (ordinary comma-separated template
arguments, and usable with `std::invoke`'s broader notion of "callable",
including pointers to member functions). C++20 removed `result_of` and
`is_callable`-via-`result_of` altogether.

## Known Uses

- Generic wrappers that log, time, or otherwise instrument a call while
  perfectly forwarding its return type.
- Compile-time "is this callable with these arguments" traits.
- Task/thread-pool `submit()` functions that must deduce a `std::future<R>`
  from an arbitrary callable, e.g. via `std::packaged_task`.

## Related Idioms

- `decltype` / trailing return types are the direct, non-metafunction way
  to express the same thing when the calling expression can be written out.

## References

- [cppreference - std::result_of (deprecated in C++17, removed in C++20)](https://en.cppreference.com/w/cpp/types/result_of)
- [cppreference - std::invoke_result](https://en.cppreference.com/w/cpp/types/result_of)
