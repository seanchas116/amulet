#pragma once

#include <amulet/range_extension.hh>
#include <amulet/type_traits.hh>

template <class TRange, class = typename std::enable_if<Amulet::IsRange<TRange>::value>::type>
inline auto operator++(const TRange &range, int) -> decltype(Amulet::extend(range))
{
  return Amulet::extend(range);
}