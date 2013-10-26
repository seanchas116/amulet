#include <gtest/gtest.h>
#include <amulet/comprehension_macro.hh>
#include <amulet/option.hh>

TEST(ComprehensionMacro, test)
{
  auto divide = [](int x, int y) -> Amulet::Option<int>{
    if (y)
      return Amulet::some(x / y);
    else
      return Amulet::none;
  };
  auto a = Amulet::some(0);
  auto b = Amulet::some(1);

  auto r1 = AMULET_DO(
    AMULET_FROM(x, b),
    AMULET_FROM(y, a),
    AMULET_LET(z, 10),
    divide(y + z, x)
  );
  
  EXPECT_EQ(10, r1.get());
}