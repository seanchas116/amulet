#include <gtest/gtest.h>
#include <amulet/methods/with_index.hh>
#include <vector>
#include <iostream>

TEST(methods, with_index)
{
  std::vector<double> xs = {1.0, 1.5, 2.0};
  std::vector<std::pair<double, int>> ys;

  for (auto x : xs | Amulet::with_index()){
    ys.push_back(x);
  }

  std::vector<std::pair<double, int>> expected = {{1.0, 0}, {1.5, 1}, {2.0, 2}};
  EXPECT_EQ(expected, ys);
}
