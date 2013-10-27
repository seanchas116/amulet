#pragma once

#include "iterator/with_index_iterator.hh"
#include "iterator/flatten_iterator.hh"
#include "iterator/unique_iterator.hh"
#include "iterator/zip_iterator.hh"
#include "iterator_range.hh"
#include "range_adaptor.hh"
#include "option.hh"
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace Amulet {

  namespace detail {

    template <typename TBinaryProc>
    struct PairForwarder{
      TBinaryProc proc;
      template <typename TPair>
      void operator()(const TPair &p)
      {
        proc(p.first, p.second);
      }
    };

    template <typename T>
    struct ReturnPairFirst
    {
      typename T::first_type operator()(const T &p)
      {
        return p.first;
      }
    };

    template <typename T>
    struct ReturnPairSecond
    {
      typename T::second_type operator()(const T &p)
      {
        return p.second;
      }
    };

    template <typename TRange, typename TUnaryFunc>
    struct MapRangePolicy
    {
      using base_range = TRange;
      using value_type = typename base_range::value_type;
      using function_result = typename std::result_of<TUnaryFunc(value_type)>::type;
      using function_type = std::function<function_result(const value_type &)>;
      using iterator = boost::transform_iterator<function_type, typename base_range::const_iterator>;

      MapRangePolicy() = default;
      MapRangePolicy(TUnaryFunc f) :
        mFunc(f)
      {}

      iterator begin(const base_range &range) const
      {
        return iterator(std::begin(range), mFunc);
      }

      iterator end(const base_range &range) const
      {
        return iterator(std::end(range), mFunc);
      }

    private:
      function_type mFunc;
    };

    template <typename TRange, typename TUnaryFunc>
    struct FilterRangePolicy
    {
      using base_range = TRange;
      using value_type = typename base_range::value_type;
      using function_type = std::function<bool(const value_type &)>;
      using iterator = boost::filter_iterator<function_type, typename base_range::const_iterator>;

      FilterRangePolicy() = default;
      FilterRangePolicy(TUnaryFunc f) :
        mFunc(f)
      {}

      iterator begin(const base_range &range) const
      {
        return iterator(mFunc, std::begin(range), std::end(range));
      }

      iterator end(const base_range &range) const
      {
        return iterator(mFunc, std::end(range), std::end(range));
      }

    private:
      function_type mFunc;
    };
    
    template <typename TRange>
    struct FlattenRangePolicy
    {
      using base_range = TRange;
      using iterator = FlattenIterator<typename base_range::const_iterator>;
      
      iterator begin(const base_range &range) const
      {
        return iterator(std::end(range), std::begin(range));
      }

      iterator end(const base_range &range) const
      {
        return iterator(std::end(range), std::end(range));
      }
    };

    template <typename TRange>
    struct ReverseRangePolicy
    {
      using base_range = TRange;
      using iterator = std::reverse_iterator<typename base_range::const_iterator>;

      iterator begin(const base_range &range) const
      {
        return iterator(std::end(range));
      }

      iterator end(const base_range &range) const
      {
        return iterator(std::begin(range));
      }
    };

    template <typename TRange>
    struct WithIndexRangePolicy
    {
      using base_range = TRange;
      using iterator = WithIndexIterator<typename base_range::const_iterator>;

      iterator begin(const base_range &range) const
      {
        return iterator(std::begin(range));
      }

      iterator end(const base_range &range) const
      {
        return iterator(std::end(range));
      }
    };

    template <typename TRange>
    struct UniqueRangePolicy
    {
      using base_range = TRange;
      using iterator = UniqueIterator<typename base_range::const_iterator>;

      iterator begin(const base_range &range) const
      {
        return iterator(std::begin(range), std::end(range), std::begin(range));
      }

      iterator end(const base_range &range) const
      {
        return iterator(std::begin(range), std::end(range), std::end(range));
      }
    };

    template <typename TRange>
    struct PartialRangePolicy
    {
      using base_range = TRange;
      using iterator = typename base_range::const_iterator;

      PartialRangePolicy() = default;
      PartialRangePolicy(size_t firstIndex, size_t lastIndex) :
        mFirstIndex(firstIndex), mLastIndex(lastIndex)
      {}

      iterator begin(const base_range &range) const
      {
        auto i = std::begin(range);
        std::advance(i, mFirstIndex);
        return i;
      }

      iterator end(const base_range &range) const
      {
        auto i = std::begin(range);
        std::advance(i, mLastIndex);
        return i;
      }

    private:
      size_t mFirstIndex = 0, mLastIndex = 0;
    };

    template <typename TRange>
    struct NarrowedRangePolicy
    {
      using base_range = TRange;
      using iterator = typename base_range::const_iterator;

      NarrowedRangePolicy() = default;
      NarrowedRangePolicy(size_t frontOffset, size_t backOffset) :
        mFrontOffset(frontOffset), mBackOffset(backOffset)
      {}

      iterator begin(const base_range &range) const
      {
        auto i = std::begin(range);
        std::advance(i, mFrontOffset);
        return i;
      }

      iterator end(const base_range &range) const
      {
        auto i = std::end(range);
        std::advance(i, -mBackOffset);
        return i;
      }

    private:
      size_t mFrontOffset = 0, mBackOffset = 0;
    };

    template <typename TRange1, typename TRange2>
    class ZipRange :
      public RangeFacade<
        ZipRange<TRange1, TRange2>,
        ZipIterator<typename TRange1::const_iterator, typename TRange2::const_iterator>
      >
    {
    public:

      using const_iterator = ZipIterator<typename TRange1::const_iterator, typename TRange2::const_iterator>;

      ZipRange() = default;

      ZipRange(const TRange1 &r1, const TRange2 r2) :
        mRange1(r1), mRange2(r2)
      {}

      const_iterator begin() const
      {
        return const_iterator(std::begin(mRange1), std::begin(mRange2));
      }

      const_iterator end() const
      {
        ptrdiff_t d1 = std::distance(std::begin(mRange1), std::end(mRange1));
        ptrdiff_t d2 = std::distance(std::begin(mRange2), std::end(mRange2));
        auto d = std::min(d1, d2);

        auto i1 = std::begin(mRange1);
        auto i2 = std::begin(mRange2);
        std::advance(i1, d);
        std::advance(i2, d);
        return const_iterator(i1, i2);
      }

    private:

      TRange1 mRange1;
      TRange2 mRange2;
    };

  }

  template <typename TBase>
  class RangeExtension;

  template <typename TIterator>
  using ExtendedIteratorRange = RangeExtension<IteratorRange<TIterator>>;

  template <typename TPolicy>
  using ExtendedRangeAdaptor = RangeExtension<RangeAdaptor<TPolicy>>;

  template <typename TValue>
  using UnitRange = RangeExtension<Option<TValue>>;
  
  template <typename TBase>
  class RangeExtension : public TBase
  {
  private:

    using base_type = TBase;
    using self_type = RangeExtension<TBase>;
    using Size = typename base_type::size_type;
    using Difference = typename base_type::difference_type;
    
    using Value = typename base_type::value_type;
    using Iterator = typename base_type::const_iterator;
    using Reference = typename base_type::const_reference;

  public:

    using TBase::TBase;

    template <typename T>
    static UnitRange<T> fromValue(const T &value)
    {
      return UnitRange<T>(value);
    }

    template <typename TUnaryProc>
    void each(TUnaryProc proc) const
    {
      std::for_each(this->begin(), this->end(), proc);
    }

    template <typename TBinaryProc>
    void eachPair(TBinaryProc proc) const
    {
      detail::PairForwarder<TBinaryProc> f = {std::move(proc)};
      each(f);
    }
    
    template <typename TBinaryProc>
    void eachWithIndex(TBinaryProc proc) const
    {
      withIndex().eachPair(proc);
    }

    template <typename TResult, typename TBinaryFunc>
    TResult foldLeft(const TResult &init, TBinaryFunc f) const
    {
      auto memo = init;
      each([&](const Value &v){
        memo = f(memo, v);
      });
      return memo;
    }

    template <typename TResult, typename TBinaryFunc>
    TResult foldRight(const TResult &init, TBinaryFunc f) const
    {
      return reverse().folrLeft(init, f);
    }

    Value min() const
    {
      return tail().foldLeft(head(), [](const Value &m, const Value &v){
        return std::min(m, v);
      });
    }

    Value max() const
    {
      return tail().foldLeft(head(), [](const Value &m, const Value &v){
        return std::max(m, v);
      });
    }
    
    template <typename TPredicate>
    Option<Value> find(TPredicate predicate) const
    {
      Option<Value> result;
      for (const Value &x : *this) {
        if (predicate(x))
          result = some(x);
      }
      return result;
    }

    bool contains(const Value &value) const
    {
      for (const Value &x : *this) {
        if (x == value)
          return true;
      }
      return false;
    }

    template <typename TRange>
    detail::ZipRange<RangeExtension<TBase>, TRange>
    zip(const TRange &other) const
    {
      return detail::ZipRange<RangeExtension<TBase>, TRange>(*this, other);
    }

    template <typename TFunc>
    ExtendedRangeAdaptor<detail::FilterRangePolicy<self_type, TFunc>>
    filter(TFunc f) const
    {
      return ExtendedRangeAdaptor<detail::FilterRangePolicy<self_type, TFunc>>(*this, f);
    }
    
    template <typename TFunc>
    ExtendedRangeAdaptor<detail::MapRangePolicy<self_type, TFunc>>
    map(TFunc f) const
    {
      return ExtendedRangeAdaptor<detail::MapRangePolicy<self_type, TFunc>>(*this, f);
    }

    ExtendedRangeAdaptor<detail::FlattenRangePolicy<self_type>>
    flatten() const
    {
      return ExtendedRangeAdaptor<detail::FlattenRangePolicy<self_type>>(*this);
    }

    template <typename TFunc>
    auto flatMap(TFunc f) const -> decltype(static_cast<const RangeExtension *>(nullptr)->map(f).flatten())
    {
      return map(f).flatten();
    }

    ExtendedRangeAdaptor<detail::ReverseRangePolicy<self_type>>
    reverse() const
    {
      return ExtendedRangeAdaptor<detail::ReverseRangePolicy<self_type>>(*this);
    }

    ExtendedRangeAdaptor<detail::WithIndexRangePolicy<self_type>>
    withIndex() const
    {
      return ExtendedRangeAdaptor<detail::WithIndexRangePolicy<self_type>>(*this);
    }

    ExtendedRangeAdaptor<detail::UniqueRangePolicy<self_type>>
    unique() const
    {
      return ExtendedRangeAdaptor<detail::UniqueRangePolicy<self_type>>(*this);
    }

    ExtendedRangeAdaptor<detail::MapRangePolicy<self_type, detail::ReturnPairFirst<Value>>>
    firsts() const
    {
      return map(detail::ReturnPairFirst<Value>());
    }

    auto keys() const -> decltype(firsts())
    {
      return firsts();
    }

    ExtendedRangeAdaptor<detail::MapRangePolicy<self_type, detail::ReturnPairSecond<Value>>>
    seconds() const
    {
      return map(detail::ReturnPairSecond<Value>());
    }

    auto values() const -> decltype(seconds())
    {
      return seconds();
    }

    RangeExtension<std::vector<Value>>
    sort() const
    {
      auto vector = to<RangeExtension<std::vector<Value>>>();
      std::sort(vector.begin(), vector.end());
      return vector;
    }

    RangeExtension<std::vector<Value>>
    stableSort() const
    {
      auto vector = to<RangeExtension<std::vector<Value>>>();
      std::stable_sort(vector.begin(), vector.end());
      return vector;
    }

    ExtendedRangeAdaptor<detail::NarrowedRangePolicy<self_type>>
    narrow(size_t frontOffset, size_t backOffset) const
    {
      BOOST_ASSERT(this->end() - this->begin() - backOffset >= frontOffset);
      return ExtendedRangeAdaptor<detail::NarrowedRangePolicy<self_type>>(*this, frontOffset, backOffset);
    }

    Reference head() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *this->begin();
    }

    auto tail() const -> decltype(narrow(0,0))
    {
      return narrow(1, 0);
    }

    auto init() const -> decltype(narrow(0,0))
    {
      return narrow(0, 1);
    }

    Reference last() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *(--this->end());
    }

    ExtendedRangeAdaptor<detail::PartialRangePolicy<self_type>>
    partial(size_t beginIndex, size_t endIndex) const
    {
      BOOST_ASSERT(beginIndex <= endIndex);
      BOOST_ASSERT(this->end() - this->begin() >= endIndex);
      return ExtendedRangeAdaptor<detail::PartialRangePolicy<self_type>>(*this, beginIndex, endIndex);
    }

    auto slice(size_t firstIndex, size_t lastIndex) const -> decltype(partial(0,0))
    {
      return partial(firstIndex, lastIndex + 1);
    }

    auto mid(size_t firstIndex, size_t size) const -> decltype(partial(0,0))
    {
      return partial(firstIndex, firstIndex + size);
    }

    template <template <typename> class TContainer>
    TContainer<Value> to() const
    {
      return to<TContainer<Value>>();
    }

    template <typename TContainer>
    TContainer to() const
    {
      TContainer container;
      //container.reserve(this->size());
      each([&](const Value &value){
        container.push_back(value);
      });
      return container;
    }

  private:

  };

  template <typename TIterator>
  inline ExtendedIteratorRange<TIterator> extend(TIterator begin, TIterator end)
  {
    return ExtendedIteratorRange<TIterator>(begin, end);
  }

  template <typename TRange>
  inline ExtendedIteratorRange<typename TRange::const_iterator> extend(const TRange &range)
  {
    return extend(std::begin(range), std::end(range));
  }
}
