// Manual RTTI idiom: each Shape reports its own kind() tag through a
// virtual function; KindOf<T> maps a concrete type back to its tag, so
// IsA<T>()/ManualCast<T>() work generically without typeid/dynamic_cast.
#include <iostream>

enum class ShapeKind { kCircle, kSquare, kTriangle };

class Shape {
 public:
  virtual ~Shape() = default;
  virtual ShapeKind Kind() const = 0;
  virtual const char* ClassName()
      const = 0;  // hand-maintained, unlike typeid(...).name()
};

class Circle : public Shape {
 public:
  ShapeKind Kind() const override { return ShapeKind::kCircle; }
  const char* ClassName() const override { return "Circle"; }
};

class Square : public Shape {
 public:
  ShapeKind Kind() const override { return ShapeKind::kSquare; }
  const char* ClassName() const override { return "Square"; }
};

class Triangle : public Shape {
 public:
  ShapeKind Kind() const override { return ShapeKind::kTriangle; }
  const char* ClassName() const override { return "Triangle"; }
};

// Maps a concrete C++ type back to its ShapeKind tag, so is_a<T>/
// manual_cast<T> can be written once instead of repeating comparisons.
template <typename T>
struct KindOf;
template <>
struct KindOf<Circle> {
  static constexpr ShapeKind kValue = ShapeKind::kCircle;
};
template <>
struct KindOf<Square> {
  static constexpr ShapeKind kValue = ShapeKind::kSquare;
};
template <>
struct KindOf<Triangle> {
  static constexpr ShapeKind kValue = ShapeKind::kTriangle;
};

template <typename T>
bool IsA(const Shape& s) {
  return s.Kind() == KindOf<T>::kValue;
}

template <typename T>
T* ManualCast(Shape* s) {
  return (s && IsA<T>(*s)) ? static_cast<T*>(s) : nullptr;
}

int main() {
  Circle circle;
  Square square;
  Triangle triangle;

  Shape* shapes[] = {&circle, &square, &triangle};
  for (Shape* s : shapes) {
    std::cout << s->ClassName() << " is_a<Circle>=" << std::boolalpha
              << IsA<Circle>(*s) << "\n";
  }

  if (ManualCast<Circle>(shapes[0])) {
    std::cout << "manual_cast<Circle>(shapes[0]) succeeded, as expected\n";
  }
  if (ManualCast<Circle>(shapes[1]) == nullptr) {
    std::cout << "manual_cast<Circle>(shapes[1]) correctly returned nullptr "
                 "(it's a Square)\n";
  }

  return 0;
}
