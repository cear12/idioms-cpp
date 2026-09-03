// Named Template Parameter idiom: each parameter wrapper carries a Tag
// identifying which logical parameter it is, so apply_named_params can
// find each one by tag regardless of the order they're passed in -- true
// "named", order-independent template configuration.
#include <cstddef>
#include <functional>
#include <iostream>
#include <type_traits>

// --- Tags identify *which* logical parameter a wrapper represents.
struct TypeTag {};
struct ComparatorTag {};
struct SizeTag {};

template <typename T>
struct TypeParam {
  using Tag = TypeTag;
  using Type = T;
};

template <typename CmpType>
struct ComparatorParam {
  using Tag = ComparatorTag;
  using Type = CmpType;
};

template <std::size_t N>
struct SizeParam {
  using Tag = SizeTag;
  static constexpr std::size_t kValue = N;
};

using DefaultType = TypeParam<int>;
using DefaultComparator = ComparatorParam<std::less<int>>;
using DefaultSize = SizeParam<16>;

// --- find_param<Tag, Default, Params...>::found is the first element of
// Params... whose ::Tag matches Tag, or Default if none match.
template <typename WantedTag, typename Default, typename... Params>
struct FindParam {
  using Found = Default;
};

template <typename WantedTag, typename Default, typename Head, typename... Tail>
struct FindParam<WantedTag, Default, Head, Tail...> {
  using Found = std::conditional_t<
      std::is_same_v<typename Head::Tag, WantedTag>, Head,
      typename FindParam<WantedTag, Default, Tail...>::Found>;
};

// --- The target template being configured.
template <typename T, typename Cmp, std::size_t N>
class Container {
 public:
  void Info() const { std::cout << "Container<T, Cmp, size=" << N << ">\n"; }
};

// --- Resolves NamedParams... (in any order) against Container's three
// positional slots by tag, then instantiates Template with the result.
template <template <class, class, std::size_t> class Template,
          typename... NamedParams>
struct ApplyNamedParams {
  using TypeP = typename FindParam<TypeTag, DefaultType, NamedParams...>::Found;
  using CmpP = typename FindParam<ComparatorTag, DefaultComparator,
                                  NamedParams...>::Found;
  using SizeP = typename FindParam<SizeTag, DefaultSize, NamedParams...>::Found;

  using Type =
      Template<typename TypeP::Type, typename CmpP::Type, SizeP::kValue>;
};

int main() {
  // Passed as Type, Size, Comparator -- deliberately out of Container's
  // own <T, Cmp, N> order, to demonstrate that order doesn't matter here.
  using MyContainer =
      ApplyNamedParams<Container, TypeParam<double>, SizeParam<32>,
                       ComparatorParam<std::greater<double>>>::Type;
  MyContainer c;
  c.Info();  // Container<T, Cmp, size=32>

  // Only Type is overridden; Comparator and Size fall back to defaults.
  using DefaultContainer = ApplyNamedParams<Container, TypeParam<char>>::Type;
  DefaultContainer d;
  d.Info();  // Container<T, Cmp, size=16>

  return 0;
}
