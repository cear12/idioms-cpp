// Policy-Based Design idiom: Container's allocation and logging strategy
// are both template parameters, resolved and (potentially) inlined at
// compile time rather than dispatched through virtual functions.
#include <iostream>
#include <new>
#include <string>

template <typename T>
struct DefaultAllocator {
    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p) {
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
    void add(const T& value) {
        this->log("add()");
        T* p = this->allocate(1);
        try {
            new (p) T(value);
        } catch (...) {
            this->deallocate(p);
            throw;
        }
        p->~T();
        this->deallocate(p);
    }

    void info() const {
        this->log("info()");
    }
};

int main() {
    Container<int, DefaultAllocator, ConsoleLogger> c1;
    c1.info();
    c1.add(42);

    Container<std::string, DefaultAllocator, NoLogger> c2;
    c2.info();
    c2.add("Hello");
    return 0;
}
