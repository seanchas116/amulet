#pragma once

#include "check_member.hh"
#include <type_traits>

namespace Amulet {

  namespace detail {
    namespace {
      template <typename TMonadic, typename TFunction>
      struct CanFlatMap
      {
      private:
        using Result = typename std::result_of<TFunction(typename TMonadic::value_type)>::type;
      public:
        static constexpr bool value = CheckMember<Result, CheckMonadic>::value;
      };
    }
  }

  template <
    typename TMonadic, typename TFunction,
    typename = typename std::enable_if<
      detail::CanFlatMap<TMonadic, TFunction>::value
    >::type
  >
  inline auto operator>>=(const TMonadic &m, TFunction f) -> decltype(m.flatMap(f))
  {
    return m.flatMap(f);
  }

  template <
    typename TMonadic, typename TFunction,
    typename = typename std::enable_if<
      !detail::CanFlatMap<TMonadic, TFunction>::value
    >::type
  >
  inline auto operator>>=(const TMonadic &m, TFunction f) -> decltype(m.map(f))
  {
    return m.map(f);
  }

  namespace detail {
    namespace {}
  }

}