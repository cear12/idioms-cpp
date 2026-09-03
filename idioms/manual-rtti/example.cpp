// Manual RTTI idiom: each Shape reports its own kind() tag through a
// virtual function; KindOf<T> maps a concrete type back to its tag, so
// is_a<T>()/manual_cast<T>() work generically without typeid/dynamic_cast.
#include <iostream>

enum class ShapeKind { Circle, Square, Triangle };

class Shape {
public:
    virtual ~Shape() = default;
    virtual ShapeKind kind() const = 0;
    virtual const char* className() const = 0; // hand-maintained, unlike typeid(...).name()
};

class Circle : public Shape {
public:
    ShapeKind kind() const override { return ShapeKind::Circle; }
    const char* className() const override { return "Circle"; }
};

class Square : public Shape {
public:
    ShapeKind kind() const override { return ShapeKind::Square; }
    const char* className() const override { return "Square"; }
};

class Triangle : public Shape {
public:
    ShapeKind kind() const override { return ShapeKind::Triangle; }
    const char* className() const override { return "Triangle"; }
};

// Maps a concrete C++ type back to its ShapeKind tag, so is_a<T>/
// manual_cast<T> can be written once instead of repeating comparisons.
template <typename T> struct KindOf;
template <> struct KindOf<Circle>   { static constexpr ShapeKind value = ShapeKind::Circle; };
template <> struct KindOf<Square>   { static constexpr ShapeKind value = ShapeKind::Square; };
template <> struct KindOf<Triangle> { static constexpr ShapeKind value = ShapeKind::Triangle; };

template <typename T>
bool is_a(const Shape& s) {
    return s.kind() == KindOf<T>::value;
}

template <typename T>
T* manual_cast(Shape* s) {
    return (s && is_a<T>(*s)) ? static_cast<T*>(s) : nullptr;
}

int main() {
    Circle circle;
    Square square;
    Triangle triangle;

    Shape* shapes[] = {&circle, &square, &triangle};
    for (Shape* s : shapes) {
        std::cout << s->className()
                  << " is_a<Circle>=" << std::boolalpha << is_a<Circle>(*s) << "\n";
    }

    if (manual_cast<Circle>(shapes[0])) {
        std::cout << "manual_cast<Circle>(shapes[0]) succeeded, as expected\n";
    }
    if (manual_cast<Circle>(shapes[1]) == nullptr) {
        std::cout << "manual_cast<Circle>(shapes[1]) correctly returned nullptr (it's a Square)\n";
    }

    return 0;
}
