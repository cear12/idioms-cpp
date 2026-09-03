// Covariant Return Types idiom: Circle::Clone() overrides Shape::Clone()
// and narrows the return type from Shape* to Circle*, so callers get the
// precise type back without an explicit downcast.
#include <iostream>
#include <memory>

class Shape {
public:
    virtual Shape* Clone() const {
        std::cout << "Cloning Shape\n";
        return new Shape(*this);
    }
    virtual void Draw() const {
        std::cout << "Drawing Shape\n";
    }
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}

    // Covariant return: Circle* is a valid override for a base that
    // returns Shape*, because Circle is publicly derived from Shape.
    Circle* Clone() const override {
        std::cout << "Cloning Circle with radius " << radius_ << "\n";
        return new Circle(*this);
    }
    void Draw() const override {
        std::cout << "Drawing Circle of radius " << radius_ << "\n";
    }
};

int main() {
    // Through a Circle-typed handle, Clone()'s *static* type is already
    // Circle* thanks to the covariant override -- no cast needed.
    std::unique_ptr<Circle> c1 = std::make_unique<Circle>(5.0);
    std::unique_ptr<Circle> c2(c1->Clone());
    c2->Draw();

    // Through a Shape-typed handle, Clone()'s static type is still Shape*
    // (covariance narrows what the *override* returns, not what a
    // less-derived pointer's call expression is typed as) -- even though a
    // Circle is cloned at runtime, getting a Circle* back still needs a cast.
    std::unique_ptr<Shape> s1 = std::make_unique<Circle>(3.0);
    std::unique_ptr<Shape> s2(s1->Clone());
    s2->Draw();

    return 0;
}
