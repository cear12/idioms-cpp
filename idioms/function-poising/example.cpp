// Function Poisoning idiom: create_foo() is wrapped in an RAII type, then
// poisoned so any later, accidental direct call to create_foo() is a
// compile error instead of a leaked resource. GCC/Clang extension.
#include <memory>
#include <iostream>

struct Foo {
    int value_;
};

extern "C" Foo* CreateFoo();
extern "C" void FooDestroy(Foo*);

Foo* CreateFoo() { return new Foo{42}; }
void FooDestroy(Foo* p) { delete p; }

struct FooDeleter {
    void operator()(Foo* p) const { FooDestroy(p); }
};
using UniqueFoo = std::unique_ptr<Foo, FooDeleter>;

inline UniqueFoo CreateUniqueFoo() {
    return UniqueFoo{CreateFoo()};
}

#if defined(__GNUC__)
#pragma GCC poison create_foo
#endif

int main() {
    UniqueFoo uf = CreateUniqueFoo();
    std::cout << "foo.value = " << uf->value_ << "\n";

    // Uncommenting the next line is now a compile error under GCC/Clang,
    // instead of a silent resource leak:
    // foo* raw = create_foo();

    return 0;
}
