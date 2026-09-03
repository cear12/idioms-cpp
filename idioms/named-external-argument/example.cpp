// Named External Argument idiom: each parameter gets its own tag type and
// factory function, so a call site reads like named arguments and
// swapping two same-typed arguments becomes a type error instead of a
// silent bug.
#include <iostream>
#include <string>

struct Width     { int value; };
struct Height    { int value; };
struct Resizable { bool value; };
struct Decorated { bool value; };
struct Title     { std::string value; };

constexpr Width     width(int v)      { return Width{v}; }
constexpr Height    height(int v)     { return Height{v}; }
constexpr Resizable resizable(bool v) { return Resizable{v}; }
constexpr Decorated decorated(bool v) { return Decorated{v}; }
Title title(const std::string& v)     { return Title{v}; }

void configureWindow(Width w, Height h, Resizable r, Decorated d, Title t) {
    std::cout << "Window Configuration:\n"
              << "  Width     = " << w.value << "\n"
              << "  Height    = " << h.value << "\n"
              << "  Resizable = " << std::boolalpha << r.value << "\n"
              << "  Decorated = " << std::boolalpha << d.value << "\n"
              << "  Title     = " << t.value << "\n";
}

int main() {
    // Reads like named arguments; swapping width(...)/height(...) by
    // accident would be caught at compile time since they're distinct types.
    configureWindow(width(800), height(600), resizable(true), decorated(false),
                     title("My App Window"));
    return 0;
}
