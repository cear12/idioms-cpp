// Lambda Visitor idiom: make_lambda folds a call across a pack of lambdas
// with a fold expression, producing a single callable usable as a
// std::visit visitor.
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

template <typename... Lambdas>
auto make_lambda(Lambdas... lambdas) {
    return [=](auto&& arg) {
        // A plain fold `(lambdas(arg), ...)` would call *every* lambda
        // unconditionally with `arg`, which only compiles if all of them
        // accept `arg`'s type -- defeating the point of a per-case visitor.
        // Guarding each call with `if constexpr` discards the ones that
        // aren't callable with this particular arg from instantiation, so
        // only the matching lambda actually runs.
        auto tryOne = [&](auto&& lambda) {
            if constexpr (std::is_invocable_v<decltype(lambda), decltype(arg)>) {
                lambda(arg);
            }
        };
        (tryOne(lambdas), ...);
    };
}

int main() {
    auto resultPrinter = make_lambda(
        [](std::monostate) { std::cout << "Empty state\n"; },
        [](int x) { std::cout << "Integer: " << x << "\n"; },
        [](const std::string& str) { std::cout << "String: " << str << "\n"; });

    std::variant<std::monostate, int, std::string> value;

    value = std::monostate{};
    std::visit(resultPrinter, value);

    value = 42;
    std::visit(resultPrinter, value);

    value = std::string("hello");
    std::visit(resultPrinter, value);

    return 0;
}
