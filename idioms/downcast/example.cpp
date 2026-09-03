// Downcasting Techniques idiom: five ways to get a Derived* back from a
// Base*, run side by side so their trade-offs (RTTI or not, per-class
// boilerplate, safety) can be compared directly.
#include <iostream>
#include <type_traits>

// ============================================================
// 1. dynamic_cast
// ============================================================
namespace technique1 {
struct Base { virtual ~Base() = default; };
struct Derived : Base { void hello() const { std::cout << "technique1::Derived\n"; } };

void demo() {
    Base* b = new Derived();
    if (Derived* d = dynamic_cast<Derived*>(b)) {
        d->hello();
    }
    delete b;
}
} // namespace technique1

// ============================================================
// 2. Virtual "is-a" function
// ============================================================
namespace technique2 {
class Derived; // forward declaration

class Base {
public:
    virtual ~Base() = default;
    virtual Derived* IsDerived() { return nullptr; }
};

class Derived : public Base {
public:
    Derived* IsDerived() override { return this; }
    void hello() const { std::cout << "technique2::Derived\n"; }
};

void demo() {
    Base* b = new Derived();
    if (Derived* d = b->IsDerived()) {
        d->hello();
    }
    delete b;
}
} // namespace technique2

// ============================================================
// 3. Type tags + static_cast
// ============================================================
namespace technique3 {
class Base {
public:
    enum Type { BASE, DERIVED };
    Type type() const { return mType; }
    enum { TAG = BASE };

    Base() : mType(BASE) {}
    virtual ~Base() = default;

protected:
    explicit Base(Type t) : mType(t) {}

private:
    Type mType;
};

class Derived : public Base {
public:
    Derived() : Base(Base::DERIVED) {}
    enum { TAG = DERIVED };
    void hello() const { std::cout << "technique3::Derived\n"; }
};

template <class Target, class Source>
Target* down_cast(Source* s) {
    // s->type() and Target::TAG are enumerators of two different (unscoped)
    // enum types; comparing them directly is well-defined (both promote to
    // int) but triggers -Wenum-compare, so the intent is made explicit here.
    return static_cast<int>(s->type()) == static_cast<int>(Target::TAG)
               ? static_cast<Target*>(s)
               : nullptr;
}

void demo() {
    Base* b = new Derived();
    if (Derived* d = down_cast<Derived>(b)) {
        d->hello();
    }
    delete b;
}
} // namespace technique3

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
    virtual TestDerived1* asTestDerived1() { return nullptr; }
};

class TestDerived1 : public TestBase {
public:
    TestDerived1* asTestDerived1() override { return this; }
    void hello() const { std::cout << "technique4::TestDerived1\n"; }
};

void demo() {
    TestBase* b = new TestDerived1();
    if (TestDerived1* d = b->asTestDerived1()) {
        d->hello();
    }
    delete b;
}
} // namespace technique4

// ============================================================
// 5. constexpr-branching as_cast
// ============================================================
namespace technique5 {
struct NonPolyBase {};
struct NonPolyDerived : NonPolyBase { void hello() const { std::cout << "technique5::NonPolyDerived\n"; } };

struct PolyBase { virtual ~PolyBase() = default; };
struct PolyDerived : PolyBase { void hello() const { std::cout << "technique5::PolyDerived\n"; } };

template <typename To, typename From>
To* as_cast(From* ptr) noexcept {
    if (!ptr) return nullptr;
    if constexpr (std::is_polymorphic_v<From>) {
        return dynamic_cast<To*>(ptr); // checked: safe even for unrelated types
    } else if constexpr (std::is_convertible_v<From*, To*>) {
        return static_cast<To*>(ptr); // no RTTI available; only safe because
                                       // convertibility was already proven statically
    } else {
        return nullptr;
    }
}

void demo() {
    PolyBase* pb = new PolyDerived();
    if (auto* pd = as_cast<PolyDerived>(pb)) pd->hello();
    delete pb;

    NonPolyDerived nd;
    NonPolyBase* nb = &nd;
    if (auto* back = as_cast<NonPolyDerived>(nb)) back->hello();
}
} // namespace technique5

int main() {
    technique1::demo();
    technique2::demo();
    technique3::demo();
    technique4::demo();
    technique5::demo();
    return 0;
}
