#ifndef META_PRINTER_HPP__
#define META_PRINTER_HPP__

template<typename T> [[deprecated]]
static constexpr bool PrintType = true;

#define PRINT_TYPE(...) static_assert(PrintType<__VA_ARGS__>)

#endif
