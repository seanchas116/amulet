#include <gtest/gtest.h>
#include <amulet/range_extension.hh>
#include <amulet/option.hh>
#include <amulet/bind_operator.hh>

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(BindOperator, range1)
{
  ExVector<int> xs = {1,2,3};
  auto flatMapped = xs >>= [](int x) {
    return ExVector<int>{x,x};
  };
  auto mapped = xs >>= [](int x) {
    return x + 1;
  };
  auto expected_f = {1,1,2,2,3,3};
  auto expected_m = {2,3,4};
  EXPECT_EQ(flatMapped, expected_f);
  EXPECT_EQ(mapped, expected_m);
}

TEST(BindOperator, range2)
{
  ExVector<int> xs = {1,2,3};
  ExVector<int> ys = {4,5,6};
  auto result = xs >>= [&ys](int x){
    return ys >>= [x](int y) {
      return x * y;
    };
  };
  auto expected = { 4, 5, 6, 8, 10, 12, 12, 15, 18 };
  EXPECT_EQ(result, expected);
}

TEST(BindOperator, option)
{
  auto mul = [](int x, int y){
    return x * y;
  };

  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 = b >>= [=](int x){
    return a >>= [=](int y){
      return divide(x, y);
    };
  };

  auto r2 = b >>= [=](int x){
    return a >>= [=](int y){
      return mul(x, y);
    };
  };

  EXPECT_EQ(false, r1.hasValue());
  EXPECT_EQ(0, r2.get());
}

#define from(ELEM, MONAD, EXPR) (MONAD >>= [=](ELEM){ return EXPR; })

TEST(BindOperator, macro)
{
  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 =
    from(int x, b,
    from(int y, a,
    divide(x, y)));
}