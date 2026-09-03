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
    using type = T;
};

template <typename CmpType>
struct ComparatorParam {
    using Tag = ComparatorTag;
    using type = CmpType;
};

template <std::size_t N>
struct SizeParam {
    using Tag = SizeTag;
    static constexpr std::size_t value = N;
};

using DefaultType       = TypeParam<int>;
using DefaultComparator = ComparatorParam<std::less<int>>;
using DefaultSize       = SizeParam<16>;

// --- find_param<Tag, Default, Params...>::found is the first element of
// Params... whose ::Tag matches Tag, or Default if none match.
template <typename WantedTag, typename Default, typename... Params>
struct find_param {
    using found = Default;
};

template <typename WantedTag, typename Default, typename Head, typename... Tail>
struct find_param<WantedTag, Default, Head, Tail...> {
    using found = std::conditional_t<
        std::is_same_v<typename Head::Tag, WantedTag>,
        Head,
        typename find_param<WantedTag, Default, Tail...>::found>;
};

// --- The target template being configured.
template <typename T, typename Cmp, std::size_t N>
class Container {
public:
    void info() const {
        std::cout << "Container<T, Cmp, size=" << N << ">\n";
    }
};

// --- Resolves NamedParams... (in any order) against Container's three
// positional slots by tag, then instantiates Template with the result.
template <template <class, class, std::size_t> class Template, typename... NamedParams>
struct apply_named_params {
    using TypeP = typename find_param<TypeTag, DefaultType, NamedParams...>::found;
    using CmpP  = typename find_param<ComparatorTag, DefaultComparator, NamedParams...>::found;
    using SizeP = typename find_param<SizeTag, DefaultSize, NamedParams...>::found;

    using type = Template<typename TypeP::type, typename CmpP::type, SizeP::value>;
};

int main() {
    // Passed as Type, Size, Comparator -- deliberately out of Container's
    // own <T, Cmp, N> order, to demonstrate that order doesn't matter here.
    using MyContainer = apply_named_params<
        Container,
        TypeParam<double>,
        SizeParam<32>,
        ComparatorParam<std::greater<double>>>::type;
    MyContainer c;
    c.info(); // Container<T, Cmp, size=32>

    // Only Type is overridden; Comparator and Size fall back to defaults.
    using DefaultContainer = apply_named_params<Container, TypeParam<char>>::type;
    DefaultContainer d;
    d.info(); // Container<T, Cmp, size=16>

    return 0;
}
