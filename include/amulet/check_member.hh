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
      typename = decltype(&T::template flatMap<int>),
      typename = decltype(&T::template map<int>)
    >
    static int check();
  };

}