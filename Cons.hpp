#ifndef META_CONS_HPP__
#define META_CONS_HPP__

// Construct {{
template<typename E, typename L>
struct Cons;

template<typename E, typename L>
using ConsT = typename Cons<E, L>::Type;
// }} Construct list

#endif
