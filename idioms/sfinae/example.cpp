// SFINAE idiom: HasSerialize<T> detects, at compile time, whether T has a
// callable Serialize() method, and CallSerialize dispatches on that
// without a hard compile error for types that don't.
#include <iostream>
#include <type_traits>
#include <utility>

namespace sfinae_demo {

template <typename T>
auto test_serialize(int)
    -> decltype(std::declval<T>().Serialize(), std::true_type());

template <typename T>
std::false_type test_serialize(...);

template <typename T>
struct HasSerialize : decltype(test_serialize<T>(0)) {};

class WithSerialize {
 public:
  void Serialize() const { std::cout << "WithSerialize::serialize()\n"; }
};

class WithoutSerialize {};

template <typename T>
typename std::enable_if<HasSerialize<T>::value, void>::type CallSerialize(
    const T& obj) {
  obj.Serialize();
}

template <typename T>
typename std::enable_if<!HasSerialize<T>::value, void>::type CallSerialize(
    const T& obj) {
  (void)obj;
  std::cout << "No serialize() member available\n";
}

void DemoHasSerialize() {
  std::cout << "-- has_serialize --\n";
  WithSerialize with;
  WithoutSerialize without;
  CallSerialize(with);
  CallSerialize(without);
}

// A second, independent SFINAE example: detecting pointer-ness via partial
// specialization rather than expression validity.
template <typename T>
struct IsPointerLike : std::false_type {};

template <typename T>
struct IsPointerLike<T*> : std::true_type {};

void DemoIsPointerLike() {
  std::cout << "\n-- is_pointer_like --\n";
  std::cout << std::boolalpha;
  std::cout << "int is a pointer: " << IsPointerLike<int>::value << "\n";
  std::cout << "int* is a pointer: " << IsPointerLike<int*>::value << "\n";
}

}  // namespace sfinae_demo

int main() {
  sfinae_demo::DemoHasSerialize();
  sfinae_demo::DemoIsPointerLike();
  return 0;
}
