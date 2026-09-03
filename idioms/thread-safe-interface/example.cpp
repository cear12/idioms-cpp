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
    void PushBack(const T& value) {
        std::lock_guard<std::mutex> lock(mtx_);
        data_.push_back(value);
    }

    size_t Size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_.size();
    }

    T Get(size_t index) const {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_.at(index);
    }

private:
    std::vector<T> data_;
    mutable std::mutex mtx_;
};

int main() {
    ThreadSafeVector<int> ts_vec;

    auto producer = [&ts_vec]() {
        for (int i = 0; i < 100; ++i) {
            ts_vec.PushBack(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    auto consumer = [&ts_vec]() {
        for (int i = 0; i < 20; ++i) {
            size_t s = ts_vec.Size();
            std::cout << "Size at step " << i << ": " << s << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };

    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();

    std::cout << "Final size: " << ts_vec.Size() << "\n";
    return 0;
}
