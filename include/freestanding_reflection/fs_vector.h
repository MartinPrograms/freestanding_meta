#ifndef FREESTANDING_METAVECTOR_H
#define FREESTANDING_METAVECTOR_H

#include <cstddef>
#include <new>
#include <initializer_list>
#include <utility>
#include <version>

// The vector *has* to be in the std:: namespace, overwriting it does not really work, so this check just makes sure theres only one std::vector at a time

#ifdef __cpp_lib_filesystem
#include <vector>
#else

namespace std {
    template<typename T>
    class vector {
    public:
        consteval vector() = default;
        consteval vector(std::initializer_list<T> init) {
            reserve(init.size());
            for (auto&& item : init)
                push_back(item);
        }

        consteval vector(const vector& other) {
            reserve(other._size);
            for (std::size_t i = 0; i < other._size; i++)
                push_back(other._data[i]);
        }

        consteval vector& operator=(vector&& other) noexcept {
            if (this != &other) {
                clear();
                if (_data) ::operator delete(_data);
                _data = other._data; _size = other._size; _capacity = other._capacity;
                other._data = nullptr; other._size = 0; other._capacity = 0;
            }
            return *this;
        }

        consteval vector& operator=(const vector& other) {
            if (this != &other) {
                clear();
                reserve(other._size);
                for (std::size_t i = 0; i < other._size; i++)
                    push_back(other._data[i]);
            }
            return *this;
        }

        constexpr ~vector() {
            clear();
            if (_data)
                __builtin_operator_delete(_data);
        }

        consteval void reserve(std::size_t n) {
            if (n > _capacity)
                grow(n);
        }

        consteval void push_back(const T& item) {
            if (_size >= _capacity)
                grow(_size + 1);
            ::new (_data + _size) T(item);
            ++_size;
        }

        consteval void push_back(T&& item) {
            if (_size >= _capacity)
                grow(_size + 1);
            ::new (_data + _size) T(std::move(item));
            ++_size;
        }

        consteval void clear() {
            for (std::size_t i = 0; i < _size; i++)
                _data[i].~T();
            _size = 0;
        }

        consteval std::size_t size() const noexcept { return _size; }
        consteval bool empty() const noexcept { return _size == 0; }

        consteval T& operator[](std::size_t index) { return _data[index]; }
        consteval const T& operator[](std::size_t index) const { return _data[index]; }

        consteval T* begin() { return _data; }
        consteval T* end() { return _data + _size; }
        consteval const T* begin() const { return _data; }
        consteval const T* end() const { return _data + _size; }

    private:
        T* _data = nullptr;
        std::size_t _size = 0;
        std::size_t _capacity = 0;

        consteval void grow(std::size_t n) {
            auto newCapacity = _capacity ? _capacity * 2 : 1;
            if (newCapacity < n)
                newCapacity = n;
            T* newData = static_cast<T*>(__builtin_operator_new(newCapacity * sizeof(T)));
            for (std::size_t i = 0; i < _size; i++) {
                ::new (newData + i) T(std::move(*(_data + i)));
                _data[i].~T();
            }
            if (_data)
                __builtin_operator_delete(_data);
            _data = newData;
            _capacity = newCapacity;
        }
    };
}

#endif

#endif //FREESTANDING_METAVECTOR_H
