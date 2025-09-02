#ifndef SFINAE_H
#define SFINAE_H


#include <QDebug>


namespace sfinae
{
    // Определение "да" версии, если serialize существует
    template<typename T>
    auto test_serialize(int) -> decltype(std::declval<T>().serialize(), std::true_type());

    // Определение "нет" версии, если serialize не существует
    template<typename T>
    std::false_type test_serialize(...);

    // Используем decltype для создания типа, который будет либо std::true_type, либо std::false_type
    template<typename T>
    struct has_serialize : decltype(test_serialize<T>(0)) {};

    // Классы для тестирования
    class WithSerialize {
    public:
        void serialize() const {
            std::cout << "WithSerialize::serialize()" << std::endl;
        }
    };

    class WithoutSerialize {
    };

    // Функция для демонстрации
    template<typename T>
    typename std::enable_if<has_serialize<T>::value, void>::type call_serialize(const T& obj) {
        obj.serialize();
    }

    template<typename T>
    typename std::enable_if<!has_serialize<T>::value, void>::type call_serialize(const T& obj) {
        std::cout << "No serialize() function" << std::endl;
    }

    int main() {
        WithSerialize with;
        WithoutSerialize without;

        call_serialize(with);    // Вызовет serialize()
        call_serialize(without); // Выведет "No serialize() function"

        return 0;
    }

    ///////////////////
    ///
    // Базовый шаблон (false)
    template<typename T>
    struct is_pointer : std::false_type {};

    // Специализация для указателей (true)
    template<typename T>
    struct is_pointer<T*> : std::true_type {};

    int main() {
        std::cout << std::boolalpha; // Включаем вывод булевых значений как true/false
        std::cout << "int is a pointer: " << is_pointer<int>::value << "\n"; // Выведет: false
        std::cout << "int* is a pointer: " << is_pointer<int*>::value << "\n"; // Выведет: true
    }
}


#endif // SFINAE_H
