// CRTP idiom: Base<Derived> calls back into Derived's own method through
// a static_cast, resolved entirely at compile time -- no virtual function,
// no vtable, and (with inlining) potentially no indirection at all.
#include <iostream>

template <typename Derived>
class Base {
public:
    void do_something() {
        std::cout << "Base::do_something\n";
        static_cast<Derived*>(this)->do_something_impl();
    }
};

class Foo : public Base<Foo> {
public:
    void do_something_impl() {
        std::cout << "Foo::do_something_impl\n";
    }
};

// Bar inherits the CRTP machinery but never defines do_something_impl();
// as long as nothing calls do_something() on a Bar, this compiles fine --
// the requirement is only enforced at the point of use, like a duck-typed
// interface.
class Bar : public Base<Bar> {};

int main() {
    Foo foo;
    foo.do_something();

    Base<Foo>().do_something();

    return 0;
}
