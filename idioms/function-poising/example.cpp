// Function Poisoning idiom: create_foo() is wrapped in an RAII type, then
// poisoned so any later, accidental direct call to create_foo() is a
// compile error instead of a leaked resource. GCC/Clang extension.
#include <memory>
#include <iostream>

struct foo {
    int value;
};

extern "C" foo* create_foo();
extern "C" void foo_destroy(foo*);

foo* create_foo() { return new foo{42}; }
void foo_destroy(foo* p) { delete p; }

struct foo_deleter {
    void operator()(foo* p) const { foo_destroy(p); }
};
using unique_foo = std::unique_ptr<foo, foo_deleter>;

inline unique_foo create_unique_foo() {
    return unique_foo{create_foo()};
}

#if defined(__GNUC__)
#pragma GCC poison create_foo
#endif

int main() {
    unique_foo uf = create_unique_foo();
    std::cout << "foo.value = " << uf->value << "\n";

    // Uncommenting the next line is now a compile error under GCC/Clang,
    // instead of a silent resource leak:
    // foo* raw = create_foo();

    return 0;
}
