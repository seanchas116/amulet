#pragma once

#include <amulet/range_extension.hh>
#include <amulet/type_traits.hh>

template <class TRange>
inline auto operator++(const TRange &range, int) ->
  typename std::enable_if<
    Amulet::IsRange<TRange>::value,
    decltype(Amulet::extend(range))
  >::type
{
  return Amulet::extend(range);
}