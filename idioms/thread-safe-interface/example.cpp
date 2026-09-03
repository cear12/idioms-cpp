// Thread-Safe Interface idiom: ThreadSafeVector wraps std::vector<T> with
// a mutex so every public operation internally synchronizes; callers never
// touch a lock themselves. Link with -pthread.
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

template <typename T>
class ThreadSafeVector {
public:
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx_);
        data_.push_back(value);
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_.size();
    }

    T get(size_t index) const {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_.at(index);
    }

private:
    std::vector<T> data_;
    mutable std::mutex mtx_;
};

int main() {
    ThreadSafeVector<int> tsVec;

    auto producer = [&tsVec]() {
        for (int i = 0; i < 100; ++i) {
            tsVec.push_back(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    auto consumer = [&tsVec]() {
        for (int i = 0; i < 20; ++i) {
            size_t s = tsVec.size();
            std::cout << "Size at step " << i << ": " << s << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };

    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();

    std::cout << "Final size: " << tsVec.size() << "\n";
    return 0;
}
