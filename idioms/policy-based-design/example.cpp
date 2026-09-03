// Policy-Based Design idiom: Container's allocation and logging strategy
// are both template parameters, resolved and (potentially) inlined at
// compile time rather than dispatched through virtual functions.
#include <iostream>
#include <new>
#include <string>

template <typename T>
struct DefaultAllocator {
    T* Allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void Deallocate(T* p) {
        ::operator delete(p);
    }
};

template <typename T>
struct ConsoleLogger {
    // const: Container::info() is const and calls log() through `this`,
    // so the policy method must be callable on a const Container.
    void log(const char* msg) const {
        std::cout << "[ConsoleLogger] " << msg << std::endl;
    }
};

template <typename T>
struct NoLogger {
    void log(const char*) const {}
};

template <
    typename T,
    template <typename> class AllocPolicy = DefaultAllocator,
    template <typename> class LogPolicy = NoLogger>
class Container : private AllocPolicy<T>, private LogPolicy<T> {
public:
    void Add(const T& value) {
        this->log("add()");
        T* p = this->Allocate(1);
        try {
            new (p) T(value);
        } catch (...) {
            this->Deallocate(p);
            throw;
        }
        p->~T();
        this->Deallocate(p);
    }

    void Info() const {
        this->log("info()");
    }
};

int main() {
    Container<int, DefaultAllocator, ConsoleLogger> c1;
    c1.Info();
    c1.Add(42);

    Container<std::string, DefaultAllocator, NoLogger> c2;
    c2.Info();
    c2.Add("Hello");
    return 0;
}
