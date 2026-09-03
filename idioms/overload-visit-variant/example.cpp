// Overload Set for std::visit idiom: `overload` aggregates several lambdas
// into one callable with one operator() per variant alternative, so
// std::visit can be called with an inline set of lambdas.
#include <iostream>
#include <variant>

struct Fluid {};
struct LightItem {};
struct HeavyItem {};
struct FragileItem {};

template <class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>; // class template argument deduction guide (C++17)

int main() {
    std::variant<Fluid, LightItem, HeavyItem, FragileItem> package = Fluid{};

    auto describe = overload{
        [](Fluid&) { std::cout << "fluid\n"; },
        [](LightItem&) { std::cout << "light item\n"; },
        [](HeavyItem&) { std::cout << "heavy item\n"; },
        [](FragileItem&) { std::cout << "fragile\n"; },
    };

    std::visit(describe, package);

    package = HeavyItem{};
    std::visit(describe, package);

    package = FragileItem{};
    std::visit(describe, package);

    return 0;
}
