// CRTP idiom: Base<Derived> calls back into Derived's own method through
// a static_cast, resolved entirely at compile time -- no virtual function,
// no vtable, and (with inlining) potentially no indirection at all.
#include <iostream>

template <typename Derived>
class Base {
 public:
  void DoSomething() {
    std::cout << "Base::do_something\n";
    static_cast<Derived*>(this)->DoSomethingImpl();
  }
};

class Foo : public Base<Foo> {
 public:
  void DoSomethingImpl() { std::cout << "Foo::do_something_impl\n"; }
};

// Bar inherits the CRTP machinery but never defines DoSomethingImpl();
// as long as nothing calls DoSomething() on a Bar, this compiles fine --
// the requirement is only enforced at the point of use, like a duck-typed
// interface.
class Bar : public Base<Bar> {};

int main() {
  Foo foo;
  foo.DoSomething();

  Base<Foo>().DoSomething();

  return 0;
}
