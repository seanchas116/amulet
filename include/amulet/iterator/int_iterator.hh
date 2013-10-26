#pragma once

#include <boost/iterator/iterator_facade.hpp>

namespace Amulet
{

  template <typename T>
  class IntIterator :
    public boost::iterator_facade<
      IntIterator<T>,
      T,
      boost::random_access_traversal_tag,
      T
    >
  {
  public:
    IntIterator()
    {}
    IntIterator(T value) :
      mValue(value)
    {}
  private:

    friend class boost::iterator_core_access;

    T dereference() const
    {
      return mValue;
    }

    bool equal(const IntIterator &other) const
    {
      return mValue == other.mValue;
    }

    void increment()
    {
      ++mValue;
    }

    void decrement()
    {
      --mValue;
    }

    void advance(ptrdiff_t d)
    {
      mValue += d;
    }

    std::ptrdiff_t distance_to(const IntIterator &other) const
    {
      return other.mValue - mValue;
    }

    T mValue;
  };

}