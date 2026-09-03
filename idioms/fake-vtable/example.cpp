// Fake Vtable idiom: a hand-rolled single-entry "vtable" (a function
// pointer) plays the role a compiler-generated vtable would normally play,
// with a derived class overriding behavior by overwriting the pointer
// instead of overriding a virtual function.
#include <iostream>

struct FakeVtable {
    void (*foo_)();
};

void BaseFoo() {
    std::cout << "Base::foo\n";
}

void DerivedFoo() {
    std::cout << "Derived::foo\n";
}

class Base {
public:
    FakeVtable* vtable_;

    Base() {
        vtable_ = new FakeVtable{&BaseFoo};
    }

    void Foo() {
        vtable_->foo_(); // dispatches through the fake vtable, not a real virtual call
    }

    virtual ~Base() {
        delete vtable_;
    }
};

class Derived : public Base {
public:
    Derived() {
        vtable_->foo_ = &DerivedFoo; // "override" by rewriting the function pointer
    }
};

int main() {
    Base* obj = new Derived();
    obj->Foo(); // prints "Derived::foo"
    delete obj;
    return 0;
}
