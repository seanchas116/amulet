#pragma once

namespace Amulet {

  namespace detail {

    template <typename T>
    struct RangeComparison
    {
      template <typename U> friend bool operator==(const T &x, const U &y) { return boost::equal(x, y); }
      template <typename U> friend bool operator==(const U &x, const T &y) { return y == x; }
      template <typename U> friend bool operator!=(const T &x, const U &y) { return !(x == y); }
      template <typename U> friend bool operator!=(const U &x, const T &y) { return !(x == y); }

      template <typename U> friend bool operator<(const T &x, const U &y) { return boost::lexicographical_compare(x, y); }
      template <typename U> friend bool operator>(const T &x, const U &y) { return y < x; }
      template <typename U> friend bool operator<=(const T &x, const U &y) { return !(x > y); }
      template <typename U> friend bool operator>=(const T &x, const U &y) { return !(x < y); }

      template <typename U> friend bool operator<(const U &x, const T &y) { return boost::lexicographical_compare(x, y); }
      template <typename U> friend bool operator>(const U &x, const T &y) { return y < x; }
      template <typename U> friend bool operator<=(const U &x, const T &y) { return !(x > y); }
      template <typename U> friend bool operator>=(const U &x, const T &y) { return !(x < y); }
    };

  }

  template <typename TDerived, typename TIterator>
  class RangeFacade :
    private detail::RangeComparison<RangeFacade<TDerived, TIterator>>
  {
  public:

    using iterator = TIterator;
    using const_iterator = iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using reference = typename std::iterator_traits<iterator>::reference;
    using const_reference = reference;
    using difference_type = typename std::iterator_traits<iterator>::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;

    const_iterator begin() const
    {
      return base()->begin();
    }

    const_iterator end() const
    {
      return base()->end();
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
        throw std::out_of_range("Amulet::RangeAdaptor");
      auto i = begin();
      std::advance(i, pos);
      return i;
    }

    const_reference operator[](size_type pos) const
    {
      BOOST_ASSERT(pos < size());
      auto i = begin();
      std::advance(i, pos);
      return i;
    }

    size_type size() const
    {
      return std::distance(begin(), end());
    }

    bool empty() const
    {
      return size() == 0;
    }

  private:

    const TDerived *base() const { return static_cast<const TDerived *>(this); }
  };
}
