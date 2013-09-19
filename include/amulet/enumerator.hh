#pragma once

#include <type_traits>
#include <utility>

namespace Amulet
{

template <typename TImpl>
class Enumerator
{
public:

  using self_type = Enumerator<TImpl>;
  using value_type = typename TImpl::value_type;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  
  class Iterator
  {
  public:
    Iterator() {}
    Iterator(const Iterator &other) = default;
    Iterator(const Enumerator *e) : m_e(e) {}

    self_type::value_type operator*()
    {
      return m_e->current();
    }
    Iterator &operator++()
    {
      if (!m_e->move_next()) {
        m_e = nullptr;
      }
      return *this;
    }
    Iterator operator++(int)
    {
      auto original = *this;
      this->operator++();
      return original;
    }
    bool operator==(const Iterator &other)
    {
      return this->m_e == other.m_e;
    }
    bool operator!=(const Iterator &other)
    {
      return !operator==(other);
    }

  private:
    
    const Enumerator *m_e = nullptr;
  };
  
  using const_iterator = Iterator;

  Enumerator(TImpl &&impl) : m_impl(std::move(impl)) {}

  const_iterator begin() const
  {
    reset();
    move_next();
    return const_iterator(this);
  }
  const_iterator end() const
  {
    return const_iterator();
  }
  value_type current() const
  {
    return m_impl.current();
  }
  bool move_next() const
  {
    return m_impl.move_next();
  }
  void reset() const
  {
    m_impl.reset();
  }

private:

  mutable TImpl m_impl;
};

template <typename TImpl>
auto make_enumerator(TImpl &&impl)
{
  return Enumerator<TImpl>(std::move(impl));
}

}