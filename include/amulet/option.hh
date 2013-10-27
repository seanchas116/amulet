#pragma once

#include <boost/optional.hpp>

namespace Amulet {

  struct NoneType {};

  template <typename T>
  class Option
  {
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = const T &;
    using const_reference = const T &;

    Option() {}
    Option(NoneType) {}
    Option(const T &value) :
      o(value)
    {}

    template <typename TFunc>
    void each(TFunc f) const
    {
      if (hasValue())
        f(get());
    }

    template <typename TFunc>
    Option<typename std::result_of<TFunc(T)>::type>
    map(TFunc f) const
    {
      using result = typename std::result_of<TFunc(T)>::type;
      if (hasValue())
        return Option<result>(f(get()));
      else
        return Option<result>();
    }

    template <typename TFunc>
    typename std::result_of<TFunc(T)>::type
    flatMap(TFunc f) const
    {
      using result = typename std::result_of<TFunc(T)>::type;
      static_assert(IsOption<result>::value, "flatMap result must also be Option");
      if (hasValue())
        return f(get());
      else
        return result();
    }

    const T &get() const { return o.get(); }
    T &get() { return o.get(); }

    const T *getPointer() const { return o.get_pointer(); }
    T *getPointer() { return o.get_pointer(); }

    bool hasValue() const { return o.is_initialized(); }

  private:
    
    template <typename U>
    struct IsOption
    {
      constexpr static bool value = false;
    };

    template <typename U>
    struct IsOption<Option<U>>
    {
      constexpr static bool value = true;
    };

    boost::optional<T> o;
  };

  constexpr NoneType none = NoneType();

  template <typename T>
  inline Option<T> some(const T &value)
  {
    return Option<T>(value);
  }

}