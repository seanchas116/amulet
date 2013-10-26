#include <gtest/gtest.h>
#include <amulet/int_range.hh>

TEST(IntRange, intRange)
{
  auto xs = Amulet::intRange(2, 5);
  auto expected = {2,3,4};
  EXPECT_EQ(xs, expected);
}