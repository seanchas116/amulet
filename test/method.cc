
#include <amulet/method.h>
#include <gtest/gtest.h>

AMULET_METHOD2(sliced, size_t, start, size_t, length,
  template <typename T>
  T apply_(T &self) {
    T ret(length);
    std::copy(self.begin() + start, self.begin() + start + length, ret.begin());
    return ret;
  }
)

AMULET_METHOD1(at, size_t, index,
  template <typename T>
  auto apply_(T &self) -> decltype(self.at(index)) {
    return self.at(index);
  }
)

AMULET_METHOD0(size,
  template <typename T>
  auto apply_(T &self) -> decltype(self.size()) {
    return self.size();
  }
)

TEST(method, method) {
  std::vector<double> xs = {1.0, 1.5, 2.0};
  const auto &const_xs = xs;
  EXPECT_EQ(3, xs | size());
  EXPECT_EQ(1.0, const_xs | at(0));
  EXPECT_EQ(1.5, xs | at(1));
  std::vector<double> sub = {1.0, 1.5};
  EXPECT_EQ(sub, xs | sliced(0, 2));
}

