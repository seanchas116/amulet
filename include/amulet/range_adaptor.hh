#pragma once

#include "range_facade.hh"

namespace Amulet {

  template <typename TPolicy>
  class RangeAdaptor :
    public RangeFacade<RangeAdaptor<TPolicy>, typename TPolicy::iterator>
  {
    using base_type = RangeFacade<RangeAdaptor<TPolicy>, typename TPolicy::iterator>;
    using base_range = typename TPolicy::base_range;

  public:

    using const_iterator = typename base_type::const_iterator;
    using const_reference = typename base_type::const_reference;
    using size_type = typename base_type::size_type;

    RangeAdaptor() = default;

    template <typename... TArgs>
    explicit RangeAdaptor(const base_range &range, TArgs&&... policyArgs) :
      mRange(range),
      mPolicy(std::forward<TArgs>(policyArgs)...)
    {}

    const_iterator begin() const
    {
      return mPolicy.begin(mRange);
    }

    const_iterator end() const
    {
      return mPolicy.end(mRange);
    }

    const_reference at(size_type pos) const
    {
      if (pos >= size())
        throw std::out_of_range("Amulet::RangeAdaptor");
      auto i = begin();
      std::advance(i, pos);
      return i;
    }

    const_reference operator[](size_type pos) const
    {
      BOOST_ASSERT(pos < size());
      auto i = begin();
      std::advance(i, pos);
      return i;
    }

    size_type size() const
    {
      return std::distance(begin(), end());
    }

    bool empty() const
    {
      return size() == 0;
    }

  private:

    base_range mRange;
    TPolicy mPolicy;
  };
}
