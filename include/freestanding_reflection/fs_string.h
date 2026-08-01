#ifndef FREESTANDING_METASTRING_H
#define FREESTANDING_METASTRING_H

#include <cstddef>
#include <new>
#include <utility>
#include <version>

#ifdef __cpp_lib_filesystem
#include <string>
#else

namespace std {
    template<typename T>
    class basic_string {
    public:
        consteval basic_string() = default;

        consteval basic_string(const T* str) {
            std::size_t len = 0;
            while (str[len] != '\0') ++len;
            assign(str, len);
        }

        consteval basic_string(const T* str, std::size_t len) {
            assign(str, len);
        }

        template<typename Range>
        requires (same_as<T, ranges::range_value_t<Range>>)
        consteval basic_string(const Range& range) {
            for (auto&& c : range)
                push_back(c);
        }

        consteval basic_string(const basic_string& other) {
            assign(other._data, other._size);
        }

        consteval basic_string(basic_string&& other) noexcept {
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            other._data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }

        consteval basic_string& operator=(const basic_string& other) {
            if (this != &other) {
                clear();
                assign(other._data, other._size);
            }
            return *this;
        }

        consteval basic_string& operator=(basic_string&& other) noexcept {
            if (this != &other) {
                clear();
                if (_data) __builtin_operator_delete(_data);
                _data = other._data; _size = other._size; _capacity = other._capacity;
                other._data = nullptr; other._size = 0; other._capacity = 0;
            }
            return *this;
        }

        constexpr ~basic_string() {
            clear();
            if (_data)
                __builtin_operator_delete(_data);
        }

        consteval void reserve(std::size_t n) {
            // +1 for the null terminator, kept internal so callers never
            // have to think about it
            if (n + 1 > _capacity)
                grow(n + 1);
        }

        consteval void push_back(T c) {
            if (_size + 1 >= _capacity)
                grow(_size + 2);
            _data[_size] = c;
            ++_size;
            _data[_size] = '\0';
        }

        consteval basic_string& operator+=(const basic_string& other) {
            reserve(_size + other._size);
            for (std::size_t i = 0; i < other._size; i++)
                push_back(other._data[i]);
            return *this;
        }

        consteval basic_string& operator+=(const T* str) {
            while (*str) push_back(*str++);
            return *this;
        }

        constexpr void clear() {
            _size = 0;
            if (_data) _data[0] = '\0';
        }

        consteval std::size_t size() const noexcept { return _size; }
        consteval bool empty() const noexcept { return _size == 0; }
        consteval const T* data() const noexcept { return _data ? _data : (const T*)""; }
        consteval const T* c_str() const noexcept { return data(); }

        consteval T& operator[](std::size_t index) { return _data[index]; }
        consteval const T& operator[](std::size_t index) const { return _data[index]; }

        consteval const T* begin() const { return data(); }
        consteval const T* end() const { return data() + _size; }

        consteval bool operator==(const basic_string& other) const {
            if (_size != other._size) return false;
            for (std::size_t i = 0; i < _size; i++)
                if (_data[i] != other._data[i]) return false;
            return true;
        }

    private:
        T* _data = nullptr;
        std::size_t _size     = 0;
        std::size_t _capacity = 0;

        consteval void assign(const T* str, std::size_t len) {
            reserve(len);
            for (std::size_t i = 0; i < len; i++)
                push_back(str[i]);
        }

        consteval void grow(std::size_t n) {
            auto newCapacity = _capacity ? _capacity * 2 : 8;
            if (newCapacity < n)
                newCapacity = n;

            T* newData = static_cast<T*>(__builtin_operator_new(newCapacity));
            for (std::size_t i = 0; i < _size; i++)
                newData[i] = _data[i];
            newData[_size] = '\0';

            if (_data)
                __builtin_operator_delete(_data);

            _data = newData;
            _capacity = newCapacity;
        }
    };

    using string = basic_string<char>;
    using u8string = basic_string<char8_t>;
}

#endif

#endif //FREESTANDING_METASTRING_H