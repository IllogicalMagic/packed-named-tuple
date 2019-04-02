#include "MergesortList.hpp"
#include "Printer.hpp"

template<typename A, typename B>
struct SizeOfCmp {
  using Type = ToBoolT<sizeof(A) <= sizeof(B)>;
};

PRINT_TYPE(MetaSortByT<SizeOfCmp, Nil>);

PRINT_TYPE(MetaSortByT<SizeOfCmp, List<int, Nil>>);

PRINT_TYPE(MetaSortByT<SizeOfCmp, List<char, List<int, List<double, List<bool, List<short, Nil>>>>>>);
