// SFINAE idiom: has_serialize<T> detects, at compile time, whether T has a
// callable serialize() method, and call_serialize dispatches on that
// without a hard compile error for types that don't.
#include <iostream>
#include <type_traits>
#include <utility>

namespace sfinae_demo {

template <typename T>
auto test_serialize(int) -> decltype(std::declval<T>().serialize(), std::true_type());

template <typename T>
std::false_type test_serialize(...);

template <typename T>
struct has_serialize : decltype(test_serialize<T>(0)) {};

class WithSerialize {
public:
    void serialize() const { std::cout << "WithSerialize::serialize()\n"; }
};

class WithoutSerialize {};

template <typename T>
typename std::enable_if<has_serialize<T>::value, void>::type call_serialize(const T& obj) {
    obj.serialize();
}

template <typename T>
typename std::enable_if<!has_serialize<T>::value, void>::type call_serialize(const T& obj) {
    (void)obj;
    std::cout << "No serialize() member available\n";
}

void demo_has_serialize() {
    std::cout << "-- has_serialize --\n";
    WithSerialize with;
    WithoutSerialize without;
    call_serialize(with);
    call_serialize(without);
}

// A second, independent SFINAE example: detecting pointer-ness via partial
// specialization rather than expression validity.
template <typename T>
struct is_pointer_like : std::false_type {};

template <typename T>
struct is_pointer_like<T*> : std::true_type {};

void demo_is_pointer_like() {
    std::cout << "\n-- is_pointer_like --\n";
    std::cout << std::boolalpha;
    std::cout << "int is a pointer: " << is_pointer_like<int>::value << "\n";
    std::cout << "int* is a pointer: " << is_pointer_like<int*>::value << "\n";
}

} // namespace sfinae_demo

int main() {
    sfinae_demo::demo_has_serialize();
    sfinae_demo::demo_is_pointer_like();
    return 0;
}
