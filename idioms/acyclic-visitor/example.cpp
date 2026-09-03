// Acyclic Visitor idiom: each element type gets its own single-method
// VisitorFor<T> interface, so a concrete visitor only implements the
// element types it actually cares about, and new element types don't
// require touching existing visitors.
#include <iostream>

class Visitor {
public:
    virtual ~Visitor() = default;
};

template <class VisitedType>
class VisitorFor {
public:
    virtual void visit(VisitedType&) = 0;

protected:
    ~VisitorFor() = default;
};

class Visitable {
public:
    virtual void accept(Visitor& v) = 0;
    virtual ~Visitable() = default;
};

class ElementA : public Visitable {
public:
    void accept(Visitor& v) override {
        if (auto* specific = dynamic_cast<VisitorFor<ElementA>*>(&v)) {
            specific->visit(*this);
        }
    }
    void fooA() { std::cout << "ElementA specific behavior\n"; }
};

class ElementB : public Visitable {
public:
    void accept(Visitor& v) override {
        if (auto* specific = dynamic_cast<VisitorFor<ElementB>*>(&v)) {
            specific->visit(*this);
        }
    }
    void fooB() { std::cout << "ElementB specific behavior\n"; }
};

// Handles both element types.
class PrintVisitor : public Visitor,
                      public VisitorFor<ElementA>,
                      public VisitorFor<ElementB> {
public:
    void visit(ElementA& a) override {
        std::cout << "PrintVisitor: visiting ElementA\n";
        a.fooA();
    }
    void visit(ElementB& b) override {
        std::cout << "PrintVisitor: visiting ElementB\n";
        b.fooB();
    }
};

// Handles only ElementA -- compiles fine even though ElementB exists.
class OnlyAVisitor : public Visitor, public VisitorFor<ElementA> {
public:
    void visit(ElementA&) override {
        std::cout << "OnlyAVisitor: handling ElementA\n";
    }
};

int main() {
    ElementA a;
    ElementB b;

    PrintVisitor printVisitor;
    std::cout << "-- PrintVisitor visits both elements --\n";
    a.accept(printVisitor);
    b.accept(printVisitor);

    OnlyAVisitor onlyAVisitor;
    std::cout << "\n-- OnlyAVisitor visits ElementA, silently skips ElementB --\n";
    a.accept(onlyAVisitor); // handled
    b.accept(onlyAVisitor); // no matching VisitorFor<ElementB>: dynamic_cast fails, silently skipped

    return 0;
}
