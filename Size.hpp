#ifndef META_SIZE_HPP__
#define META_SIZE_HPP__

#include <cstddef>

// Size {{
template<typename T>
struct Size;

template<typename L>
static constexpr std::size_t SizeV = Size<L>::Value;
// }} Size

#endif
