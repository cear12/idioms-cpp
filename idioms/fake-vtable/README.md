# Fake Vtable

## Intent

Implement single-dispatch, overridable behavior manually with a
function-pointer member, instead of relying on the compiler-generated
virtual table.

## Motivation

A real virtual function call goes through the compiler's vtable: an
indirect call whose target is looked up through a pointer stored in every
polymorphic object. In most code that overhead is irrelevant, but in
tightly constrained environments -- or when a class needs to change its
"override" at runtime rather than at construction, or needs precise
control over ABI layout -- implementing the same one level of indirection
by hand with a function pointer can be useful.

## Solution

Store a function pointer (a "fake vtable" of one entry) in the base class
and initialize it in the base constructor; a derived class overrides the
behavior by overwriting that pointer in its own constructor rather than by
overriding a virtual function.

## Notes

This is a hand-rolled, much more limited version of what the compiler
already does for you with `virtual`. It's shown here for what it teaches
about how virtual dispatch works under the hood, not as something to
prefer over `virtual` in ordinary code: real vtables are backed by the
ABI, support multiple overridable methods and multiple inheritance
correctly, and don't require manually managing the pointer's lifetime.

## When It Can Be Useful

High-performance or embedded systems wanting explicit control over
dispatch and memory layout, or code that needs to swap an object's
behavior at runtime rather than fix it at construction (real `virtual`
dispatch is fixed once the object's dynamic type is set).

## References

- [More C++ Idioms - Fake Vtable](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Fake_Vtable)
