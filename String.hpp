#ifndef META_STRING_HPP__
#define META_STRING_HPP__

#include <utility>

#include "Seq.hpp"

template<typename T, T...Cs>
constexpr auto operator""_nt() -> Seq<std::integral_constant<T, Cs>...> { return {}; }

#define METASTRING(STR) decltype(STR ""_nt)

#endif
