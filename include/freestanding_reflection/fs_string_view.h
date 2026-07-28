#ifndef FREESTANDING_METASTRING_VIEW_H
#define FREESTANDING_METASTRING_VIEW_H

#include <cstddef>

#if __STDC_HOSTED__
#include <string_view>
#else

namespace std {
    template<typename CharT>
    class basic_string_view {
    public:
        consteval basic_string_view() = default;

        consteval basic_string_view(const CharT* str) : _data(str) {
            std::size_t len = 0;
            while (str[len] != CharT(0)) ++len;
            _size = len;
        }

        consteval basic_string_view(const CharT* str, std::size_t len)
            : _data(str), _size(len) {}

        // implicit on purpose -- covers fs_basic_string<CharT> (and anything
        // else with data()/size()) converting to a view, e.g. exception::u8what()
        // returning a u8string_view from a u8string member
        template<typename Str>
            requires requires(const Str& s) { s.data(); s.size(); }
        consteval basic_string_view(const Str& s) : _data(s.data()), _size(s.size()) {}

        consteval std::size_t size()  const noexcept { return _size; }
        consteval bool empty() const noexcept { return _size == 0; }
        consteval const CharT* data() const noexcept { return _data; }

        consteval const CharT& operator[](std::size_t index) const { return _data[index]; }

        consteval const CharT* begin() const { return _data; }
        consteval const CharT* end() const { return _data + _size; }

        consteval bool operator==(const basic_string_view& other) const {
            if (_size != other._size) return false;
            for (std::size_t i = 0; i < _size; i++)
                if (_data[i] != other._data[i]) return false;
            return true;
        }

    private:
        const CharT* _data = nullptr;
        std::size_t _size = 0;
    };

    using string_view = basic_string_view<char>;
    using u8string_view = basic_string_view<char8_t>;
}

#endif

#endif //FREESTANDING_METASTRING_VIEW_H
