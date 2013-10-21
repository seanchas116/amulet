#pragma once

namespace Amulet {

  template <typename T, typename Check>
  struct CheckMember
  {
  private:
    struct General_ {};
    struct Special_ : General_ {};
    
    template <typename U, typename = decltype(Check::template check<U>())>
    constexpr static bool impl(Special_) { return true; }
    template <typename U>
    constexpr static bool impl(General_) { return false; }

  public:
    constexpr static bool value = impl<T>(Special_());
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