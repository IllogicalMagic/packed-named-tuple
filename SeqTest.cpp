#include "Printer.hpp"
#include "Seq.hpp"

using S = Seq<int, bool>;

PRINT_TYPE(ToListT<S>);
PRINT_TYPE(FromListT<ToListT<S>>);
