#pragma once

#include "iterator/with_index_iterator.hh"
#include "iterator/flatten_iterator.hh"
#include "iterator_range.hh"
#include <boost/iterator/transform_iterator.hpp>
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
  }

  template <typename TBase>
  class RangeExtension;

  template <typename TIterator>
  using ExtendedIteratorRange = RangeExtension<IteratorRange<TIterator>>;
  
  template <typename TBase>
  class RangeExtension : public TBase
  {
  private:

    using base_type = TBase;
    using self_type = RangeExtension<TBase>;
    using size_type = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;
    
    using Value = typename base_type::value_type;
    using Iterator = typename base_type::const_iterator;
    using Reference = typename base_type::const_reference;
    using SubRange = ExtendedIteratorRange<typename base_type::const_iterator>;

    template <typename TIterator>
    static ExtendedIteratorRange<TIterator> makeIteratorRange(TIterator begin, TIterator end)
    {
      return ExtendedIteratorRange<TIterator>(begin, end);
    }

  public:

    using TBase::TBase;

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
    
    template <typename TFunc>
    ExtendedIteratorRange<
      boost::filter_iterator<
        std::function<bool(const Value &)>,
        Iterator>>
    filter(TFunc f) const
    {
      using fn = std::function<bool(const Value &)>;
      using it = boost::filter_iterator<fn, Iterator>;
      return makeIteratorRange(
        it(f, this->begin(), this->end()),
        it(f, this->end(), this->end())
      );
    }
    
    template <typename TFunc>
    ExtendedIteratorRange<
      boost::transform_iterator<
        std::function<typename std::result_of<TFunc(const Value &)>::type(const Value &)>,
        Iterator>>
    map(TFunc f) const
    {
      using fn = std::function<typename std::result_of<TFunc(const Value &)>::type(const Value &)>;
      using it = boost::transform_iterator<fn, Iterator>;
      return makeIteratorRange(
        it(this->begin(), f),
        it(this->end(), f)
      );
    }
    
    ExtendedIteratorRange<FlattenIterator<Iterator>>
    flatten() const
    {
      return makeIteratorRange(
        makeFlattenIterator(this->end(), this->begin()),
        makeFlattenIterator(this->end(), this->end())
      );
    }

    template <typename TFunc>
    auto flatMap(TFunc f) const -> decltype(static_cast<const RangeExtension *>(nullptr)->map(f).flatten())
    {
      return map(f).flatten();
    }

    ExtendedIteratorRange<std::reverse_iterator<Iterator>>
    reverse() const
    {
      return makeIteratorRange(
        std::reverse_iterator<Iterator>(this->end()),
        std::reverse_iterator<Iterator>(this->begin())
      );
    }

    ExtendedIteratorRange<WithIndexIterator<Iterator>>
    withIndex() const
    {
      return makeIteratorRange(
        makeWithIndexIterator(this->begin()),
        makeWithIndexIterator(this->end())
      );
    }

    Reference head() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *this->begin();
    }
    SubRange tail() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return makeIteratorRange(++this->begin(), this->end());
    }
    SubRange init() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return makeIteratorRange(this->begin(), --this->end());
    }
    Reference last() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *(--this->end());
    }
    SubRange slice(size_type first_index, size_type last_index) const
    {
      return mid(first_index, last_index - first_index + 1);
    }
    SubRange mid(size_type first_index, size_type size) const
    {
      BOOST_ASSERT(this->end() - this->begin() >= first_index + size);
      auto begin_it = this->begin() + first_index;
      auto end_it = begin_it + size;
      return makeIteratorRange(begin_it, end_it);
    }

    template <template <typename> class TContainer>
    TContainer<Value> to() const
    {
      TContainer<Value> container;
      //container.reserve(this->size());
      each([&](const Value &value){
        container.push_back(value);
      });
      return container;
    }

  private:

  };

  template <typename TIterator>
  ExtendedIteratorRange<TIterator> extend(TIterator begin, TIterator end)
  {
    return RangeExtension<IteratorRange<TIterator>>(begin, end);
  }

  template <typename TRange>
  ExtendedIteratorRange<typename TRange::const_iterator> extend(const TRange &range)
  {
    return extend(std::begin(range), std::end(range));
  }
}
