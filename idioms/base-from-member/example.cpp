// Base-from-Member idiom: BaseFromMember<T> is inherited ahead of Base,
// so its `member_` is fully constructed (base classes construct in
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
  explicit Member(int x) : value_(x) {
    std::cout << "Member constructed with value=" << value_ << "\n";
  }
  int value_;
};

struct Base {
  explicit Base(Member& m) {
    std::cout << "Base constructed, sees Member value=" << m.value_ << "\n";
  }
};

template <typename MemberType, int UniqueID = 0>
struct BaseFromMember {
  MemberType member_;

  template <typename... Args>
  explicit BaseFromMember(Args&&... args)
      : member_(std::forward<Args>(args)...) {}
};

// Inheriting BaseFromMember<Member> before Base guarantees `member_` is
// constructed first, so Base(member_) is always safe.
struct Derived : private BaseFromMember<Member>, public Base {
  explicit Derived(int x)
      : BaseFromMember<Member>(x),  // `member` constructed here, first
        Base(member_)               // ...then safely passed to Base
  {}

  using BaseFromMember<Member>::member_;
};

int main() {
  Derived d(42);
  std::cout << "Derived.member.value = " << d.member_.value_ << "\n";
  return 0;
}
