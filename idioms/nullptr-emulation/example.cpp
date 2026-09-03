// nullptr Emulation idiom: a class that implicitly converts to any pointer
// type but to no integral type, used as a pre-C++11 library stand-in for
// what became the `nullptr` keyword in C++11.
//
// NOTE: the original idiom names its global instance `nullptr`; that name
// is now a reserved keyword (this file targets C++17), so it is named
// `my_nullptr` here purely so this historical technique still compiles.
// New code should just use the real `nullptr` keyword.
#include <cstddef>
#include <iostream>

const class nullptr_emulation_t {
public:
    // Convertible to any non-member pointer type...
    template <class T>
    operator T*() const { return 0; }

    // ...or any member pointer type...
    template <class C, class T>
    operator T C::*() const { return 0; }

private:
    // ...but never take its address, so it can't be mistaken for a real
    // object with storage.
    void operator&() const = delete;

} my_nullptr = {};

void takesPointer(int*) {
    std::cout << "takesPointer(int*) called\n";
}

void takesInt(int) {
    std::cout << "takesInt(int) called\n";
}

// Overloaded on pointer vs. integral: with real 0/NULL this would be
// ambiguous or silently pick the wrong overload; my_nullptr resolves to
// the pointer overload unambiguously, exactly like the real `nullptr`.
void overloaded(int*) { std::cout << "overloaded(int*) called\n"; }
void overloaded(long) { std::cout << "overloaded(long) called\n"; }

int main() {
    int* p = my_nullptr;
    std::cout << "p == nullptr: " << std::boolalpha << (p == nullptr) << "\n";

    takesPointer(my_nullptr);
    takesInt(0); // ordinary 0 still converts to int, as always

    overloaded(my_nullptr); // binds to the pointer overload, unambiguously

    return 0;
}
