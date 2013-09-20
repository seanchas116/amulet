#include <gtest/gtest.h>
#include <amulet/methods/map.hh>
#include <vector>
#include <iostream>

TEST(methods, map)
{
  std::vector<double> xs = {1.0, 1.5, 2.0};
  std::vector<double> ys;

  for (auto x : xs | Amulet::map([](double x){return x * x;})) {
    ys.push_back(x);
  }
  
  std::vector<double> expected = {1.0, 2.25, 4.0};
  EXPECT_EQ(expected, ys);
}
