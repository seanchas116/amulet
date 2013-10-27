#include <gtest/gtest.h>
#include <amulet/short_query_macro.hh>
#include <amulet/option.hh>
#include <amulet/range_extension.hh>

TEST(QueryMacro, option)
{
  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 = _do(
    _from(x, b),
    _from(y, a),
    _let(z, 10),
    _select(divide(y + z, x))
  );
  
  EXPECT_EQ(10, r1.get());
}

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(QueryMacro, range)
{
  auto xs = ExVector<int>{1,2};
  auto ys = ExVector<int>{3,4};
  auto zs = _do(
    _from(x, xs),
    _from(y, ys),
    _select(ExVector<int>{x, y})
  );
  auto expected = { 1, 3, 1, 4, 2, 3, 2, 4 };
  EXPECT_EQ(zs, expected);

}