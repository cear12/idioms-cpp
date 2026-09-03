# Construction Tracker (function-try-block idiom)

## Intent

Know exactly which member finished initializing last when a constructor's
member-initializer list throws, using a function-try-block instead of
scattering manual flags through the class.

## Motivation

If a constructor's initializer list throws partway through, the members
that already finished constructing are destroyed automatically, but by
default the *catch* block (if any) doesn't know which member was being
initialized when the exception was thrown -- useful information for
logging or diagnostics. Threading a side-effect through each member's own
constructor call records that information as a byproduct of construction
order, which is fixed and well-defined (declaration order), regardless of
initializer-list order.

## Solution

A `try`/`catch` wrapped around the entire constructor body (a
"function-try-block") can catch exceptions from the initializer list, not
just the body. Combined with each tracked member's constructor updating a
shared `tracker` variable as its *last* action (via the comma operator in
the initializer expression), the `catch` block can report the last member
that started initializing when the exception propagated.

## Known Uses

- Diagnosing which subobject failed to construct in classes with several
  fallible members, without changing every member's own type to carry
  diagnostic state.

## References

- [More C++ Idioms - Construction Tracker](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Construction_Tracker)
- [cppreference - function-try-block](https://en.cppreference.com/w/cpp/language/function-try-block)
