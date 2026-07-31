#ifndef FREESTANDING_METAOPTIONAL_H
#define FREESTANDING_METAOPTIONAL_H

#include <cstddef>
#include <new>
#include <utility>
#include <type_traits>
#include <version>

#ifdef __cpp_lib_filesystem
#include <optional>
#else

namespace std {
    struct nullopt_t {
        explicit consteval nullopt_t(int) {}
    };

    inline constexpr nullopt_t nullopt{0};

    template<typename T>
    class optional {
    public:
        consteval optional() noexcept {}
        consteval optional(nullopt_t) noexcept {}

        template<typename U = T>
        requires (!std::is_same_v<std::remove_cvref_t<U>, optional> && std::is_constructible_v<T, U&&>)
        consteval optional(U&& value) : _engaged(true) {
            ::new (_storage()) T(std::forward<U>(value));
        }

        consteval optional(const optional& other) : _engaged(other._engaged) {
            if (_engaged)
                ::new (_storage()) T(*other._storage());
        }

        consteval optional(optional&& other) noexcept : _engaged(other._engaged) {
            if (_engaged)
                ::new (_storage()) T(std::move(*other._storage()));
        }

        consteval optional& operator=(const optional& other) {
            if (this != &other) {
                reset();
                if (other._engaged) {
                    ::new (_storage()) T(*other._storage());
                    _engaged = true;
                }
            }
            return *this;
        }

        consteval optional& operator=(optional&& other) noexcept {
            if (this != &other) {
                reset();
                if (other._engaged) {
                    ::new (_storage()) T(std::move(*other._storage()));
                    _engaged = true;
                }
            }
            return *this;
        }

        constexpr ~optional() {
            reset();
        }

        consteval void reset() {
            if (_engaged) {
                _storage()->~T();
                _engaged = false;
            }
        }

        template<typename... Args>
        consteval T& emplace(Args&&... args) {
            reset();
            :: new (_storage()) T(std::forward<Args>(args)...);
            _engaged = true;
            return *_storage();
        }

        consteval bool has_value() const noexcept { return _engaged; }
        consteval explicit operator bool() const noexcept { return _engaged; }

        consteval T& operator*() {return *_storage(); }
        consteval const T& operator*() const { return *_storage(); }
        consteval T* operator->() { return _storage(); }
        consteval const T* operator->() const { return _storage(); }

        consteval T value_or(T fallback) const {
            return _engaged ? *_storage() : fallback;
        }

    private:
        union {
            char _dummy;
            T _value;
        };

        bool _engaged = false;
        consteval T* _storage() { return &_value; }
        consteval const T* _storage() const { return &_value; }
    };
}

#endif

#endif //FREESTANDING_METAOPTIONAL_H
