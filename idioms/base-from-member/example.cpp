// Base-from-Member idiom: base_from_member<T> is inherited ahead of Base,
// so its `member` is fully constructed (base classes construct in
// declaration order) before Base's constructor runs and receives a
// reference to it.
//
// The bug this idiom fixes, for reference (not compiled -- it is
// undefined behavior, using `member` before it is constructed):
//
//   struct Derived : Base {
//       Member member;
//       Derived(int x)
//           : Base(member),  // BUG: `member` isn't constructed yet
//             member(x)
//       {}
//   };
#include <iostream>
#include <utility>

struct Member {
    explicit Member(int x) : value(x) {
        std::cout << "Member constructed with value=" << value << "\n";
    }
    int value;
};

struct Base {
    explicit Base(Member& m) {
        std::cout << "Base constructed, sees Member value=" << m.value << "\n";
    }
};

template <typename MemberType, int UniqueID = 0>
struct base_from_member {
    MemberType member;

    template <typename... Args>
    explicit base_from_member(Args&&... args)
        : member(std::forward<Args>(args)...) {}
};

// Inheriting base_from_member<Member> before Base guarantees `member` is
// constructed first, so Base(member) is always safe.
struct Derived : private base_from_member<Member>, public Base {
    explicit Derived(int x)
        : base_from_member<Member>(x), // `member` constructed here, first
          Base(member)                 // ...then safely passed to Base
    {}

    using base_from_member<Member>::member;
};

int main() {
    Derived d(42);
    std::cout << "Derived.member.value = " << d.member.value << "\n";
    return 0;
}
