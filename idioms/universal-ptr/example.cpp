// Universal Pointer idiom: UniversalPtr<T> wraps std::shared_ptr<T> with
// either a no-op deleter (observing a raw pointer/reference it does not
// own) or a deleter that forwards to a captured unique_ptr deleter (taking
// over ownership from a unique_ptr).
#include <iostream>
#include <memory>
#include <utility>

template <typename T>
class UniversalPtr {
public:
    constexpr UniversalPtr() noexcept = default;
    constexpr UniversalPtr(std::nullptr_t) noexcept {}

    // Observing: wraps a raw pointer without taking ownership of it.
    explicit UniversalPtr(T* raw_ptr) noexcept
        : ptr_(raw_ptr, &NoDelete), owning_(false) {}

    // Observing: wraps the address of a referenced object.
    explicit UniversalPtr(T& obj) noexcept
        : ptr_(&obj, &NoDelete), owning_(false) {}

    // Owning: takes over a unique_ptr's ownership (and its deleter).
    template <typename D>
    UniversalPtr(std::unique_ptr<T, D> owned)
        : ptr_(owned.get(), [d = owned.get_deleter()](T* p) mutable { d(p); })
        , owning_(true) {
        owned.release(); // ptr_'s captured deleter now owns destruction
    }

    T* Get() const noexcept { return ptr_.get(); }
    T& operator*() const { return *ptr_; }
    T* operator->() const noexcept { return ptr_.get(); }
    explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

    // Whether this UniversalPtr will destroy its pointee when the last
    // reference to it goes away. Not observable from shared_ptr itself in
    // any simpler portable way, so tracked explicitly.
    bool Owns() const noexcept { return owning_; }

private:
    static void NoDelete(T*) noexcept {}

    std::shared_ptr<T> ptr_;
    bool owning_ = false;
};

struct Widget {
    int id_;
    explicit Widget(int i) : id_(i) { std::cout << "Widget(" << id_ << ") constructed\n"; }
    ~Widget() { std::cout << "Widget(" << id_ << ") destroyed\n"; }
};

int main() {
    Widget stack_widget(1);
    {
        UniversalPtr<Widget> observing(&stack_widget);
        std::cout << "observing.owns()=" << std::boolalpha << observing.Owns()
                  << " id=" << observing->id_ << "\n";
    } // observing destroyed here; stackWidget must NOT be destroyed by it

    std::cout << "stackWidget still alive: id=" << stack_widget.id_ << "\n";

    {
        UniversalPtr<Widget> owning(std::make_unique<Widget>(2));
        std::cout << "owning.owns()=" << std::boolalpha << owning.Owns()
                  << " id=" << owning->id_ << "\n";
    } // owning destroyed here; Widget(2) IS destroyed as part of this

    std::cout << "done\n";
    return 0;
}
