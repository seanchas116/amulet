#include <amulet/int_range.hh>
#include <vector>
#include <iterator>

int main()
{
  auto fizzbuzz = Amulet::intRange(0, 100).map([](int x)->std::string{
    if (x % 15 == 0)
      return "fizzbuzz";
    else if (x % 3 == 0)
      return "fizz";
    else if (x % 5 == 0)
      return "buzz";
    else
      return std::to_string(x);
  });

  std::copy(fizzbuzz.begin(), fizzbuzz.end(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;
}
