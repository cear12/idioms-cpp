// No-Heap Class idiom: making operator new/new[] protected turns an
// attempt to heap-allocate NoHeap from outside the class into a compile
// error, forcing it to live only on the stack or as a subobject.
#include <iostream>

class NoHeap {
protected:
    static void* operator new(std::size_t);    // declared, deliberately not defined:
    static void* operator new[](std::size_t);  // never meant to be called

public:
    void hello() const { std::cout << "NoHeap on the stack\n"; }
};

class NoHeapTwo : public NoHeap {
    // Inherits NoHeap's protected operator new/new[], so NoHeapTwo is
    // equally restricted without repeating anything.
};

int main() {
    NoHeap a;         // fine: stack allocation
    NoHeapTwo b;       // fine: stack allocation, inherited restriction
    a.hello();
    b.hello();

    // Every one of the following is a compile error (operator new is
    // protected/inaccessible here), which is the entire point:
    //
    //   NoHeap* p1 = new NoHeap;         // error: 'static void* NoHeap::operator new(...)' is protected
    //   NoHeap* p2 = new NoHeap[1];      // error: operator new[] is protected
    //   NoHeapTwo* p3 = new NoHeapTwo[10]; // error: inherited protected operator new[]

    return 0;
}
