// Exploding Return Type idiom: a wrapper that looks like a plain T at the
// call site (thanks to an implicit conversion operator) but throws at the
// point of conversion if the underlying operation actually failed.
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T>
class exploding_return {
    bool ok_;
    T value_{};
    std::string error_;
public:
    exploding_return(T v)
        : ok_(true), value_(std::move(v)) {}

    exploding_return(std::string err)
        : ok_(false), error_(std::move(err)) {}

    // The "explosion": converting to T throws if construction was from an
    // error message instead of a value.
    operator T() {
        if (!ok_)
            throw std::runtime_error(error_);
        return std::move(value_);
    }
};

exploding_return<int> parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::string("Cannot parse '") + s + "'";
    }
}

int main() {
    int x = parse_int("42");
    std::cout << "Parsed: " << x << "\n";

    try {
        int y = parse_int("not a num"); // throws inside the conversion
        std::cout << "Parsed: " << y << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
