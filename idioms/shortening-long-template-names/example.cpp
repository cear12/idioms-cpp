// Shortening Long Template Names idiom: typedef, using-aliases, alias
// templates, and function/class-local aliases, each demonstrated in turn.
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

// ============================================================
// 1. typedef for one specific instantiation (pre-C++11 style)
// ============================================================
typedef std::unique_ptr<std::vector<std::string>> StringVecPtr;
typedef std::unordered_map<std::string, std::shared_ptr<std::string>> StringCache;

// ============================================================
// 2. using declarations (C++11+, preferred over typedef)
// ============================================================
using StringMap = std::unordered_map<std::string, std::string>;
using IntVector = std::vector<int>;
using EventHandler = std::function<void(const std::string&, int)>;

// ============================================================
// 3. Alias templates: parameterized aliases (C++11+, no typedef equivalent)
// ============================================================
template <typename T>
using UniquePtr = std::unique_ptr<T>;

template <typename T>
using Vector = std::vector<T>;

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

// ============================================================
// 4. Local aliases inside a class
// ============================================================
class DataProcessor {
public:
    using DataType = std::pair<std::string, double>;
    using DataContainer = std::vector<DataType>;

    void AddSample(const std::string& label, double value) {
        data_.push_back({label, value});
    }

    void ProcessData() const {
        // Local alias inside a function body.
        using ResultType = std::unordered_map<std::string, int>;
        ResultType counts;
        for (const auto& item : data_) counts[item.first]++;
        for (const auto& [key, count] : counts)
            std::cout << key << " has " << count << " sample(s)\n";
    }

private:
    DataContainer data_;
};

// Local aliases inside a template function.
template <typename Container>
void ProcessContainer(const Container& container) {
    using ValueType [[maybe_unused]] = typename Container::value_type;

    std::cout << "Size: " << container.size() << "\nElements: ";
    for (const auto& v : container) std::cout << v << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== typedef / using aliases ===\n";
    StringVecPtr strings = std::make_unique<std::vector<std::string>>();
    strings->push_back("Hello");
    strings->push_back("World");

    StringCache cache;
    cache["key1"] = std::make_shared<std::string>("value1");

    StringMap config;
    config["debug"] = "true";

    IntVector numbers = {1, 2, 3, 4, 5};
    std::cout << "strings=" << strings->size() << " cache=" << cache.size()
              << " config=" << config.size() << " numbers=" << numbers.size() << "\n";

    std::cout << "\n=== alias templates ===\n";
    UniquePtr<int> number = std::make_unique<int>(42);
    Vector<double> values = {1.1, 2.2, 3.3};
    Map<std::string, int> counts = {{"apples", 5}, {"oranges", 3}};
    Matrix<int> matrix = {{1, 2, 3}, {4, 5, 6}};
    std::cout << "number=" << *number << " values=" << values.size()
              << " counts=" << counts.size()
              << " matrix=" << matrix.size() << "x" << matrix[0].size() << "\n";

    std::cout << "\n=== DataProcessor (class-local aliases) ===\n";
    DataProcessor processor;
    processor.AddSample("temp", 21.5);
    processor.AddSample("temp", 22.0);
    processor.AddSample("humidity", 55.0);
    processor.ProcessData();

    std::cout << "\n=== function-local aliases ===\n";
    ProcessContainer(Vector<int>{1, 2, 3});
    ProcessContainer(Vector<std::string>{"Hello", "World"});

    return 0;
}
