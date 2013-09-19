#pragma once

#include <amulet/method.hh>
#include <amulet/range_enumerator.hh>
#include <iostream>

namespace Amulet
{
  namespace detail
  {
    template <typename TSourceEnumerator, typename TPredicate>
    class FilterEnumeratorImpl
    {
    public:
      FilterEnumeratorImpl(TSourceEnumerator &&source, TPredicate predicate) :
        m_source(std::move(source)),
        m_predicate(predicate)
      {
      }

      using value_type = typename TSourceEnumerator::value_type;

      value_type current()
      {
        return m_current;
      }

      bool move_next()
      {
        while (true) {
          if (!m_source.move_next())
            return false;
          m_current = m_source.current();
          if (m_predicate(m_current))
            return true;
        }
      }

      void reset()
      {
        m_source.reset();
      }

      TSourceEnumerator m_source;
      TPredicate m_predicate;
      value_type m_current;
    };

    template <typename TSourceEnumerator, typename TPredicate>
    auto make_filter_enumerator(TSourceEnumerator &&source, TPredicate f)
    {
      return make_enumerator(FilterEnumeratorImpl<TSourceEnumerator, TPredicate>(std::move(source), f));
    }
  }
  
  AMULET_METHOD_TEMPLATE(
    filter, filter,
    (1, ((typename, TPredicate))),
    (1, ((TPredicate, f))),
    template <typename T>
    auto apply_(const T &range)
    {
      return detail::make_filter_enumerator(make_enumerator_from_range(range), f);
    }
    template <typename T>
    auto apply_(T &&range)
    {
      return detail::make_filter_enumerator(make_enumerator_from_range(range), f);
    }
  )
}