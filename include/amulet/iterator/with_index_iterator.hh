#pragma once

#include <boost/iterator/iterator_adaptor.hpp>
#include <type_traits>

namespace Amulet {

  template <class TRange>
  class WithIndexIterator;

  namespace detail {

    template <class TIterator>
    struct WithIndexIteratorBase
    {
    private:
      using traits = std::iterator_traits<TIterator>;
      using value_type = std::pair<std::size_t, typename traits::value_type>;
    public:
      using type = boost::iterator_adaptor<
        WithIndexIterator<TIterator>,
        TIterator,
        value_type,
        boost::use_default,
        value_type,
        boost::use_default
      >;
    };

  }

  template <class TIterator>
  class WithIndexIterator : 
    public detail::WithIndexIteratorBase<TIterator>::type
  {
  private:
    using base = typename detail::WithIndexIteratorBase<TIterator>::type;

  public:
    WithIndexIterator()
    {}

    explicit WithIndexIterator(typename base::base_type i) :
      base(i),
      m_index(0)
    {}

  private:
    friend class boost::iterator_core_access;

    typename base::reference dereference() const
    {
      return std::make_pair(m_index, *this->base_reference());
    }

    void increment()
    {
      ++m_index;
      ++(this->base_reference());
    }

    void decrement()
    {
      BOOST_ASSERT_MSG(m_index > 0, "with_index iterator out of range");
      --m_index;
      --(this->base_reference());
    }

    void advance(typename base::difference_type n)
    {
      m_index += n;
      BOOST_ASSERT_MSG(m_index > 0, "with_index iterator out of range");
      this->base_reference() += n;
    }
    
    typename base::difference_type m_index;
  };

  template <typename TIterator>
  WithIndexIterator<TIterator> makeWithIndexIterator(TIterator i)
  {
    return WithIndexIterator<TIterator>(i);
  }
}