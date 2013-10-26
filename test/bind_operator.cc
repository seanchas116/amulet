#include <gtest/gtest.h>
#include <amulet/range_extension.hh>
#include <amulet/bind_operator.hh>

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(BindOperator, test)
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

TEST(BindOperator, comprehension)
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