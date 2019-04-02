#ifndef META_FOLDR_HPP__
#define META_FOLDR_HPP__

// Right fold {{
template<template<typename, typename> typename F, typename A, typename L>
struct FoldR;

template<template<typename, typename> typename F, typename A, typename L>
using FoldRT = typename FoldR<F, A, L>::Type;
// }} Right fold

#endif
