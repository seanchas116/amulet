#pragma once

#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/lexicographical_compare.hpp>
#include <boost/operators.hpp>

namespace Amulet {

  template <typename TIterator>
  class IteratorRange :
    private boost::less_than_comparable<IteratorRange<TIterator>>,
    private boost::equality_comparable<IteratorRange<TIterator>>
  {
  public:

    using iterator = TIterator;
    using const_iterator = iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using reference = typename std::iterator_traits<iterator>::reference;
    using const_reference = reference;
    using difference_type = typename std::iterator_traits<iterator>::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;

    IteratorRange() = default;

    IteratorRange(iterator begin, iterator end) :
      mBegin(begin), mEnd(end)
    {
    }

    const_iterator begin() const
    {
      return mBegin;
    }
    const_iterator end() const
    {
      return mEnd;
    }
    const_iterator cbegin() const
    {
      return begin();
    }
    const_iterator cend() const
    {
      return end();
    }

    const_reference at(size_type pos) const
    {
      if (pos >= size())
        throw std::out_of_range("Amulet::IteratorRange");
      auto i = mBegin;
      std::advance(i, pos);
      return i;
    }

    const_reference operator[](size_type pos) const
    {
      BOOST_ASSERT(pos < size());
      auto i = mBegin;
      std::advance(i, pos);
      return i;
    }

    size_type size() const
    {
      return std::distance(mBegin, mEnd);
    }

    bool empty() const
    {
      return size() == 0;
    }

    template <typename TRange>
    bool operator<(const TRange &other) const
    {
      return boost::lexicographical_compare(*this, other);
    }

    template <typename TRange>
    bool operator==(const TRange &other) const
    {
      return boost::equal(*this, other);
    }

  private:

    iterator mBegin, mEnd;
  };

}
