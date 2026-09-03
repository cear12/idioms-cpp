# Curiously Recurring Template Pattern (CRTP)

## Intent

Give a base class access to the interface of the derived class that
inherits from it, resolved entirely at compile time -- "static
polymorphism" -- by having the derived class pass itself as the base's
template argument.

## Motivation

Ordinary (dynamic) polymorphism uses virtual functions and pays for an
indirect call through a vtable. When the concrete derived type is known at
compile time, CRTP achieves a similar "base calls into derived" shape
without any virtual function or vtable: `Base<Derived>` can
`static_cast<Derived*>(this)` and call a method it trusts `Derived` to
provide, and the compiler can inline the whole chain.

## Solution

```cpp
template <typename Derived>
class Base {
public:
    void do_something() { static_cast<Derived*>(this)->do_something_impl(); }
};

class Foo : public Base<Foo> {
public:
    void do_something_impl() { /* ... */ }
};
```

`Foo` inherits from `Base<Foo>` -- the class "recurs" through its own
name as the base's template argument, giving the pattern its name.

## Known Uses

- Compile-time (non-virtual) interfaces, e.g. much of the standard
  library's iterator adaptor machinery and libraries like Eigen.
- Mixins that need to call back into the most-derived type, e.g.
  `enable_shared_from_this` in spirit (though its actual implementation
  differs).
- Policy-Based Design and Hierarchy Generation frequently implement
  individual policies as CRTP bases.

## References

- [More C++ Idioms - CRTP](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Curiously_Recurring_Template_Pattern)
