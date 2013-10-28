#include <amulet/range_extension.hh>
#include <gtest/gtest.h>
#include <deque>
#include <list>

template <typename T>
using ExVector = Amulet::RangeExtension<std::vector<T>>;

TEST(RangeExtension, each)
{
  ExVector<int> xs = {1, 3, 7, 10};
  std::vector<int> result;
  xs.each([&](int x){
    result.push_back(x);
  });
  EXPECT_EQ(result, xs);
}

TEST(RangeExtension, eachPair)
{
  ExVector<std::pair<int, double>> xs = {{1, 0.5}, {2, 1.0}, {3, 1.5}};
  std::vector<std::pair<int, double>> result;
  xs.eachPair([&](int i, double d){
    result.push_back(std::make_pair(i, d));
  });
  EXPECT_EQ(result, xs);
}

TEST(RangeExtension, eachWithIndex)
{
  ExVector<int> xs = {1,4,6,3};
  std::vector<std::pair<std::size_t, int>> expected = {{0,1},{1,4},{2,6},{3,3}};
  std::vector<std::pair<std::size_t, int>> result;
  
  xs.eachWithIndex([&](size_t i, int v){
    result.push_back(std::make_pair(i, v));
  });

  EXPECT_EQ(result, expected);
}

TEST(RangeExtension, min)
{
  ExVector<int> xs = {2,4,1,2};
  EXPECT_EQ(1, xs.min());
}

TEST(RangeExtension, max)
{
  ExVector<int> xs = {2,4,1,2};
  EXPECT_EQ(4, xs.max());
}

TEST(RangeExtension, find)
{
  ExVector<int> xs = {1,4,5};
  auto a = xs.find([](int x){
    return x == 1;
  });
  auto b = xs.find([](int x){
    return x == 0;
  });
  EXPECT_EQ(1, a.get());
  EXPECT_EQ(false, b.hasValue());
}

TEST(RangeExtension, contains)
{
  ExVector<int> xs = {1,2,3};
  auto a = xs.contains(1);
  auto b = xs.contains(0);
  EXPECT_EQ(true, a);
  EXPECT_EQ(false, b);
}

TEST(RangeExtension, filter)
{
  ExVector<int> xs = {1,4,6,3};
  auto filtered = xs.filter([](int x){
    return x % 2 == 0;
  });
  ExVector<int> expected = {4, 6};
  EXPECT_EQ(expected, filtered);
}

TEST(RangeExtension, map)
{
  ExVector<int> xs = {1, 2, 3};
  auto mapped = xs.map([](int x){
    return x * 0.5;
  });
  auto expected = {0.5, 1.0, 1.5};
  EXPECT_EQ(mapped, expected);
}

TEST(RangeExtension, flatten)
{
  ExVector<ExVector<int>> xs = {{1,2,3},{4,5,6},{7,8,9}};
  auto flattened = xs.flatten();
  auto expected = {1,2,3,4,5,6,7,8,9};
  EXPECT_EQ(expected, flattened);
}

TEST(RangeExtension, flatten_reverse)
{
  ExVector<ExVector<int>> xs = {{1,2,3},{4,5,6},{7,8,9}};
  auto flattened = xs.flatten().reverse();
  auto expected = {9,8,7,6,5,4,3,2,1};
  EXPECT_EQ(flattened, expected);
}

TEST(RangeExtension, flatMap)
{
  ExVector<int> xs = {1,2,3};
  auto flatMapped = xs.flatMap([](double x){
    return ExVector<double>{x, 2*x, 3*x};
  });
  ExVector<double> expected = {1,2,3,2,4,6,3,6,9};
  EXPECT_EQ(flatMapped, expected);
}

TEST(RangeExtension, flatMap_reverse)
{
  ExVector<int> xs = {1,2,3};
  auto flatMapped = xs.flatMap([](int x){
    return ExVector<int>{x, 2*x, 3*x};
  }).reverse();

  auto expected = {9,6,3,6,4,2,3,2,1};
  EXPECT_EQ(flatMapped, expected);
}

TEST(RangeExtension, withIndex)
{
  ExVector<int> xs = {1,4,6,3};
  auto withIndexed = xs.withIndex();
  std::vector<std::pair<std::size_t, int>> expected = {{0,1},{1,4},{2,6},{3,3}};
  EXPECT_EQ(withIndexed, expected);
}

TEST(RangeExtension, unique)
{
  ExVector<int> xs = {1,3,2,3,1};
  auto uniqued = xs.unique();
  auto expected = {1,3,2};
  EXPECT_EQ(uniqued, expected);
}

TEST(RangeExtension, unique_reverse)
{
  ExVector<int> xs = {1,3,2,3,1};
  auto uniqued = xs.unique().reverse();
  auto expected = {2,3,1};
  EXPECT_EQ(uniqued, expected);
}

TEST(RangeExtension, sort)
{
  ExVector<int> xs = {1,3,2,3,1};
  auto sorted = xs.sort();
  auto expected = ExVector<int>{1,1,2,3,3};
  EXPECT_EQ(expected, sorted);
}

TEST(RangeExtension, sortBy)
{
  ExVector<int> xs = {1,3,2,3,1};
  auto sorted = xs.sortBy([](int x, int y){
    return x > y;
  });
  auto expected = ExVector<int>{3,3,2,1,1};
  EXPECT_EQ(expected, sorted);
}

TEST(RangeExtension, zip)
{
  ExVector<int> xs = {1,1,2};
  ExVector<int> ys = {2,3,5};
  ExVector<std::pair<int, int>> expected = {{1,2},{1,3},{2,5}};
  auto zipped = xs.zip(ys);
  EXPECT_EQ(zipped, expected);
}

TEST(RangeExtension, to)
{
  ExVector<int> xs = {1,3,2};
  auto list = xs.to<std::list<int>>();
  auto deque = xs.to<std::deque<int>>();
  auto expected_list = std::list<int>{1,3,2};
  auto expected_deque = std::deque<int>{1,3,2};
  EXPECT_EQ(expected_list, list);
  EXPECT_EQ(expected_deque, deque);
}

TEST(RangeExtension, head_tail_init_last)
{
  ExVector<int> xs = {1, 4, 6, 3};
  auto h = xs.head();
  auto t = xs.tail();
  auto i = xs.init();
  auto l = xs.last();

  auto expected_t = {4, 6, 3};
  auto expected_i = {1, 4, 6};

  EXPECT_EQ(h, 1);
  EXPECT_EQ(t, expected_t);
  EXPECT_EQ(i, expected_i);
  EXPECT_EQ(l, 3);
}

TEST(RangeExtension, slice_mid)
{
  ExVector<int> xs = {1, 4, 6, 3};
  auto s = xs.slice(1, 3);
  auto expected_s = {4, 6, 3};
  auto m = xs.mid(2, 2);
  auto expected_m = {6, 3};
  EXPECT_EQ(s, expected_s);
  EXPECT_EQ(m, expected_m);
}
