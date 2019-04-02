#ifndef META_IF_HPP__
#define META_IF_HPP__

#include "Assert.hpp"
#include "Bool.hpp"

// Conditional {{
template<typename C, typename T, typename F>
struct If : Assert<IsBool<C>> {
  using Type = T;
};

template<typename T, typename F>
struct If<False, T, F> {
  using Type = F;
};

template<typename C, typename T, typename F>
using IfT = typename If<C, T, F>::Type;
// }} Conditional

#endif
