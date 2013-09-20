#include <gtest/gtest.h>
#include <amulet/methods/filter.hh>
#include <vector>
#include <cmath>
#include <iostream>

TEST(methods, filter)
{
  std::vector<double> xs = {1.0, 1.5, 2.0};
  std::vector<double> ys;

  for (auto x : xs | Amulet::filter([](double x){return x == std::floor(x);})) {
    ys.push_back(x);
  }
  
  std::vector<double> expected = {1.0, 2.0};
  EXPECT_EQ(expected, ys);
}