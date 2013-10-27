#include <gtest/gtest.h>
#include <amulet/option.hh>

TEST(Option, filter)
{
  auto a = Amulet::some(0);
  auto b = a.filter([](int x){return x == 1;});
  EXPECT_EQ(false, b.hasValue());
}

TEST(Option, map)
{
  auto m = Amulet::some(10).map([](int x){
    return x * x;
  });
  EXPECT_EQ(100, m.get());
}

TEST(Option, flatMap)
{
  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 = b.flatMap([=](int x){
    return a.flatMap([=](int y){
      return divide(x, y);
    });
  });

  auto r2 = a.flatMap([=](int x){
    return b.flatMap([=](int y){
      return divide(x, y);
    });
  });

  EXPECT_EQ(false, r1.hasValue());
  EXPECT_EQ(0, r2.get());
}