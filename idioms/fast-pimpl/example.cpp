// Fast Pimpl idiom: Wrapped lives inside Wrapper's own inline byte buffer
// (constructed there with placement new) instead of behind a separately
// heap-allocated pointer, while Wrapped's definition still stays fully
// hidden from anyone who only sees Wrapper's declaration.
#include <cstddef>  // std::max_align_t
#include <iostream>
#include <memory>  // std::launder
#include <new>

class Wrapper {
 public:
  Wrapper();
  ~Wrapper();
  Wrapper(const Wrapper&) = delete;
  Wrapper& operator=(const Wrapper&) = delete;

  void DoSomething();

 private:
  struct Wrapped;  // incomplete here -- callers of Wrapper never see it

  // Fixed-size inline storage for Wrapped. The size (64 bytes) is chosen
  // generously for this example; a real Fast Pimpl picks it based on the
  // actual implementation type and enforces it with the static_assert
  // below, in the .cpp file where Wrapped becomes complete.
  alignas(std::max_align_t) char storage_[64];

  Wrapped* GetImpl();
};

// --- Implementation, only visible in this translation unit.
struct Wrapper::Wrapped {
  int value_;
  Wrapped() : value_(42) {
    std::cout << "Wrapped constructed with value=" << value_ << "\n";
  }
  ~Wrapped() { std::cout << "Wrapped destructed\n"; }
  void Work() const { std::cout << "Work with value=" << value_ << "\n"; }
};

Wrapper::Wrapper() {
  static_assert(sizeof(Wrapped) <= sizeof(storage_),
                "Wrapped is too big for local storage");
  new (storage_)
      Wrapped();  // placement new: construct in Wrapper's own inline buffer
}

Wrapper::~Wrapper() { GetImpl()->~Wrapped(); }

Wrapper::Wrapped* Wrapper::GetImpl() {
  // std::launder is needed because `storage`'s declared type (char[64])
  // differs from the object now living in it (Wrapped); without it, an
  // optimizer would be allowed to assume the reinterpret_cast result
  // still points to a char array.
  return std::launder(reinterpret_cast<Wrapped*>(storage_));
}

void Wrapper::DoSomething() { GetImpl()->Work(); }

int main() {
  Wrapper w;
  w.DoSomething();
  return 0;
}
