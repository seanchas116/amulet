#pragma once

#include <amulet/method.hh>
#include <amulet/range_enumerator.hh>

namespace Amulet
{

template <typename TSourceEnumerator, typename TProc>
class MapEnumeratorImpl
{
public:
  MapEnumeratorImpl(TSourceEnumerator &&source, TProc proc) :
    m_source(std::move(source)),
    m_proc(proc)
  {}

  using value_type = typename std::result_of<TProc(typename TSourceEnumerator::value_type)>::type;

  value_type current() const
  {
    return m_proc(m_source.current());
  }
  bool move_next()
  {
    return m_source.move_next();
  }
  void reset()
  {
    return m_source.reset();
  }

private:

  TSourceEnumerator m_source;
  TProc m_proc;
};

template <typename TSourceEnumerator, typename TProc>
auto make_map_enumerator(TSourceEnumerator &&source, TProc proc)
{
  return make_enumerator(MapEnumeratorImpl<TSourceEnumerator, TProc>(std::move(source), proc));
}

AMULET_METHOD_TEMPLATE(
  map, map,
  (1, ((typename, TUnaryFunc))),
  (1, ((TUnaryFunc, f))),
  template <typename T>
  auto apply_(const T &range)
  {
    return make_map_enumerator(make_enumerator_from_range(range), f);
  }
  template <typename T>
  auto apply_(T &&range)
  {
    return make_map_enumerator(make_enumerator_from_range(range), f);
  }
)

}
