#pragma once

#include <boost/iterator/iterator_adaptor.hpp>
#include <type_traits>

namespace Amulet {
  
  template <typename TBaseIterator>
  class UniqueIterator;

  namespace detail {
    template <typename TBaseIterator>
    struct UniqueIteratorBase
    {
      using type = boost::iterator_adaptor<
        UniqueIterator<TBaseIterator>,
        TBaseIterator,
        boost::use_default,
        typename std::conditional<
          std::is_convertible<
            typename boost::iterator_traversal<TBaseIterator>::type,
            boost::bidirectional_traversal_tag
          >::value,
          boost::bidirectional_traversal_tag,
          boost::use_default
        >::type
      >;
    };
  }

  template <typename TBaseIterator>
  class UniqueIterator :
    public detail::UniqueIteratorBase<TBaseIterator>::type
  {
    using base_type = typename detail::UniqueIteratorBase<TBaseIterator>::type;

  public:

    UniqueIterator()
    {}

    UniqueIterator(TBaseIterator begin, TBaseIterator end, TBaseIterator i) :
      base_type(i),
      mBegin(begin),
      mEnd(end)
    {}

  private:

    friend class boost::iterator_core_access;

    void increment()
    {
      do {
        ++this->base_reference();
      } while (this->base() != mEnd && duplicated());
    }

    void decrement()
    {
      do {
        --this->base_reference();
      } while (duplicated());
    }

    bool duplicated() const
    {
      auto current = this->base();
      for (auto i = mBegin; i != current; ++i)
      {
        if (*i == *current)
          return true;
      }
      return false;
    }

    TBaseIterator mBegin, mEnd;
  };

  template <typename TBaseIterator>
  inline UniqueIterator<TBaseIterator> makeUniqueIterator(TBaseIterator begin, TBaseIterator end, TBaseIterator i)
  {
    return UniqueIterator<TBaseIterator>(begin, end, i);
  }
}