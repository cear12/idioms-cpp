// Type Erasure idiom: a minimal, educational std::any-alike. Any concrete
// type is wrapped behind the non-template IHolder interface at
// construction time, and can be recovered later via a checked cast<T>().
#include <iostream>
#include <memory>
#include <typeinfo>
#include <string>
#include <stdexcept>

class Any {
public:
    struct IHolder {
        virtual ~IHolder() = default;
        virtual std::unique_ptr<IHolder> clone() const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template <typename T>
    struct Holder : IHolder {
        explicit Holder(const T& value) : value_(value) {}
        explicit Holder(T&& value) : value_(std::move(value)) {}

        std::unique_ptr<IHolder> clone() const override {
            return std::make_unique<Holder<T>>(value_);
        }
        const std::type_info& type() const override {
            return typeid(T);
        }

        T value_;
    };

    Any() = default;

    template <typename T>
    Any(T value) : holder_(std::make_unique<Holder<T>>(std::move(value))) {}

    Any(const Any& other)
        : holder_(other.holder_ ? other.holder_->clone() : nullptr) {}

    Any& operator=(const Any& other) {
        if (this != &other) {
            holder_ = other.holder_ ? other.holder_->clone() : nullptr;
        }
        return *this;
    }

    bool hasValue() const noexcept { return holder_ != nullptr; }

    const std::type_info& type() const noexcept {
        return holder_ ? holder_->type() : typeid(void);
    }

    template <typename T>
    T& cast() {
        if (!holder_ || holder_->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<Holder<T>*>(holder_.get())->value_;
    }

    template <typename T>
    const T& cast() const {
        if (!holder_ || holder_->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<const Holder<T>*>(holder_.get())->value_;
    }

private:
    std::unique_ptr<IHolder> holder_;
};

int main() {
    Any a = 42;
    Any b = std::string("hello");

    std::cout << "Type of a: " << a.type().name() << "\n";
    std::cout << "Type of b: " << b.type().name() << "\n";

    std::cout << "a contains int: " << a.cast<int>() << "\n";
    std::cout << "b contains string: " << b.cast<std::string>() << "\n";

    try {
        a.cast<std::string>(); // throws: a actually holds an int
    } catch (const std::bad_cast&) {
        std::cout << "Caught bad_cast as expected\n";
    }

    return 0;
}
