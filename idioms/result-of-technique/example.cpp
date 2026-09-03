// The result_of Technique idiom: std::result_of<F(Args...)>::type deduces
// a callable's return type. Shown alongside std::invoke_result_t, its
// C++17-recommended, non-deprecated replacement.
#include <iostream>
#include <type_traits>
#include <utility>

// --- Old style: std::result_of, deprecated in C++17 and removed in C++20.
// Demonstrating the historical idiom intentionally triggers
// -Wdeprecated-declarations; the warning is suppressed only for this one
// function, with the reason documented at the point of suppression.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
template <typename F, typename... Args>
typename std::result_of<F(Args...)>::type
InvokeOld(F f, Args&&... args) {
    return f(std::forward<Args>(args)...);
}
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// --- C++17-recommended replacement: std::invoke_result_t.
template <typename F, typename... Args>
std::invoke_result_t<F, Args...>
InvokeNew(F f, Args&&... args) {
    return f(std::forward<Args>(args)...);
}

struct Multiplier {
    int operator()(int x, int y) const { return x * y; }
};

int main() {
    Multiplier m;
    auto add = [](double a, double b) -> double { return a + b; };

    int prod_old = InvokeOld(m, 3, 4);
    double sum_old = InvokeOld(add, 1.5, 2.5);
    std::cout << "[result_of]      3 * 4 = " << prod_old << "\n";
    std::cout << "[result_of]  1.5 + 2.5 = " << sum_old << "\n";

    int prod_new = InvokeNew(m, 3, 4);
    double sum_new = InvokeNew(add, 1.5, 2.5);
    std::cout << "[invoke_result_t] 3 * 4 = " << prod_new << "\n";
    std::cout << "[invoke_result_t] 1.5 + 2.5 = " << sum_new << "\n";

    return 0;
}
