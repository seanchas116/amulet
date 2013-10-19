#include <gtest/gtest.h>
#include <amulet/range_extension.hh>

TEST(RangeExtension, each)
{
  int sum = 0;
  Amulet::RangeExtension<std::vector<int>> xs = {1, 3, 7, 10};
  xs.each([&](int x){
    sum += x;
  });
  EXPECT_EQ(21, sum);
}