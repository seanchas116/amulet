Amulet
======

Amulet is a header-only C++ library that provides functional container processing support like in Underscore.js, Ruby or LINQ.

## Example

```cpp
template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

ExVector<std::pair<std::string, int>> prices = {
  {"orange", 50},
  {"apple", 100},
  {"banana", 150},
  {"carrot",90}};

// method chaining
auto fruitsPrices2 = prices.filter([](const std::pair<std::string, int> &pair){
  return pair.first != "carrot";
}).values();

// comprehension like LINQ query expressions
auto fruitPrices1 = _do(
  _from(pair, prices),
  _where(pair.first != "carrot"),
  _select(pair.second)
);
```


