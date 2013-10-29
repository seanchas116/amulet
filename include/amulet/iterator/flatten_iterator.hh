#pragma once

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/assert.hpp>
#include <type_traits>

namespace Amulet {

  template <typename TIterator>
  class FlattenIterator;

  namespace detail {

    template <class TIterator>
    struct FlattenIteratorBase
    {
      using original_value = typename std::iterator_traits<TIterator>::value_type;
      using value_type = typename original_value::value_type;
      using original_reference = typename std::iterator_traits<TIterator>::reference;
      using original_value_iterator = typename original_value::const_iterator;

      using type = boost::iterator_adaptor<
        FlattenIterator<TIterator>,
        TIterator,
        value_type,
        typename std::conditional<
          std::is_convertible<
            typename boost::iterator_traversal<TIterator>::type,
            boost::bidirectional_traversal_tag
          >::value,
          boost::bidirectional_traversal_tag,
          boost::use_default
        >::type,
        value_type,
        boost::use_default
      >;
    };

  }

/*
  template <typename TIterator>
  class FlattenIterator<TIterator, true> :
    public detail::FlattenIteratorBase<TIterator, true>::type
  {
  public:

    using indirect_base = typename detail::FlattenIteratorBase<TIterator, true>;

    FlattenIterator()
    {}

    FlattenIterator(TIterator end, TIterator i) :
      indirect_base::type(i),
      mBaseEnd(end)
    {
      if (mBaseEnd != i) {
        mIt = std::begin(*this->base());
        while (mIt == std::end(*this->base())) {
          if (++this->base_reference() == mBaseEnd) break;
          mIt = std::begin(*this->base());
        }
      }
    }

    typename indirect_base::type::reference dereference() const
    {
      return *mIt;
    }

    void increment()
    {
      ++mIt;
      while (mIt == std::end(*this->base())) {
        if (++this->base_reference() == mBaseEnd) break;
        mIt = std::begin(*this->base());
      }
    }

    void decrement()
    {
      while (this->base() == mBaseEnd || mIt == std::begin(*this->base())) {
        --this->base_reference();
        mIt = std::end(*this->base());
      }
      --mIt;
    }

    bool equal(const FlattenIterator &other) const
    {
      if (this->base() == mBaseEnd)
        return other.base() == mBaseEnd;
      else
        return this->base() == other.base() && mIt == other.mIt;
    }

  private:

    TIterator mBaseEnd;
    typename indirect_base::original_value_iterator mIt;
  };*/

  template <typename TIterator>
  class FlattenIterator :
    public detail::FlattenIteratorBase<TIterator>::type
  {
    using indirect_base = typename detail::FlattenIteratorBase<TIterator>;

  public:
    FlattenIterator() = default;

    FlattenIterator(TIterator end, TIterator i) :
      indirect_base::type(i),
      mBaseEnd(end)
    {
      if (mBaseEnd != i) {
        mValue = *i;
        mIt = std::begin(mValue);
        while (mIt == std::end(mValue)) {
          ++this->base_reference();
          if (this->base() == mBaseEnd) break;
          mValue = *this->base();
          mIt = std::begin(mValue);
        }
      }
    }

    FlattenIterator(const FlattenIterator &other) :
      indirect_base::type(other.base()),
      mBaseEnd(other.mBaseEnd)
    {
      if (this->base() != mBaseEnd) {
        auto distance = std::distance(std::begin(other.mValue), other.mIt);
        mValue = other.mValue;
        mIt = std::begin(mValue);
        std::advance(mIt, distance);
      }
    }

    FlattenIterator(FlattenIterator &&other) :
      indirect_base::type(other.base()),
      mBaseEnd(std::move(other.mBaseEnd))
    {
      const auto &constOther = other;
      if (this->base() != mBaseEnd) {
        auto distance = std::distance(std::begin(constOther.mValue), other.mIt);
        mValue = std::move(other.mValue);
        mIt = std::begin(mValue);
        std::advance(mIt, distance);
      }
    }

    FlattenIterator &operator=(const FlattenIterator &other)
    {
      this->base_reference() = other.base();
      mBaseEnd = other.mBaseEnd;
      if (this->base() != mBaseEnd) {
        auto distance = std::distance(std::begin(other.mValue), other.mIt);
        mValue = other.mValue;
        mIt = std::begin(mValue);
        std::advance(mIt, distance);
      }
      return *this;
    }

    FlattenIterator &operator=(FlattenIterator &&other)
    {
      const auto &constOther = other;
      this->base_reference() = other.base();
      mBaseEnd = std::move(other.mBaseEnd);
      if (this->base() != mBaseEnd) {
        auto distance = std::distance(std::begin(constOther.mValue), other.mIt);
        mValue = std::move(other.mValue);
        mIt = std::begin(mValue);
        std::advance(mIt, distance);
      }
      return *this;
    }

  private:

    friend class boost::iterator_core_access;

    typename indirect_base::type::reference dereference() const
    {
      return *mIt;
    }

    void increment()
    {
      BOOST_ASSERT(this->base() != mBaseEnd);
      ++mIt;
      while (mIt == std::end(mValue)) {
        if (++this->base_reference() == mBaseEnd) break;
        mValue = *this->base();
        mIt = std::begin(mValue);
      }
    }

    void decrement()
    {
      while (this->base() == mBaseEnd || mIt == std::begin(mValue)) {
        --this->base_reference();
        mValue = *this->base();
        mIt = std::end(mValue);
      }
      --mIt;
    }

    bool equal(const FlattenIterator &other) const
    {
      if (this->base() == mBaseEnd)
        return other.base() == mBaseEnd;
      else if (other.base() == mBaseEnd)
        return false;
      else
        return this->base() == other.base() && std::distance(std::begin(mValue), mIt) == std::distance(std::begin(other.mValue), other.mIt);
    }

    TIterator mBaseEnd;
    typename indirect_base::original_value mValue;
    typename indirect_base::original_value_iterator mIt;
  };

  template <typename TIterator>
  inline FlattenIterator<TIterator> makeFlattenIterator(TIterator end, TIterator i)
  {
    return FlattenIterator<TIterator>(end, i);
  }

}
