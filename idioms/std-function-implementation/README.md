# std::function, Reimplemented

## Intent

Understand how `std::function` erases the type of an arbitrary callable
(lambda, function pointer, functor) behind one concrete, non-template
type, by building a minimal version of the same mechanism.

## Motivation

`std::function<R(Args...)>` can hold a lambda, a plain function, or a
functor with any internal representation, yet is itself one fixed,
non-template type. That's Type Erasure (see the sibling `type-erasure`
idiom) specialized to callables: an abstract interface describing "can be
invoked with `Args...`, returns `R`", and a template implementation of
that interface, one instantiation per concrete callable type actually
stored.

## Solution

`ICallable` declares a pure virtual `invoke(Args...)`. `CallableImpl<F>`
holds an actual `F` and implements `invoke` by calling it. `Function<R(Args...)>`
owns a `unique_ptr<ICallable>` and, in its templated constructor, wraps
whatever callable it's given in a `CallableImpl<F>` -- the concrete
callable's type `F` only exists in that one constructor template
instantiation, and is invisible to everything else, including
`Function`'s own `operator()`.

## Known Uses

`std::function` itself; the same interface-plus-templated-implementation
shape reappears anywhere a fixed type needs to store "any T that satisfies
this interface" (see also: `type-erasure`).

## Related Idioms

- Type Erasure (the same mechanism, generalized beyond callables).

## References

- [cppreference - std::function](https://en.cppreference.com/w/cpp/utility/functional/function)
