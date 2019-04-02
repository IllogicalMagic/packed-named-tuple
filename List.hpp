#ifndef META_LIST_HPP__
#define META_LIST_HPP__

#include "Assert.hpp"
#include "Bool.hpp"
#include "Cons.hpp"
#include "FoldR.hpp"
#include "Size.hpp"
#include "ZipWith.hpp"

// List {{
struct Nil;

template<typename H, typename T>
struct List;
// }} List

// List predicate {{
template<typename L>
struct IsList : False {};

template<>
struct IsList<Nil> : True {};
template<typename H, typename T>
struct IsList<List<H, T>> : True {};
// }} List predicate

// Construct list {{
template<typename E, typename H, typename T>
struct Cons<E, List<H, T>> {
  using Type = List<E, List<H, T>>;
};

template<typename E>
struct Cons<E, Nil> {
  using Type = List<E, Nil>;
};
// }} Construct list

// List accessors {{
template<typename T>
struct Head;

template<typename H, typename T>
struct Head<List<H, T>> {
  using Type = H;
};

template<typename L>
using HeadT = typename Head<L>::Type;

template<typename T>
struct Tail;

template<typename H, typename T>
struct Tail<List<H, T>> {
  using Type = T;
};

template<typename L>
using TailT = typename Tail<L>::Type;
// }} List accessors

// Size of list {{
template<typename H, typename T>
struct Size<List<H, T>> {
  static constexpr std::size_t Value = 1 + Size<T>::Value;
};

template<>
struct Size<Nil> {
  static constexpr std::size_t Value = 0;
};
// }} Size of list

// Take first N elements from list {{
template<typename L, std::size_t N>
struct Take : Assert<IsList<L>> {
  using Type = List<HeadT<L>, typename Take<TailT<L>, N - 1>::Type>;
};

template<typename L>
struct Take<L, 0> : Assert<IsList<L>> {
  using Type = Nil;
};

template<typename L, std::size_t N>
using TakeT = typename Take<L, N>::Type;
// }} Take

// Drop first N elements from list {{
template<typename L, std::size_t N>
struct Drop : Assert<IsList<L>> {
  using Type = typename Drop<typename Tail<L>::Type, N - 1>::Type;
};

template<typename L>
struct Drop<L, 0> {
  using Type = L;
};

template<typename L, std::size_t N>
using DropT = typename Drop<L, N>::Type;
// }} Drop

// Right fold {{
template<template<typename, typename> typename F,
         typename A, typename H, typename T>
struct FoldR<F, A, List<H, T>> {
  using NewA = typename FoldR<F, A, T>::Type;
  using Type = typename F<H, NewA>::Type;
};

template<template<typename, typename> typename F, typename A>
struct FoldR<F, A, Nil> {
  using Type = A;
};
// }} Right fold

// Append {{
template<typename A, typename B>
struct Append : Assert<IsList<A>, IsList<B>> {
  using Type = FoldRT<Cons, B, A>;
};

template<typename A, typename B>
using AppendT = typename Append<A, B>::Type;
// }} Append

// ZipWith {{
template<template<typename, typename> typename F,
         typename AH, typename AT,
         typename BH, typename BT>
struct ZipWith<F, List<AH, AT>, List<BH, BT>>
  : Assert<ToBoolT<SizeV<AT> == SizeV<BT>>> {
  using H = typename F<AH, BH>::Type;
  using Type = List<H, typename ZipWith<F, AT, BT>::Type>;
};

template<template<typename, typename> typename F>
struct ZipWith<F, Nil, Nil> {
  using Type = Nil;
};
// }} ZipWith

#endif
