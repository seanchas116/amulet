
#include <amulet/short_query_macro.hh>
#include <amulet/range_extension.hh>
#include <vector>

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

int main()
{
  ExVector<std::pair<std::string, int>> prices = {{"orange", 50}, {"apple", 100}, {"banana", 150}, {"carrot",90}};

  prices.eachPair([](const std::string &name, int price){
    std::cout << "price of " << name << ": " << price << std::endl;
  });

  // comprehension like LINQ query expressions
  auto fruitPrices1 = _do(
    _from(pair, prices),
    _where(pair.first != "carrot"),
    _select(pair.second)
  );

  // method chaining
  auto fruitsPrices2 = prices.filter([](const std::pair<std::string, int> &pair){
    return pair.first != "carrot";
  }).values();

  return 0;
}

