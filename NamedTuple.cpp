#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#include "PNTuple.hpp"
#include "Printer.hpp"

struct Empty {};

int main() {
  using namespace std::string_literals;

  auto Tpl = makePNTuple("band"_nt, "QOTSA"s,
                         "members"_nt, 5,
                         "founder"_nt, "Josh Homme"s,
                         "albumsNumber"_nt, 6,
                         "empty"_nt, Empty());
  using STpl = std::tuple<std::string, int, std::string, int, Empty>;
  std::cout << "Packed tuple size is " << sizeof(Tpl)
            << "\nTuple size is " << sizeof(STpl) << std::endl;

  std::cout << "band = " << Tpl["band"_nt]
            << "\nmembers = " << Tpl["members"_nt]
            << "\nfounder = " << Tpl["founder"_nt]
            << "\nalbumsNumber = " << Tpl["albumsNumber"_nt]
            << std::endl;

  PNTuple<FIELD("length", int), FIELD("width", int)> R1(1, 2);
  auto R2 = makePNTuple("length"_nt, 1, "width"_nt, 2);

  static_assert(std::is_same_v<decltype(R1), decltype(R2)>);
  assert(R1 == R2);

  return 0;
}
