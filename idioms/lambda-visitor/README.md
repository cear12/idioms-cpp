# Lambda Visitor

## Intent

Build a single reusable callable out of several lambdas, one per case, to
serve as a `std::visit` visitor -- generated through a factory function
rather than the `overload` aggregate-of-base-classes technique.

## Motivation

This is a close relative of the `overload-visit-variant` idiom, but built
differently: instead of composing an aggregate type that inherits from
each lambda's closure type, `make_lambda` returns a single generic lambda
that folds a call across every one of its captured lambdas using a fold
expression, calling whichever one accepts the argument's actual type.

## Solution

```cpp
template<typename... Lambdas>
auto make_lambda(Lambdas... lambdas) {
    return [=](auto&& arg) {
        auto tryOne = [&](auto&& lambda) {
            if constexpr (std::is_invocable_v<decltype(lambda), decltype(arg)>) {
                lambda(arg);
            }
        };
        (tryOne(lambdas), ...); // unary right fold over the comma operator
    };
}
```

A naive fold `(lambdas(arg), ...)` (no `if constexpr` guard) calls *every*
lambda in the pack unconditionally with the same `arg` -- which only
compiles if every single lambda accepts every alternative's type, which
defeats the point of a per-case visitor and is a common mistake when first
writing this idiom. Guarding each call with `if constexpr` +
`std::is_invocable_v` discards the non-matching calls from instantiation
entirely, so only the lambda whose parameter type actually matches `arg`
is compiled and invoked.

## Notes

With the `if constexpr` guard, this behaves like `overload` + `std::visit`
for the case that actually matches -- but it is worth noticing that this
version does its own type-based dispatch inside the returned lambda,
whereas `overload` relies on ordinary C++ overload resolution (via
`using Ts::operator()...`) to do the same job. Both are shown so the two
mechanisms can be compared directly against `overload-visit-variant`.

## Related Idioms

- Overload Set for `std::visit` (`overload-visit-variant`) -- the more
  common way to write an inline `std::visit` visitor.

## References

- [cppreference - fold expressions](https://en.cppreference.com/w/cpp/language/fold)
