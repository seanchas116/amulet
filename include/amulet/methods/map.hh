#pragma once

#include <amulet/method.hh>
#include <boost/range/adaptor/transformed.hpp>

namespace Amulet
{
  AMULET_METHOD_TEMPLATE(
    map, map,
    (1, ((typename, TUnaryFunc))),
    (1, ((TUnaryFunc, f))),
    template <typename T>
    auto apply_(T &&range)
    {
      return range | boost::adaptors::transformed(f);
    }
  )
}