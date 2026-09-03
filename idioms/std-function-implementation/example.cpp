// std::function, Reimplemented: Function<R(Args...)> type-erases any
// callable behind ICallable/CallableImpl<F>, the same interface-plus-
// templated-implementation shape std::function itself uses.
#include <iostream>
#include <memory>
#include <utility>

template <typename Signature>
class Function; // primary template intentionally undefined; only the
                 // R(Args...) specialization below is usable

template <typename R, typename... Args>
class Function<R(Args...)> {
    struct ICallable {
        virtual R Invoke(Args... args) = 0;
        virtual ~ICallable() = default;
    };

    template <typename F>
    struct CallableImpl : ICallable {
        F f_;
        explicit CallableImpl(F f) : f_(std::move(f)) {}
        R Invoke(Args... args) override { return f_(std::forward<Args>(args)...); }
    };

public:
    Function() = default;

    template <typename F>
    Function(F f) : callable_(std::make_unique<CallableImpl<F>>(std::move(f))) {}

    R operator()(Args... args) const {
        return callable_->Invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return static_cast<bool>(callable_); }

private:
    std::unique_ptr<ICallable> callable_;
};

int Subtract(int a, int b) { return a - b; }

struct Multiplier {
    int factor_;
    int operator()(int x) const { return x * factor_; }
};

int main() {
    Function<int(int, int)> f1 = [](int a, int b) { return a + b; }; // lambda
    std::cout << "f1(10, 3) = " << f1(10, 3) << "\n";

    Function<int(int, int)> f2 = Subtract; // plain function
    std::cout << "f2(10, 3) = " << f2(10, 3) << "\n";

    Function<int(int)> f3 = Multiplier{5}; // functor with state
    std::cout << "f3(4) = " << f3(4) << "\n";

    Function<void()> f4 = [] { std::cout << "f4 called (void return)\n"; };
    f4();

    return 0;
}
