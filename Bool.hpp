#ifndef META_BOOL_HPP__
#define META_BOOL_HPP__

// Boolean support {{
struct False {
  using Type = False;
  constexpr operator bool() { return false; }
};

struct True {
  using Type = True;
  constexpr operator bool() { return true; }
};

template<bool B>
struct ToBool : True {};
template<>
struct ToBool<false> : False {};

template<bool B>
using ToBoolT = typename ToBool<B>::Type;
// }} Boolean support

// Bool predicate {{
template<typename T>
struct IsBool : False {};

template<> struct IsBool<True> : True {};
template<> struct IsBool<False> : True {};
// }} Bool predicate

#endif
