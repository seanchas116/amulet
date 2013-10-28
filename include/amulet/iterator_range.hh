#pragma once

#include "range_facade.hh"

namespace Amulet {

  template <typename TIterator>
  class IteratorRange :
    public RangeFacade<IteratorRange<TIterator>, TIterator>
  {
  public:

    IteratorRange() = default;

    IteratorRange(TIterator begin, TIterator end) :
      mBegin(begin), mEnd(end)
    {
    }

    TIterator begin() const
    {
      return mBegin;
    }
    TIterator end() const
    {
      return mEnd;
    }

  private:

    TIterator mBegin, mEnd;
  };

}
