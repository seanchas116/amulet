#pragma once

#include "range_extension.hh"
#include "iterator/int_iterator.hh"

namespace Amulet {

  template <typename T>
  inline ExtendedIteratorRange<IntIterator<T>> intRange(T first, T last)
  {
    return extend(IntIterator<T>(first), IntIterator<T>(last));
  }

  template <typename T>
  inline ExtendedIteratorRange<IntIterator<T>> times(T count)
  {
    return intRange(T(0), count);
  }

}
