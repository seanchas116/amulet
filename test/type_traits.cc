#include <amulet/type_traits.hh>
#include <vector>
#include <gtest/gtest.h>

TEST(TypeTraits, IsRange)
{
  static_assert(Amulet::IsRange<std::vector<int>>::value, "");
  static_assert(!Amulet::IsRange<double>::value, "");
}