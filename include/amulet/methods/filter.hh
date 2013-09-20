#pragma once

#include <amulet/method.hh>
#include <boost/range/adaptor/filtered.hpp>

namespace Amulet
{
  AMULET_METHOD_TEMPLATE(
    filter, filter,
    (1, ((typename, TUnaryFunc))),
    (1, ((TUnaryFunc, f))),
    template <typename T>
    auto apply_(T &&range)
    {
      return range | boost::adaptors::filtered(f);
    }
  )
}