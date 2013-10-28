Amulet
======

Amulet is a header-only C++ library that provides some functional programming support (mainly for containers) like in Underscore.js, Ruby or LINQ.

- Add convenient functional methods to containers (`Amulet::RangeExtension`)
- Comprehension syntax (query macro)
- Optional values (`Amulet::Option`)

## RangeExtension

`Amulet::RangeExtension` is a template class that inherits the given container class (which must have `begin()` and `end()` methods)
and adds a set of convenient methods for container processing to the original class.

All methods provided by RangeExtension is immutable (const).

### Some Examples

```
RangeExtension<std::vector<int>> xs = {1,2,3};

// produce a new container by transforming each value by a function
xs.map([](int x){
  return x * 2;
}); // => {2,4,6}

// collect values that fulfill a condition
xs.filter([](int x){
  return x % 2 == 0;
}); // => {2}

// make a reverse container
xs.reverse(); // => {3,2,1}

// sum up the values
xs.foldLeft(0, [](int sum, int x){
  return sum + x;
}); // => 6

// map and flatten
xs.flatMap([](int x){
  return RangeExtension<std::vector<int>>{x, x};
}); // => {1,1,2,2,3,3}

// append index to each value
xs.withIndex(); // => {{0,1},{1,2},{2,3}}
```

For detail, see `include/amulet/range_extension.hh`.

### Extend existing containers

`Amulet::extend` wraps a normal container and make RangeExtension methods available.

```cpp
std::vector<int> vec = {1,2,3};
auto twices = Amulet.extend(vec).map([](int x){
  return x * 2;
});
```

### Int Range

`Amulet::intRange` returns an integer range that has RangeExtension methods.

```cpp
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
```

### Weaknesses

For safety and convenience, RangeExtension methods that returns a transformed container (e.g., `map`, `filter`) internally copy the original container.


## Query Macro (Comprehension)

Amulet provides a comprehension syntax similar to LINQ qury expressions, for-comrehension in Scala or do-notation in Haskell, using preprocessor macro.

```cpp
template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

ExVector<std::pair<std::string, int>> prices = {
  {"orange", 50},
  {"apple", 100},
  {"banana", 150},
  {"carrot",90}};

// comprehension version 
auto fruitPrices1 = _do(
  _from(pair, prices),
  _where(pair.first != "carrot"),
  _select(pair.second)
);

// method chaining version
auto fruitsPrices2 = prices.filter([](const std::pair<std::string, int> &pair){
  return pair.first != "carrot";
}).map([](const std::pair<std::string, int> &pair){
  return pair.second;
});

// simpler method chaining version, using values() to collect the second value from each pair
auto fruitsPrices3 = prices.filter([](const std::pair<std::string, int> &pair){
  return pair.first != "carrot";
}).values();
```

## Option

`Amulet::Option` is a container-like object that represents 'optional' values.
It internally uses `boost::optional`.

While `boost::optional` is analogous to pointers, `Amulet::Option` is analogous to containers (containers that can have 0 or 1 value).

Since Option is analogous to containers, you can use query macro with Option values.

```
auto divide = [](int x, int y) -> Amulet::Option<int>{
  if (y)
    return Amulet::some(x / y);  // succeeded
  else
    return Amulet::none;  // failed, returns invalid value
};
auto a = Amulet::some(0);
auto b = Amulet::some(1);

auto divided = _do(
  _from(x, b),
  _from(y, a),
  divide(x, y)
); // has no value because divide() will be failed

auto added1 = _do(
  _from(x, a),
  _from(y, b),
  _select(x + y)
); // Amulet::Optional<int>(1)

auto added2 = _do(
  _from(x, a),
  _from(y, divided),
  _select(x + y)
); // has no value because divided has no value
```

## Monad

Monad is a sort of design pattern heavily used in Haskell and Scala.

In Amulet, Monad concept is defined as followings:

- has `value_type` type.
- has `template <typename F> flatMap(F f)` method.
- has `template <typename T> fromValue(const T &vaue)` static method.

RangeExtension and Option fulfills this concept.

In fact, query macro can be used with any values which type fulfills Monad concept.
