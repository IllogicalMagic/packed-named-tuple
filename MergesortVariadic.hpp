#include <type_traits>
#include <utility>

template<typename...Ts>
struct Seq;

template<typename Xs, typename Ys>
struct HalfImplEnd {
  using First = Xs;
  using Second = Ys;
};

template<typename T, typename U, std::size_t S>
struct HalfImpl;

template<typename...Xs, typename Y, typename...Ys, std::size_t S>
struct HalfImpl<Seq<Xs...>, Seq<Y, Ys...>, S> :
  std::conditional_t<S == 0,
                     HalfImplEnd<Seq<Xs...>, Seq<Y, Ys...>>,
                     HalfImpl<Seq<Y, Xs...>, Seq<Ys...>, S - 1>> {};

template<typename T>
struct Half;

template<typename...Ts>
struct Half<Seq<Ts...>> : HalfImpl<Seq<>, Seq<Ts...>, sizeof...(Ts) / 2> {};

template<>
struct Half<Seq<>> {
  using First = Seq<>;
  using Second = Seq<>;
};

template<typename Acc, typename Xs, typename Ys>
struct MergeImpl;

template<typename...As, typename X, typename...Xs, typename Y, typename...Ys>
struct MergeImpl<Seq<As...>, Seq<X, Xs...>, Seq<Y, Ys...>> :
  std::conditional_t<(sizeof(X) < sizeof(Y)),
                       MergeImpl<Seq<As..., X>, Seq<Xs...>, Seq<Y, Ys...>>,
                       MergeImpl<Seq<As..., Y>, Seq<X, Xs...>, Seq<Ys...>>> {};

template<typename...As, typename...Xs>
struct MergeImpl<Seq<As...>, Seq<Xs...>, Seq<>> {
  using Type = Seq<As..., Xs...>;
};

template<typename...As, typename...Ys>
struct MergeImpl<Seq<As...>, Seq<>, Seq<Ys...>> {
  using Type = Seq<As..., Ys...>;
};

template<typename A, typename B>
struct Merge : MergeImpl<Seq<>, A, B> {};

template<typename T>
struct MergeSort;

template<typename...Ts>
struct MergeSort<Seq<Ts...>> :
  Merge<typename MergeSort<typename Half<Seq<Ts...>>::First>::Type,
        typename MergeSort<typename Half<Seq<Ts...>>::Second>::Type> {};

template<typename T>
struct MergeSort<Seq<T>> {
  using Type = Seq<T>;
};

using X = decltype(typename MergeSort<Seq<double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int,
                   double, char, bool, int, short, int
                   >>::Type())::X;
