# do/while(0) Macro

## Intent

Make a multi-statement function-like macro behave, syntactically, like a
single statement -- so it's safe to use anywhere a single statement is
expected, including as the unbraced body of an `if`.

## Motivation

A macro defined as a bare block,

```c
#define BAD_MACRO(a, b) { statement1(a); statement2(b); }
```

breaks when used as the body of an `if` without braces, followed by
`else`:

```c
if (cond)
    BAD_MACRO(x, y);
else
    other();
```

expands to

```c
if (cond)
    { statement1(x); statement2(y); };   // <- stray ';' is its own empty statement
else
    other();
```

The block plus the call site's trailing `;` count as *two* statements
after the `if`; only the block binds to it, so the `else` is left
dangling with no matching `if` -- a compile error.

## Solution

Wrap the macro body in `do { ... } while (0)` (no trailing semicolon in
the macro definition itself):

```c
#define GOOD_MACRO(a, b) do { statement1(a); statement2(b); } while (0)
```

`do ... while(0)` is a single statement syntactically, so
`GOOD_MACRO(x, y);` -- semicolon included -- is exactly one statement, and
plugs safely into an unbraced `if`/`else`, a loop body, or anywhere else a
single statement is expected. The `while(0)` condition means the loop body
still only ever executes once.

## Known Uses

Virtually every multi-statement function-like macro in widely used C/C++
codebases (glibc, the Linux kernel, etc.) uses this pattern.

## References

- [More C++ Idioms - Reference](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Reference)
- [C++ FAQ - Why should I use do/while(0) instead of just {}?](https://isocpp.org/wiki/faq/misc-technical-issues#macros-with-multi-stmts)
