// Implicit Conversions idiom: contrasts a type with implicit conversions
// both ways (A) against one that requires explicit conversion (B).
#include <iostream>
#include <string>
#include <cstdlib>

// A wraps a string but converts implicitly to int.
struct A {
    // Implicit constructor from std::string.
    A(const std::string& s) : value_(s) {}

    // Implicit conversion operator to int.
    operator int() const {
        return std::atoi(value_.c_str());
    }

    void Print() const {
        std::cout << "A holds: " << value_ << "\n";
    }

private:
    std::string value_;
};

// B is the same idea, but with an explicit constructor that suppresses
// implicit construction from std::string (and therefore from const char*).
struct B {
    explicit B(const std::string& s) : value_(s) {}

    // Conversion operator to double is left implicit on purpose, to show
    // that "explicit" on the constructor and on the conversion operator
    // are independent choices.
    operator double() const {
        return std::stod(value_);
    }

    void Print() const {
        std::cout << "B holds: " << value_ << "\n";
    }

private:
    std::string value_;
};

void PrintInt(int x) {
    std::cout << "printInt: " << x << "\n";
}

void PrintDouble(double d) {
    std::cout << "printDouble: " << d << "\n";
}

int main() {
    A a1("100");
    a1.Print();

    // Implicit conversion A -> int (one user-defined conversion).
    PrintInt(a1);

    // NOTE: printInt("200") would NOT compile here: const char* -> std::string
    // is already one user-defined conversion (via std::string's converting
    // constructor), and std::string -> A would be a second one. C++ allows
    // at most one user-defined conversion per implicit sequence, so the two
    // don't chain. Constructing the intermediate explicitly keeps the rest
    // of the sequence (A -> int) implicit:
    PrintInt(A("200"));

    B b1("3.14");
    b1.Print();

    // Implicit B -> double.
    PrintDouble(b1);

    // const char* -> std::string -> B is explicit, so this would fail:
    // printDouble("2.718"); // error: no matching function

    // Explicit conversion required instead:
    PrintDouble(static_cast<double>(B("2.718")));

    return 0;
}
