# Named External Argument

## Intent

Make call sites of a function with several same-typed or hard-to-remember
parameters self-documenting, without changing the language (C++ has no
built-in named-parameter syntax).

## Motivation

```cpp
configureWindow(640, 480, true, false, 5);
```

reads as five opaque numbers and booleans; getting the order wrong
compiles silently and produces a hard-to-spot bug. Wrapping each parameter
type in a small tag struct with a same-named factory function lets the
call site read almost like real named arguments:

```cpp
configureWindow(width(640), height(480), resizable(true), decorated(false), border(5));
```

## Solution

Each logical parameter gets its own tiny struct (`Width`, `Height`, ...)
and a lowercase factory function of the same name that constructs it. The
target function takes these wrapper types instead of raw `int`/`bool`, so
a call site names each argument by construction, and passing two
arguments in the wrong order is now a type error instead of a silent bug
(since e.g. `Width` and `Height` are different types, even though both
just wrap an `int`).

## Known Uses

- Any API with several parameters of the same or easily-confused types
  (dimensions, flags, IDs) where argument order mistakes are a real risk.

## Related Idioms

- Named Parameter Idiom (chained setters returning `*this`) achieves a
  similar readability goal with a different mechanism.

## References

- [More C++ Idioms - Named External Argument](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_External_Argument)
