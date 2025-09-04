#include <iostream>
#include <variant>

struct Fluid { };
struct LightItem { };
struct HeavyItem { };
struct FragileItem { };


template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>; // для C++17

int main() {
    std::variant<Fluid, LightItem, HeavyItem, FragileItem> package = Fluid{};
    std::visit(overload{
        [](Fluid& )       { std::cout << "fluid\n"; },
        [](LightItem& )   { std::cout << "light item\n"; },
        [](HeavyItem& )   { std::cout << "heavy item\n"; },
        [](FragileItem& ) { std::cout << "fragile\n"; }
    }, package);
}
