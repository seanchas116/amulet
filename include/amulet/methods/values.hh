#pragma once

#include <amulet/method.hh>
#include <boost/range/adaptor/map.hpp>

namespace Amulet
{
  AMULET_METHOD(
    values, values,
    (0, ()),
    template <typename T>
    auto apply_(T &&range)
    {
      return range | boost::adaptors::map_values;
    }
  )
}