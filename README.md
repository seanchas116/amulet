Amulet
======

Amulet is a header-only C++ library that provides some functional programming support (mainly for containers and monads) like in Underscore.js, Ruby, Scala or LINQ.

- Add convenient functional methods to containers (`Amulet::RangeExtension`)
- Comprehension syntax (query macro)
- Optional values (`Amulet::Option`)

## RangeExtension

`Amulet::RangeExtension` is a template class that inherits the given container class (which must have `begin()` and `end()` methods)
and adds a set of convenient methods for container manipulation to the original class.

All methods provided by RangeExtension is immutable (const).

### Some Examples

```cpp
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

For detail, see the reference (at the bottom of this page).

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

Amulet provides a comprehension syntax similar to LINQ query expressions, for-comrehension in Scala or do-notation in Haskell, using preprocessor macro.

```cpp
template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

auto xs = ExVector<int>{1,2};
auto ys = ExVector<int>{3,4};

auto product = _do(
  _from(x, xs),
  _from(y, ys),
  _select(std::make_pair(x, y))
); // => {{1,3},{1,4},{2,3},{2,4}}

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

```cpp
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

## Reference

### RangeExtension

- `each(function)`

Passes every value in the container to a function.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.each([](int x){
  std::cout << x << std::endl;
});
```

- `eachPair(function)`

Passes every pair in the container to a function.
Available only if the value type of the container is std::pair or a equivalent one.

```cpp
RangeExtension<std::map<int, std::string>> xs = {{1, "one"},{2, "two"},{3, "three"}};
xs.eachPair([](int x, const std::string &str){
  std::cout << x << ": " << str << std::endl;
});
```

- `eachWithIndex(function)`

Passes every value and its index to a function.
Equivalent to `withIndex().eachPair`

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.eachWithIndex([](size_t index, int x){
  std::cout << index << ": " << x << std::endl;
});
```

- `foldLeft(initialState, function)`

Passes each value and the current state to a function in left-to-right order and sets the current state to the return value.

```
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.foldLeft(0, [](int sum, int x){
  return sum + x;
}); // => 6
```

- `foldRight(initialState, function)`

Passes each value and the current state to a function in right-to-left order and sets the current state to the return value.
Equivalent to `reverse().foldLeft`

- `min()`

Returns the minimum value.

- `max()`

Returns the maximum value.

- `find(predicate)`

Finds the first value that fulfill the condition.
Return type is `Option<value_type>`.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.find([](int x){
  return x % 2 == 0;
}); // Option<int>(2)
xs.find([](int x){
  return x < 0;
}); // Option<int>() (has no value)
```

- `contains(value)`

Returns whether the container contains the value.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.contains(1); // => true
xs.contains(0); // => false
```

- `zip(otherContainer)`

Merges together each value of the container and another container into a std::pair.
The size of merged container will be the minimum of the sizes of original ones.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
RangeExtension<std::vector<int>> ys = {4,5,6,7};
xs.zip(ys); // => {{1,4},{2,5},{3,6}}
```

- `filter(predicate)`

Collects all values that fulfill a condition.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.filter([](int x){
  return x % 2 != 0;
}); // => {1,3}
```

- `map(function)`

Produces a new container by transforming each value by a function.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.map([](int x){
  return x * 2;
}); // => {2,4,6}
```

- `flatten()`

Flattens a container of containers.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<
  RangeExtension<std::vector<int>>
>> xs = {{1,2,3},{4,5,6},{7,8,9}};
xs.flatten(); // => {1,2,3,4,5,6,7,8,9}
```

- `flatMap(function)`

Equivalent to `map(function).flatten()`.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.flatMap([](int x){
  return RangeExtension<std::vector<int>>{x,x};
}); // => {1,1,2,2,3,3}
```

- `reverse()`

Makes a reverse container.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.reverse(); // => {3,2,1}
```

- `withIndex()`

Merges together each index and each value int a std::pair.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3};
xs.withIndex(); // => {{0,1},{1,2},{2,3}}
```

- `unique()`

Removes any duplicates.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,2,3,1};
xs.unique(); // => {1,2,3}
```

- `firsts(), keys()`

Collects the first value of each pair.
Available only if the value type of the container is std::pair or a equivalent one.
The result is lazily-evaluated.

```cpp
RangeExtension<std::map<int, std::string>> xs = {{1, "one"},{2, "two"},{3, "three"}};
xs.firsts(); // => {1,2,3}
```

- `seconds(), values()`

Collects the second value of each pair.
Available only if the value type of the container is std::pair or a equivalent one.
The result is lazily-evaluated.

```cpp
RangeExtension<std::map<int, std::string>> xs = {{1, "one"},{2, "two"},{3, "three"}};
xs.seconds(); // => {"one","two","three"}
```

- `sort(), stableSort()`

Sorts the values.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.sort(); // => {0,1,2,3}
```

- `sortBy(compare), stableSortBy(compare)`

Sorts the values by a compare function.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.sortBy([](int x, int y){
  return x > y;
}); // => {3,2,1,0}
```

- `partial(beginIndex, endIndex)`

Returns a sliced container from `beginIndex` until `endIndex`.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.partial(1,3) // => {3,2}
```

- `slice(firstIndex, lastIndex)`

Returns a sliced container from `firstIndex` to `lastIndex`.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.slice(1,3) // => {3,2,0}
```

- `mid(firstIndex, size)`

Returns a sliced container from `firstIndex` until `firstIndex + size`.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.mid(1,2) // => {3,2}
```

- `narrow(frontOffset, backOffset)`

Drops the first `frontOffset` values and the last `backOffset` values.
The result is lazily-evaluated.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
xs.narrow(1,1) // => {3,2}
```

- `head()`

Returns the first value.

- `tail()`

Drops the first value.
Equivalent to `narrow(1,0)`.

- `init()`

Drops the last value.
Equivalent to `narrow(0,1)`.

- `tail()`

Returns the last value.

- `to<NewContainer>()`

Creates a new container and puts all the values into it.

```cpp
RangeExtension<std::vector<int>> xs = {1,3,2,0};
auto newVector = xs.map([](int x){
  return x * 2;
}).to<std::vector<int>>(); // => std::vector<int>{2,4,6}
```

### Option

