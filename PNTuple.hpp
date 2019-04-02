#ifndef PACKED_NAMED_TUPLE_HPP__
#define PACKED_NAMED_TUPLE_HPP__

#include <tuple>
#include <utility>

#include "Assert.hpp"
#include "Bool.hpp"
#include "MergesortList.hpp"
#include "String.hpp"

// Helpers for tuple creation {{
template<typename N, typename T>
struct TupleField;

template<typename T>
struct IsTupleField : False {};
template<typename N, typename T>
struct IsTupleField<TupleField<N, T>> : True {};

#define FIELD(N, T) TupleField<METASTRING(N), T>
// }} Helpers

// Wrappers for getters {{
template<typename>
struct TypeWrap {};

template<std::size_t>
struct IndexWrap {};
// }} Wrappers

// Single tuple element {{
// Non empty base class optimizable element
template<typename N, typename T, typename I>
struct PNTupleElem {
private:
  T t;

public:
  constexpr PNTupleElem(): t() {}
  template<typename U>
  constexpr PNTupleElem(U &&u): t(std::forward<U>(u)) {}

  // Indexed getter
  constexpr T &get(I &&) noexcept { return t; }
  constexpr const T &get(I &&) const noexcept { return t; }

  // Typed getter
  constexpr T &get(TypeWrap<T> &&) noexcept { return t; }
  constexpr const T&get(TypeWrap<T> &&) const noexcept { return t; }

  // Named getter
  constexpr T &get(N &&) noexcept { return t; }
  constexpr const T &get(N &&) const noexcept { return t; }
};

// Element with empty base class optimization
template<typename N, typename T, typename I>
struct PNTupleElemEmpty : private T {
  constexpr PNTupleElemEmpty() = default;
  template<typename U>
  constexpr PNTupleElemEmpty(U &&u): T(std::forward<U>(u)) {}

  // Indexed getter
  constexpr T &get(I &&) noexcept { return *this; }
  constexpr const T &get(I &&) const noexcept { return *this; }

  // Typed getter
  constexpr T &get(TypeWrap<T> &&) noexcept { return *this; }
  constexpr const T&get(TypeWrap<T> &&) const noexcept { return *this; }

  // Named getter
  constexpr T &get(N &&) noexcept { return *this; }
  constexpr const T &get(N &&) const noexcept { return *this; }
};

// Dispatcher
template<typename N, typename T, typename I>
struct PNTupleElemGeneral :
  std::conditional_t<std::is_empty_v<T> && !std::is_final_v<T>,
                     PNTupleElemEmpty<N, T, I>, PNTupleElem<N, T, I>> {
  using Base =
    std::conditional_t<std::is_empty_v<T> && !std::is_final_v<T>,
                       PNTupleElemEmpty<N, T, I>, PNTupleElem<N, T, I>>;

  using Base::get;
  
  constexpr PNTupleElemGeneral() = default;
  template<typename U>
  constexpr PNTupleElemGeneral(U &&u): Base(std::forward<U>(u)) {}
};
// }} Tuple element

// Intermediate base with sorted tuple elements {{
template<typename SE, typename E>
struct PNTupleImplSorted;

// Seq<SEs...> is sorted, Seq<Es...> is in initial order.
// Order of elements in Es... corresponds with what user will pass to constructor.
template<typename...SEs, typename...Es>
struct PNTupleImplSorted<Seq<SEs...>, Seq<Es...>> : SEs... {
  using Es::get...;

  constexpr PNTupleImplSorted() = default;
  template<typename...Vs>
  constexpr PNTupleImplSorted(Vs&&...vs): Es(std::forward<Vs>(vs))... {}
};
// }} Sorted base

// Comparator by size of type {{
template<typename A, typename B>
struct SizeOfCmp;

template<typename AN, typename AT, typename AI,
         typename BN, typename BT, typename BI>
struct SizeOfCmp<PNTupleElemGeneral<AN, AT, AI>,
                 PNTupleElemGeneral<BN, BT, BI>> {
  static constexpr bool Empty = std::is_empty_v<AT> && !std::is_final_v<AT>;
  using Type = ToBoolT<Empty || (sizeof(AT) <= sizeof(BT))>;
};
// }} Comparator

// Intermediate base with unsorted tuple elements {{
template<typename SE>
struct PNTupleImpl;

template<typename...Es>
struct PNTupleImpl<Seq<Es...>>
  : PNTupleImplSorted<FromListT<MetaSortByT<SizeOfCmp, ToListT<Seq<Es...>>>>, Seq<Es...>> {
  using Base = PNTupleImplSorted<FromListT<MetaSortByT<SizeOfCmp, ToListT<Seq<Es...>>>>, Seq<Es...>>;

  using Base::get;

  constexpr PNTupleImpl() = default;
  template<typename...Vs>
  constexpr PNTupleImpl(Vs&&...vs): Base(std::forward<Vs>(vs)...) {}
};
// }} Unsorted base

// Turn index sequence into sequence of index wraps {{
template<std::size_t...Is>
auto wrapIndices(std::index_sequence<Is...> &&) -> Seq<IndexWrap<Is>...>;
// }} Index wrapper

// Zipper to make tuple elements from tuple fields and index wrappers {{
template<typename A, typename B>
struct ZipElem;

template<typename N, typename T, typename I>
struct ZipElem<TupleField<N, T>, I> {
  using Type = PNTupleElemGeneral<N, T, I>;
};
// }} Zipper

// Main packed named tuple class {{
template<typename...Fs>
struct PNTuple :
  Assert<IsTupleField<Fs>...>,
  PNTupleImpl<ZipWithT<ZipElem,
                       Seq<Fs...>,
                       decltype(wrapIndices(std::make_index_sequence<sizeof...(Fs)>()))>> {
  using Base =
    PNTupleImpl<ZipWithT<ZipElem, Seq<Fs...>,
                         decltype(wrapIndices(std::make_index_sequence<sizeof...(Fs)>()))>>;

  using Base::get;

  constexpr PNTuple() = default;
  template<typename...Vs>
  constexpr PNTuple(Vs&&...vs) : Base(std::forward<Vs>(vs)...) {}

  // Pretty named getters
  template<typename N>
  constexpr auto &operator[](N &&Name) { return get(std::move(Name)); }
  template<typename N>
  constexpr auto &operator[](N &&Name) const { return get(std::move(Name)); }
};
// }} Packed named tuple

// Getters {{
// Indexed getters
template<std::size_t I, typename...Ts, typename...Ns>
constexpr auto &Get(PNTuple<TupleField<Ts, Ns>...> &T) noexcept {
  return T.get(IndexWrap<I>());
};

template<std::size_t I, typename...Ts, typename...Ns>
constexpr auto &Get(const PNTuple<TupleField<Ts, Ns>...> &T) noexcept {
  return T.get(IndexWrap<I>());
};

template<std::size_t I, typename...Ts, typename...Ns>
constexpr auto &&Get(PNTuple<TupleField<Ts, Ns>...> &&T) noexcept {
  using Type = std::remove_reference_t<decltype(T.get(IndexWrap<I>()))>;
  return std::forward<Type &&>(T.get(IndexWrap<I>()));
};

template<std::size_t I, typename...Ts, typename...Ns>
constexpr auto &&Get(const PNTuple<TupleField<Ts, Ns>...> &&T) noexcept {
  using Type = std::remove_reference_t<decltype(T.get(IndexWrap<I>()))>;
  return std::forward<Type &&>(T.get(IndexWrap<I>()));
};
// }} Indexed getters

// Typed getters {{
template<typename T, typename...Ts, typename...Ns>
constexpr auto &Get(PNTuple<TupleField<Ts, Ns>...> &Tpl) noexcept {
  return Tpl.get(TypeWrap<T>());
};

template<typename T, typename...Ts, typename...Ns>
constexpr auto &Get(const PNTuple<TupleField<Ts, Ns>...> &Tpl) noexcept {
  return Tpl.get(TypeWrap<T>());
};

template<typename T, typename...Ts, typename...Ns>
constexpr auto &&Get(PNTuple<TupleField<Ts, Ns>...> &&Tpl) noexcept {
  using Type = std::remove_reference_t<decltype(Tpl.get(TypeWrap<T>()))>;
  return std::forward<Type &&>(Tpl.get(TypeWrap<T>()));
};

template<typename T, typename...Ts, typename...Ns>
constexpr auto &&Get(const PNTuple<TupleField<Ts, Ns>...> &&Tpl) noexcept {
  using Type = std::remove_reference_t<decltype(Tpl.get(TypeWrap<T>()))>;
  return std::forward<Type &&>(Tpl.get(TypeWrap<T>()));
};
// }} Typed getters
// }} Getters

// Tuple size {{
template<typename T>
struct PNTupleSize;

template<typename...Fs>
struct PNTupleSize<PNTuple<Fs...>>
  : std::integral_constant<std::size_t, sizeof...(Fs)> {};

template<typename T>
constexpr std::size_t PNTupleSizeV = PNTupleSize<T>::value;
// }} Tuple size

// Relational operators {{
template<typename A, typename B, std::size_t...Is>
constexpr bool tupleEqual(A Lhs, B Rhs, std::index_sequence<Is...> &&) {
  return (true && ... && (Get<Is>(Lhs) == Get<Is>(Rhs)));
}

template<typename...AFs, typename...BFs>
constexpr bool operator==(const PNTuple<AFs...> &Lhs,
                          const PNTuple<BFs...> &Rhs) {
  return tupleEqual(Lhs, Rhs, std::make_index_sequence<sizeof...(AFs)>());
}

template<typename...AFs, typename...BFs>
constexpr bool operator!=(const PNTuple<AFs...> &Lhs,
                          const PNTuple<BFs...> &Rhs) {
  return !(Lhs == Rhs);
}

template<std::size_t I, typename A, typename B>
constexpr bool tupleLess(A Lhs, B Rhs) {
  if (Get<I>(Lhs) < Get<I>(Rhs))
    return true;
  if (Get<I>(Rhs) < Get<I>(Lhs))
    return false;
  if constexpr (I + 1 < PNTupleSizeV<A>)
    return tupleLess<I + 1>(Lhs, Rhs);
  else
    return false;
}

template<typename...AFs, typename...BFs>
constexpr bool operator<(const PNTuple<AFs...> &Lhs,
                         const PNTuple<BFs...> &Rhs) {
  return tupleLess<0>(Lhs, Rhs);
}

template<>
constexpr bool operator<(const PNTuple<> &,
                         const PNTuple<> &) {
  return false;
}

template<typename...AFs, typename...BFs>
constexpr bool operator>=(const PNTuple<AFs...> &Lhs,
                          const PNTuple<BFs...> &Rhs) {
  return !(Lhs < Rhs);
}

template<typename...AFs, typename...BFs>
constexpr bool operator>(const PNTuple<AFs...> &Lhs,
                         const PNTuple<BFs...> &Rhs) {
  return Rhs < Lhs;
}

template<typename...AFs, typename...BFs>
constexpr bool operator<=(const PNTuple<AFs...> &Lhs,
                          const PNTuple<BFs...> &Rhs) {
  return !(Rhs < Lhs);
}
// }} Relational operators

// Make tuple {{
template<typename T>
struct DeducePNTypeImpl {
  using Type = T;
};

template<typename T>
struct DeducePNTypeImpl<std::reference_wrapper<T>> {
  using Type = T &;
};

template<typename T>
struct DeducePNType {
  using Type = typename DeducePNTypeImpl<std::decay_t<T>>::Type;
};

template<std::size_t...Is, typename...Ts>
constexpr auto makePNTupleImpl(std::index_sequence<Is...> &&, Ts&&...ts) {
  auto T = std::forward_as_tuple(std::forward<Ts>(ts)...);
  return PNTuple<TupleField<std::remove_reference_t<decltype(std::get<Is * 2>(T))>,
                            typename DeducePNType<decltype(std::get<Is * 2 + 1>(T))>::Type>...>
    (std::get<Is * 2 + 1>(std::move(T))...);
}

template<typename...Ts>
constexpr auto makePNTuple(Ts&&...ts) {
  static_assert(sizeof...(Ts) % 2 == 0, "Bad tuple constructor");
  return makePNTupleImpl(std::make_index_sequence<sizeof...(Ts) / 2>(), std::forward<Ts>(ts)...);
}
// }} Make tuple

#endif
