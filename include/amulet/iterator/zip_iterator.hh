#pragma once

#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>

namespace Amulet {

  template <typename TIterator1, typename TIterator2>
  class ZipIterator;

  namespace detail {

    template <typename T, typename U>
    struct CommonBase
    {
      using type = typename std::conditional<
        std::is_base_of<T, U>::value,
        T,
        U
      >::type;
    };

    template <typename TIterator1, typename TIterator2>
    struct ZipIteratorBase
    {
      using traits1 = std::iterator_traits<TIterator1>;
      using traits2 = std::iterator_traits<TIterator2>;
      using value_type = std::pair<typename traits1::value_type, typename traits2::value_type>;
      using type = boost::iterator_facade<
        ZipIterator<TIterator1, TIterator2>,
        value_type,
        typename CommonBase<
          typename boost::iterator_category<TIterator1>::type,
          typename boost::iterator_category<TIterator2>::type
        >::type,
        value_type
      >;
    };
  }

  template <typename TIterator1, typename TIterator2>
  class ZipIterator :
    public detail::ZipIteratorBase<TIterator1, TIterator2>::type
  {
    using base_type = typename detail::ZipIteratorBase<TIterator1, TIterator2>::type;

  public:

    using value_type = typename base_type::value_type;
    using difference_type = typename base_type::difference_type;

    ZipIterator() = default;
    ZipIterator(TIterator1 i1, TIterator2 i2) :
      mIterator1(i1),
      mIterator2(i2)
    {}

  private:

    friend class boost::iterator_core_access;

    value_type dereference() const
    {
      return std::make_pair(*mIterator1, *mIterator2);
    }

    bool equal(const ZipIterator &other) const
    {
      return mIterator1 == other.mIterator1 && mIterator2 == other.mIterator2;
    }

    void increment()
    {
      ++mIterator1;
      ++mIterator2;
    }

    void decrement()
    {
      --mIterator1;
      --mIterator2;
    }

    void advance(difference_type d)
    {
      mIterator1 += d;
      mIterator2 += d;
    }

    difference_type distance_to(const ZipIterator &other) const
    {
      return other.mIterator1 - mIterator1;
    }
    
    TIterator1 mIterator1;
    TIterator2 mIterator2;
  };

}
