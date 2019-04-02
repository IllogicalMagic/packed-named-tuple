#ifndef META_SEQ_HPP__
#define META_SEQ_HPP__

#include "Assert.hpp"
#include "Bool.hpp"
#include "Cons.hpp"
#include "FoldR.hpp"
#include "List.hpp"
#include "ZipWith.hpp"

// Sequence {{
template<typename...Ts>
struct Seq { };
// }} Sequence

// Sequence checker {{
template<typename T>
struct IsSeq : False {};
template<typename...Ts>
struct IsSeq<Seq<Ts...>> : True {};
// }} Sequence checker

// Construct sequence {{
template<typename E, typename...Ts>
struct Cons<E, Seq<Ts...>> {
  using Type = Seq<E, Ts...>;
};
// }} Construct

// Size {{
template<typename...Ts>
struct Size<Seq<Ts...>> {
  static constexpr std::size_t Value = sizeof...(Ts);
};
// }} Size

// FoldR for sequence {{
template<template<typename, typename> typename F, typename A, typename T, typename...Ts>
struct FoldR<F, A, Seq<T, Ts...>> {
  using NewA = typename FoldR<F, A, Seq<Ts...>>::Type;
  using Type = typename F<T, NewA>::Type;
};

template<template<typename, typename> typename F, typename A>
struct FoldR<F, A, Seq<>> {
  using Type = A;
};
// }} FoldR

// Make list from sequence {{
template<typename S>
struct ToList : Assert<IsSeq<S>> {
  using Type = FoldRT<Cons, Nil, S>;
};

template<typename S>
using ToListT = typename ToList<S>::Type;
// }} ToList

// Make sequence from list {{
template<typename L>
struct FromList : Assert<IsList<L>> {
  using Type = FoldRT<Cons, Seq<>, L>;
};

template<typename L>
using FromListT = typename FromList<L>::Type;
// }} Sequence from list

// ZipWith {{
template<template<typename, typename> typename F,
         typename X, typename...Xs,
         typename Y, typename...Ys>
struct ZipWith<F, Seq<X, Xs...>, Seq<Y, Ys...>>
  : Assert<ToBoolT<sizeof...(Xs) == sizeof...(Ys)>> {
  using Z = typename F<X, Y>::Type;
  using Type = ConsT<Z, typename ZipWith<F, Seq<Xs...>, Seq<Ys...>>::Type>;
};

template<template<typename, typename> typename F>
struct ZipWith<F, Seq<>, Seq<>> {
  using Type = Seq<>;
};
// }} ZipWith

#endif
