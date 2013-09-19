
#include <amulet/method.hh>
#include <gtest/gtest.h>
#include <vector>
#include <initializer_list>

AMULET_METHOD(sliced, sliced,
  (2, ((size_t, start), (size_t, length))),
  template <typename T>
  T apply_(T &self) {
    T ret(length);
    std::copy(self.begin() + start, self.begin() + start + length, ret.begin());
    return ret;
  }
)

AMULET_METHOD(at, at,
  (1, ((size_t, index))),
  template <typename T>
  auto apply_(T &self) {
    return self.at(index);
  }
)

AMULET_METHOD(size, size,
  (0, ()),
  template <typename T>
  auto apply_(T &self) {
    return self.size();
  }
)

AMULET_METHOD_TEMPLATE(apply,apply,
  (1, ((typename, TUnaryFunc))),
  (1, ((TUnaryFunc, f))),
  template <typename T>
  auto apply_(T &self) {
    return f(self);
  }
)

TEST(method, method)
{
  std::vector<double> xs = {1.0, 1.5, 2.0};
  const auto &const_xs = xs;
  EXPECT_EQ(3, xs | size());
  EXPECT_EQ(1.0, const_xs | at(0));
  EXPECT_EQ(1.5, xs | at(1));
  std::vector<double> sub = {1.0, 1.5};
  EXPECT_EQ(sub, xs | sliced(0, 2));
}

TEST(method, method_template)
{
  double x = 1.5;
  double y = x | apply([](double x){return x * x;});
  EXPECT_EQ(1.5 * 1.5, y);
}
