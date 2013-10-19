#pragma once

#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include "iterator/with_index_iterator.hh"
#include "iterator_range.hh"

namespace Amulet {

  namespace detail {
    template <typename TPair, typename TBinaryProc>
    inline void forwardPair(const TPair &pair, TBinaryProc proc)
    {
      proc(pair.first, pair.second);
    }
  }
  
  template <typename TBase>
  class RangeExtension : public TBase
  {
  private:

    using base_type = TBase;
    using self_type = RangeExtension<TBase>;
    using size_type = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;

    template <typename TIterator>
    static auto makeIteratorRange(TIterator begin, TIterator end)
    {
      return RangeExtension<IteratorRange<TIterator>>(begin, end);
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
      each(std::bind(&detail::forwardPair, std::placeholders::_1, proc));
    }
    
    template <typename TBinaryProc>
    void eachWithIndex(TBinaryProc proc) const
    {
      withIndex().eachPair(proc);
    }
    
    template <typename TPred>
    auto filter(TPred f) const
    {
      return makeIteratorRange(
        boost::make_filter_iterator(f, this->begin(), this->end()),
        boost::make_filter_iterator(f, this->end(), this->end())
      );
    }
    
    template <typename TUnaryFunc>
    auto map(TUnaryFunc f) const
    {
      return makeIteratorRange(
        boost::make_transform_iterator(this->begin(), f),
        boost::make_transform_iterator(this->end(), f)
      );
    }

    template <typename TUnaryFunc>
    auto flatMap(TUnaryFunc f) const;

    auto withIndex() const
    {
      return makeIteratorRange(
        WithIndexIterator<self_type>(this->begin()),
        WithIndexIterator<self_type>(this->end())
      );
    }

    auto head() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *this->begin();
    }
    auto tail() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return makeIteratorRange(this->begin() + 1, this->end());
    }
    auto init() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return makeIteratorRange(this->begin(), this->end() - 1);
    }
    auto last() const
    {
      BOOST_ASSERT(this->end() - this->begin() > 0);
      return *(this->end() - 1);
    }
    auto slice(size_type first_index, size_type last_index) const
    {
      return mid(first_index, last_index - first_index + 1);
    }
    auto mid(size_type first_index, size_type size) const
    {
      BOOST_ASSERT(this->end() - this->begin() >= first_index + size);
      auto begin_it = this->begin() + first_index;
      auto end_it = begin_it + size;
      return makeIteratorRange(begin_it, end_it);
    }

  private:

  };

  template <typename TIterator>
  auto extend(TIterator begin, TIterator end)
  {
    return RangeExtension<IteratorRange<TIterator>>(begin, end);
  }

  template <typename TRange>
  auto extend(const TRange &range)
  {
    return extend(std::begin(range), std::end(range));
  }
}