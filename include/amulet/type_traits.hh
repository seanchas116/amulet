#pragma once

#include <type_traits>
#include <functional>

namespace Amulet {

  template <typename T, typename Check, typename = void>
  struct CheckSubstitution
  {
    static constexpr bool value = false;
  };

  template <typename T, typename Check>
  struct  CheckSubstitution<T, Check, typename std::conditional<true, void, decltype(Check::template check<T>())>::type>
  {
    static constexpr bool value = true;
  };

  struct CheckRange
  {
    template <
      typename T,
      typename = decltype(std::begin(*static_cast<const T*>(nullptr))),
      typename = decltype(std::end(*static_cast<const T*>(nullptr)))
    >
    static int check();
  };

  template <typename T>
  using IsRange = CheckSubstitution<T, CheckRange>;

}
