#ifndef META_SORT_HPP__
#define META_SORT_HPP__

#include "Assert.hpp"
#include "Bool.hpp"
#include "If.hpp"
#include "List.hpp"

// Merge {{
template<template<typename, typename> typename Cmp, typename As, typename Xs, typename Ys>
struct MergeImpl {
  using Cond = typename Cmp<HeadT<Xs>, HeadT<Ys>>::Type;
  using Hd = IfT<Cond, HeadT<Xs>, HeadT<Ys>>;
  using Tl = typename IfT<Cond, Tail<Xs>, Tail<Ys>>::Type;
  using MIRest = typename IfT<Cond, MergeImpl<Cmp, As, Tl, Ys>, MergeImpl<Cmp, As, Xs, Tl>>::Type;
  using Type = List<Hd, MIRest>;
};

// Second list is over
template<template<typename, typename> typename Cmp, typename As, typename Xs>
struct MergeImpl<Cmp, As, Xs, Nil> {
  using Type = AppendT<As, Xs>;
};

// First list is over
template<template<typename, typename> typename Cmp, typename As, typename Ys>
struct MergeImpl<Cmp, As, Nil, Ys> {
  using Type = AppendT<As, Ys>;
};

// Both lists are ended
template<template<typename, typename> typename Cmp, typename As>
struct MergeImpl<Cmp, As, Nil, Nil> {
  using Type = As;
};

template<template<typename, typename> typename Cmp, typename A, typename B>
struct Merge : MergeImpl<Cmp, Nil, A, B> {};
// }} Merge

// Mergesort {{
template<template<typename, typename> typename Cmp, typename L>
struct MergeSort {
  using Lhs = typename MergeSort<Cmp, TakeT<L, SizeV<L> / 2>>::Type;
  using Rhs = typename MergeSort<Cmp, DropT<L, SizeV<L> / 2>>::Type;
  using Type = typename Merge<Cmp, Lhs, Rhs>::Type;
};

// One element
template<template<typename, typename> typename Cmp, typename H>
struct MergeSort<Cmp, List<H, Nil>> {
  using Type = List<H, Nil>;
};

// Zero elements
template<template<typename, typename> typename Cmp>
struct MergeSort<Cmp, Nil> {
  using Type = Nil;
};
// }} Mergesort

// Metasort {{
template<template<typename, typename> typename Cmp, typename L>
struct MetaSortBy : MergeSort<Cmp, L>, Assert<IsList<L>> {};

template<template<typename, typename> typename Cmp, typename L>
using MetaSortByT = typename MetaSortBy<Cmp, L>::Type;
// }} Metasort

#endif
