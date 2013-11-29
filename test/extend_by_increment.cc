#include <amulet/dsl/extend_by_increment.hh>
#include <gtest/gtest.h>

TEST(ExtendByIncrement, test)
{
  std::vector<int> xs = {1,2,3};
  auto mapped = xs++.map([](int x){
    return x * 2;
  });
  auto expected = {2,4,6};
  EXPECT_EQ(expected, mapped);
}