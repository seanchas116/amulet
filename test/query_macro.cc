
#define BOOST_PP_VARIADICS 1

#include <amulet/option.hh>
#include <amulet/range_extension.hh>
#include <amulet/dsl/short_query_macro.hh>
#include <gtest/gtest.h>


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

  auto divided = _do(
    _from(x, b),
    _from(y, a),
    _let(z, 10),
    divide(x, y)
  );

  auto added1 = _do(
    _from(x, a),
    _from(y, b),
    _select(x + y)
  );

  auto added2 = _do(
    _from(x, a),
    _from(y, divided),
    _select(x + y)
  );
  
  EXPECT_EQ(false, added2.hasValue());
}

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(QueryMacro, range)
{
  auto xs = ExVector<int>{1,2};
  auto ys = ExVector<int>{3,4};
  
  auto result1 = _do(
    _from(x, xs),
    _from(y, ys),
    _where(y == 3),
    (ExVector<int>{x, y})
  );
  auto result2 = _do(
    _from(x, xs),
    _from(y, ys),
    _select(x + y)
  );
  
  auto expected1 = { 1, 3, 2, 3 };
  auto expected2 = { 4, 5, 5, 6 };
  EXPECT_EQ(expected1, result1);
  EXPECT_EQ(expected2, result2);
}

