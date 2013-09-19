#pragma once
#include <amulet/enumerator.hh>

namespace Amulet
{

template <typename TSourceRange>
class RangeEnumeratorImpl
{
public:

  using value_type = typename TSourceRange::value_type;

  RangeEnumeratorImpl(const TSourceRange &source) :
    m_source(source)
  {
  }
  value_type current() const
  {
    return *m_iter;
  }
  bool move_next()
  {
    if (m_started) {
      ++m_iter;
    } else {
      m_iter = m_source.begin();
      m_end = m_source.end();
      m_started = true;
    }
    return m_iter != m_end;
  }
  void reset()
  {
    m_started = false;
  }

private:

  const TSourceRange &m_source;
  bool m_started = false;
  typename TSourceRange::const_iterator m_iter, m_end;
};

template <typename TSourceRange>
auto make_enumerator_from_range(const TSourceRange &range)
{
  return make_enumerator(RangeEnumeratorImpl<TSourceRange>(range));
}

template <typename TImpl>
auto make_enumerator_from_range(Enumerator<TImpl> &&enumerator)
{
  return enumerator;
}

}