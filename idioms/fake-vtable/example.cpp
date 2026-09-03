// Fake Vtable idiom: a hand-rolled single-entry "vtable" (a function
// pointer) plays the role a compiler-generated vtable would normally play,
// with a derived class overriding behavior by overwriting the pointer
// instead of overriding a virtual function.
#include <iostream>

struct FakeVtable {
    void (*foo)();
};

void BaseFoo() {
    std::cout << "Base::foo\n";
}

void DerivedFoo() {
    std::cout << "Derived::foo\n";
}

class Base {
public:
    FakeVtable* vtable;

    Base() {
        vtable = new FakeVtable{&BaseFoo};
    }

    void foo() {
        vtable->foo(); // dispatches through the fake vtable, not a real virtual call
    }

    virtual ~Base() {
        delete vtable;
    }
};

class Derived : public Base {
public:
    Derived() {
        vtable->foo = &DerivedFoo; // "override" by rewriting the function pointer
    }
};

int main() {
    Base* obj = new Derived();
    obj->foo(); // prints "Derived::foo"
    delete obj;
    return 0;
}
