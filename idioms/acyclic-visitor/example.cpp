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
  virtual void Visit(VisitedType&) = 0;

 protected:
  ~VisitorFor() = default;
};

class Visitable {
 public:
  virtual void Accept(Visitor& v) = 0;
  virtual ~Visitable() = default;
};

class ElementA : public Visitable {
 public:
  void Accept(Visitor& v) override {
    if (auto* specific = dynamic_cast<VisitorFor<ElementA>*>(&v)) {
      specific->Visit(*this);
    }
  }
  void FooA() { std::cout << "ElementA specific behavior\n"; }
};

class ElementB : public Visitable {
 public:
  void Accept(Visitor& v) override {
    if (auto* specific = dynamic_cast<VisitorFor<ElementB>*>(&v)) {
      specific->Visit(*this);
    }
  }
  void FooB() { std::cout << "ElementB specific behavior\n"; }
};

// Handles both element types.
class PrintVisitor : public Visitor,
                     public VisitorFor<ElementA>,
                     public VisitorFor<ElementB> {
 public:
  void Visit(ElementA& a) override {
    std::cout << "PrintVisitor: visiting ElementA\n";
    a.FooA();
  }
  void Visit(ElementB& b) override {
    std::cout << "PrintVisitor: visiting ElementB\n";
    b.FooB();
  }
};

// Handles only ElementA -- compiles fine even though ElementB exists.
class OnlyAVisitor : public Visitor, public VisitorFor<ElementA> {
 public:
  void Visit(ElementA&) override {
    std::cout << "OnlyAVisitor: handling ElementA\n";
  }
};

int main() {
  ElementA a;
  ElementB b;

  PrintVisitor print_visitor;
  std::cout << "-- PrintVisitor visits both elements --\n";
  a.Accept(print_visitor);
  b.Accept(print_visitor);

  OnlyAVisitor only_a_visitor;
  std::cout
      << "\n-- OnlyAVisitor visits ElementA, silently skips ElementB --\n";
  a.Accept(only_a_visitor);  // handled
  b.Accept(only_a_visitor);  // no matching VisitorFor<ElementB>: dynamic_cast
                             // fails, silently skipped

  return 0;
}
