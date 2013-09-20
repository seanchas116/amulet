#pragma once

#include <amulet/method.hh>
#include <type_traits>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/value_type.hpp>

namespace Amulet
{
  namespace detail
  {
    template <class TRange>
    class WithIndexIterator;

    template <class TRange>
    struct WithIndexIteratorBase
    {
    private:
      using range_iterator = typename boost::range_iterator<TRange>::type;
      using range_value = typename boost::range_value<TRange>::type;
      using range_difference = typename boost::range_difference<TRange>::type;
      using value_type = std::pair<range_value, range_difference>;
    public:
      using type = boost::iterator_adaptor<
        WithIndexIterator<TRange>,
        range_iterator,
        value_type,
        boost::use_default,
        value_type,
        range_difference>;
    };

    template <class TRange>
    class WithIndexIterator : 
      public WithIndexIteratorBase<TRange>::type
    {
    private:
      using base = typename WithIndexIteratorBase<TRange>::type;

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
        return std::make_pair(*this->base_reference(), m_index);
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

  }

  AMULET_METHOD(
    with_index, with_index,
    (0, ()),
    template <typename T>
    auto apply_(T &&range)
    {
      using iter = detail::WithIndexIterator<typename std::remove_reference<T>::type>;
      return boost::make_iterator_range(
        iter(boost::begin(range)),
        iter(boost::end(range)));
    }
  )
}