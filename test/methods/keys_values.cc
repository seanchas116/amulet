#include <gtest/gtest.h>
#include <amulet/methods/keys.hh>
#include <amulet/methods/values.hh>
#include <vector>
#include <map>
#include <iostream>

TEST(methods, keys_values)
{
  std::map<int, std::string> hash = {{1, "one"}, {2, "two"}, {3, "three"}};
  
  std::vector<int> keys;
  std::vector<std::string> values;

  for (auto x : hash | Amulet::keys()) {
    keys.push_back(x);
  }

  for (auto x : hash | Amulet::values()) {
    values.push_back(x);
  }

  std::vector<int> expected_keys = {1, 2, 3};
  std::vector<std::string> expected_values = {"one", "two", "three"};

  EXPECT_EQ(expected_keys, keys);
  EXPECT_EQ(expected_values, values);
}
