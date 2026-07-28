#include <freestanding_reflection/fs_meta.h>

enum Fruit {
    APPLE,
    PEAR,
    BANANA
};

template<typename E>
struct enum_entry {
    E value;
    std::string_view name;
};

template<typename E>
requires (std::is_enum_v<E>)
constexpr auto& Entries() {
    static constexpr auto entries = [] {
        static constexpr auto enumerators = std::define_static_array(std::meta::enumerators_of(^^E));
        std::array<enum_entry<E>, enumerators.size()> result{};
        size_t i = 0;
        template for (constexpr auto e : enumerators) {
            constexpr auto id = std::meta::identifier_of(e);
            constexpr auto staticName = std::define_static_string(id);
            result[i++] = enum_entry<E>{[:e:], std::string_view(staticName, id.size())};
        }
        return result;
    }();
    return entries;
}

int main() {
    for (auto& entry : Entries<Fruit>()) {
        printf("Fruit: %s, Value: %d\n", entry.name.data(), static_cast<int>(entry.value));
    }
}