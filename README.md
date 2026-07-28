# Freestanding meta

A very basic port of GCC16's `<meta>` to a freestanding environment.

If `__STDC_HOSTED__` is not true, it implements comptime versions of `std::vector`, `std::string`, `std::optional`, and `std::string_view`.
Otherwise it includes the appropriate system headers for those.

Replacing `std::vector` with any other namespace/typename breaks it, it does work for the others, but `std::vector` seems to be an internal compiler issue, so I kept the `std::` prefix for all others, for the sake of consistency.
I tried using `freestanding_meta::` prefixes for each type, except this broke `std::vector`, and only `std::vector`.

### Changes made to fs_meta.h:
- added at line `643` `__detail::__statically_sized` as a replacement for `ranges::__static_sized_range` since that was not found.
- changed includes `<optional>`, `<string>`, `<string_view>`, and `<vector>` to "fs_*," alternatives.  

### LICENSE
`LICENSE` covers every file **except** `fs_meta.h`  
`LICENSE.GCC` covers `fs_meta.h`