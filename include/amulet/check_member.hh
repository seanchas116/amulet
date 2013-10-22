#pragma once

namespace Amulet {

  template <typename T, typename Check, typename = void>
  struct CheckMember
  {
    static constexpr bool value = false;
  };

  template <typename T, typename Check>
  struct  CheckMember<T, Check, typename std::conditional<true, void, decltype(Check::template check<T>())>::type>
  {
    static constexpr bool value = true;
  };
  
  struct CheckMonadic
  {
    template <
      typename T,
      typename = typename T::value_type,
      typename = decltype(
        static_cast<const T *>(nullptr)->flatMap(std::function<T(typename T::value_type)>())
      ),
      typename = decltype(
        static_cast<const T *>(nullptr)->map(std::function<typename T::value_type(typename T::value_type)>())
      )
    >
    static int check();
  };

}