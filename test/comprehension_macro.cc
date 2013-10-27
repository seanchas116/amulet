#include <gtest/gtest.h>
#include <amulet/comprehension_macro.hh>
#include <amulet/option.hh>
#include <amulet/range_extension.hh>

TEST(ComprehensionMacro, option)
{
  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 = AMULET_DO(
    AMULET_FROM(x, b),
    AMULET_FROM(y, a),
    AMULET_LET(z, 10),
    divide(y + z, x)
  );
  
  EXPECT_EQ(10, r1.get());
}

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(ComprehensionMacro, range)
{
  auto xs = ExVector<int>{1,2};
  auto ys = ExVector<int>{3,4};
  auto zs = AMULET_DO(
    AMULET_FROM(x, xs),
    AMULET_FROM(y, ys),
    (ExVector<int>{x, y})
  );
  auto expected = { 1, 3, 1, 4, 2, 3, 2, 4 };
  EXPECT_EQ(zs, expected);

}