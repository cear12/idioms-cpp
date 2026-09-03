// Downcasting Techniques idiom: five ways to get a Derived* back from a
// Base*, run side by side so their trade-offs (RTTI or not, per-class
// boilerplate, safety) can be compared directly.
#include <iostream>
#include <type_traits>

// ============================================================
// 1. dynamic_cast
// ============================================================
namespace technique1 {
struct Base {
  virtual ~Base() = default;
};
struct Derived : Base {
  void Hello() const { std::cout << "technique1::Derived\n"; }
};

void Demo() {
  Base* b = new Derived();
  if (Derived* d = dynamic_cast<Derived*>(b)) {
    d->Hello();
  }
  delete b;
}
}  // namespace technique1

// ============================================================
// 2. Virtual "is-a" function
// ============================================================
namespace technique2 {
class Derived;  // forward declaration

class Base {
 public:
  virtual ~Base() = default;
  virtual Derived* IsDerived() { return nullptr; }
};

class Derived : public Base {
 public:
  Derived* IsDerived() override { return this; }
  void Hello() const { std::cout << "technique2::Derived\n"; }
};

void Demo() {
  Base* b = new Derived();
  if (Derived* d = b->IsDerived()) {
    d->Hello();
  }
  delete b;
}
}  // namespace technique2

// ============================================================
// 3. Type tags + static_cast
// ============================================================
namespace technique3 {
class Base {
 public:
  enum Type { kBase, kDerived };
  Type GetType() const { return m_type_; }
  enum { kTag = kBase };

  Base() : m_type_(kBase) {}
  virtual ~Base() = default;

 protected:
  explicit Base(Type t) : m_type_(t) {}

 private:
  Type m_type_;
};

class Derived : public Base {
 public:
  Derived() : Base(Base::kDerived) {}
  enum { kTag = kDerived };
  void Hello() const { std::cout << "technique3::Derived\n"; }
};

template <class Target, class Source>
Target* DownCast(Source* s) {
  // s->GetType() and Target::kTag are enumerators of two different
  // (unscoped) enum types; comparing them directly is well-defined (both
  // promote to int) but triggers -Wenum-compare, so the intent is made
  // explicit here.
  return static_cast<int>(s->GetType()) == static_cast<int>(Target::kTag)
             ? static_cast<Target*>(s)
             : nullptr;
}

void Demo() {
  Base* b = new Derived();
  if (Derived* d = DownCast<Derived>(b)) {
    d->Hello();
  }
  delete b;
}
}  // namespace technique3

// ============================================================
// 4. Manual RTTI via per-type virtual accessors
//    (see the sibling `manual-rtti` idiom for the open, single-accessor
//    style of hand-rolled RTTI and a fuller discussion)
// ============================================================
namespace technique4 {
class TestDerived1;

class TestBase {
 public:
  virtual ~TestBase() = default;
  virtual TestDerived1* AsTestDerived1() { return nullptr; }
};

class TestDerived1 : public TestBase {
 public:
  TestDerived1* AsTestDerived1() override { return this; }
  void Hello() const { std::cout << "technique4::TestDerived1\n"; }
};

void Demo() {
  TestBase* b = new TestDerived1();
  if (TestDerived1* d = b->AsTestDerived1()) {
    d->Hello();
  }
  delete b;
}
}  // namespace technique4

// ============================================================
// 5. constexpr-branching AsCast
// ============================================================
namespace technique5 {
struct NonPolyBase {};
struct NonPolyDerived : NonPolyBase {
  void Hello() const { std::cout << "technique5::NonPolyDerived\n"; }
};

struct PolyBase {
  virtual ~PolyBase() = default;
};
struct PolyDerived : PolyBase {
  void Hello() const { std::cout << "technique5::PolyDerived\n"; }
};

template <typename To, typename From>
To* AsCast(From* ptr) noexcept {
  if (!ptr) return nullptr;
  if constexpr (std::is_polymorphic_v<From>) {
    return dynamic_cast<To*>(ptr);  // checked: safe even for unrelated types
  } else if constexpr (std::is_convertible_v<From*, To*>) {
    return static_cast<To*>(
        ptr);  // no RTTI available; only safe because
               // convertibility was already proven statically
  } else {
    return nullptr;
  }
}

void Demo() {
  PolyBase* pb = new PolyDerived();
  if (auto* pd = AsCast<PolyDerived>(pb)) pd->Hello();
  delete pb;

  NonPolyDerived nd;
  NonPolyBase* nb = &nd;
  if (auto* back = AsCast<NonPolyDerived>(nb)) back->Hello();
}
}  // namespace technique5

int main() {
  technique1::Demo();
  technique2::Demo();
  technique3::Demo();
  technique4::Demo();
  technique5::Demo();
  return 0;
}
