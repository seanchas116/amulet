#pragma once

#include <boost/preprocessor.hpp>

#define AMULET_METHOD_GET_TYPE(n, array) \
  BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_ARRAY_ELEM(n, array))
#define AMULET_METHOD_GET_ARG(n, array) \
  BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_ARRAY_ELEM(n, array))

#define AMULET_METHOD_ARG_LIST_REPEATER(z, n, array) \
  BOOST_PP_COMMA_IF(n) AMULET_METHOD_GET_TYPE(n, array) AMULET_METHOD_GET_ARG(n, array)

#define AMULET_METHOD_ARG_DEFS_REPEATER(z, n, array) \
  AMULET_METHOD_GET_TYPE(n, array) AMULET_METHOD_GET_ARG(n, array);

#define AMULET_METHOD_ARG_INITS_REPEATER(z, n, array) \
  BOOST_PP_COMMA_IF(n) AMULET_METHOD_GET_ARG(n, array)(AMULET_METHOD_GET_ARG(n, array))

#define AMULET_METHOD_ARG_CALL_REPEATER(z, n, array) \
  BOOST_PP_COMMA_IF(n) AMULET_METHOD_GET_ARG(n, array)

#define AMULET_METHOD_ARRAY_REPEAT(array, macro) \
  BOOST_PP_REPEAT(BOOST_PP_ARRAY_SIZE(array), macro, array)

#define AMULET_METHOD_ARG_LIST(array) \
  AMULET_METHOD_ARRAY_REPEAT(array, AMULET_METHOD_ARG_LIST_REPEATER)

#define AMULET_METHOD_ARG_DEFS(array) \
  AMULET_METHOD_ARRAY_REPEAT(array, AMULET_METHOD_ARG_DEFS_REPEATER)

#define AMULET_METHOD_ARG_INITS(array) \
  AMULET_METHOD_ARRAY_REPEAT(array, AMULET_METHOD_ARG_INITS_REPEATER)

#define AMULET_METHOD_ARG_CALL(array) \
  AMULET_METHOD_ARRAY_REPEAT(array, AMULET_METHOD_ARG_CALL_REPEATER)

#define AMULET_METHOD(NAME, TYPE_ARG_ARRAY, ...) \
  namespace detail { \
    struct NAME##_args { \
      NAME##_args(AMULET_METHOD_ARG_LIST(TYPE_ARG_ARRAY)) \
      BOOST_PP_IF(BOOST_PP_ARRAY_SIZE(TYPE_ARG_ARRAY), : , ) \
      AMULET_METHOD_ARG_INITS(TYPE_ARG_ARRAY) \
      {} \
      AMULET_METHOD_ARG_DEFS(TYPE_ARG_ARRAY) \
      __VA_ARGS__ \
    }; \
  } \
  detail::NAME##_args NAME(AMULET_METHOD_ARG_LIST(TYPE_ARG_ARRAY)) { \
    return detail::NAME##_args(AMULET_METHOD_ARG_CALL(TYPE_ARG_ARRAY)); \
  } \
  template <typename T> \
  auto operator|(T &self, detail::NAME##_args &&args) -> decltype(args.apply_(self)) { \
    return args.apply_(self); \
  }

#define AMULET_METHOD_TEMPLATE(NAME, TEMPLATE_ARG_ARRAY, TYPE_ARG_ARRAY, ...) \
  namespace detail { \
    template <AMULET_METHOD_ARG_LIST(TEMPLATE_ARG_ARRAY)> \
    struct NAME##_args { \
      NAME##_args(AMULET_METHOD_ARG_LIST(TYPE_ARG_ARRAY)) \
      BOOST_PP_IF(BOOST_PP_ARRAY_SIZE(TYPE_ARG_ARRAY), : , ) \
      AMULET_METHOD_ARG_INITS(TYPE_ARG_ARRAY) \
      {} \
      AMULET_METHOD_ARG_DEFS(TYPE_ARG_ARRAY) \
      __VA_ARGS__ \
    }; \
  } \
  template <AMULET_METHOD_ARG_LIST(TEMPLATE_ARG_ARRAY)> \
  detail::NAME##_args<AMULET_METHOD_ARG_CALL(TEMPLATE_ARG_ARRAY)> NAME(AMULET_METHOD_ARG_LIST(TYPE_ARG_ARRAY)) { \
    return detail::NAME##_args<AMULET_METHOD_ARG_CALL(TEMPLATE_ARG_ARRAY)>(AMULET_METHOD_ARG_CALL(TYPE_ARG_ARRAY)); \
  } \
  template <typename T, AMULET_METHOD_ARG_LIST(TEMPLATE_ARG_ARRAY)> \
  auto operator|(T &self, detail::NAME##_args<AMULET_METHOD_ARG_CALL(TEMPLATE_ARG_ARRAY)> &&args) -> decltype(args.apply_(self)) { \
    return args.apply_(self); \
  }

#define AMULET_METHOD0(NAME,  ...) \
  AMULET_METHOD(NAME, (0, ()), __VA_ARGS__)
#define AMULET_METHOD1(NAME, T0, A0,  ...) \
  AMULET_METHOD(NAME, (1, ((T0, A0))), __VA_ARGS__)
#define AMULET_METHOD2(NAME, T0, A0, T1, A1,  ...) \
  AMULET_METHOD(NAME, (2, ((T0, A0), (T1, A1))), __VA_ARGS__)
#define AMULET_METHOD3(NAME, T0, A0, T1, A1, T2, A2,  ...) \
  AMULET_METHOD(NAME, (3, ((T0, A0), (T1, A1), (T2, A2))), __VA_ARGS__)
#define AMULET_METHOD4(NAME, T0, A0, T1, A1, T2, A2, T3, A3,  ...) \
  AMULET_METHOD(NAME, (4, ((T0, A0), (T1, A1), (T2, A2), (T3, A3))), __VA_ARGS__)
#define AMULET_METHOD5(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4,  ...) \
  AMULET_METHOD(NAME, (5, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4))), __VA_ARGS__)
#define AMULET_METHOD6(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5,  ...) \
  AMULET_METHOD(NAME, (6, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5))), __VA_ARGS__)
#define AMULET_METHOD7(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6,  ...) \
  AMULET_METHOD(NAME, (7, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6))), __VA_ARGS__)
#define AMULET_METHOD8(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7,  ...) \
  AMULET_METHOD(NAME, (8, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7))), __VA_ARGS__)
#define AMULET_METHOD9(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8,  ...) \
  AMULET_METHOD(NAME, (9, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8))), __VA_ARGS__)
#define AMULET_METHOD10(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9,  ...) \
  AMULET_METHOD(NAME, (10, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9))), __VA_ARGS__)
#define AMULET_METHOD11(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10,  ...) \
  AMULET_METHOD(NAME, (11, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10))), __VA_ARGS__)
#define AMULET_METHOD12(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11,  ...) \
  AMULET_METHOD(NAME, (12, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11))), __VA_ARGS__)
#define AMULET_METHOD13(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12,  ...) \
  AMULET_METHOD(NAME, (13, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12))), __VA_ARGS__)
#define AMULET_METHOD14(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13,  ...) \
  AMULET_METHOD(NAME, (14, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13))), __VA_ARGS__)
#define AMULET_METHOD15(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14,  ...) \
  AMULET_METHOD(NAME, (15, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14))), __VA_ARGS__)
#define AMULET_METHOD16(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15,  ...) \
  AMULET_METHOD(NAME, (16, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15))), __VA_ARGS__)
#define AMULET_METHOD17(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16,  ...) \
  AMULET_METHOD(NAME, (17, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16))), __VA_ARGS__)
#define AMULET_METHOD18(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16, T17, A17,  ...) \
  AMULET_METHOD(NAME, (18, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16), (T17, A17))), __VA_ARGS__)
#define AMULET_METHOD19(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16, T17, A17, T18, A18,  ...) \
  AMULET_METHOD(NAME, (19, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16), (T17, A17), (T18, A18))), __VA_ARGS__)
#define AMULET_METHOD20(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16, T17, A17, T18, A18, T19, A19,  ...) \
  AMULET_METHOD(NAME, (20, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16), (T17, A17), (T18, A18), (T19, A19))), __VA_ARGS__)
#define AMULET_METHOD21(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16, T17, A17, T18, A18, T19, A19, T20, A20,  ...) \
  AMULET_METHOD(NAME, (21, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16), (T17, A17), (T18, A18), (T19, A19), (T20, A20))), __VA_ARGS__)
#define AMULET_METHOD22(NAME, T0, A0, T1, A1, T2, A2, T3, A3, T4, A4, T5, A5, T6, A6, T7, A7, T8, A8, T9, A9, T10, A10, T11, A11, T12, A12, T13, A13, T14, A14, T15, A15, T16, A16, T17, A17, T18, A18, T19, A19, T20, A20, T21, A21,  ...) \
  AMULET_METHOD(NAME, (22, ((T0, A0), (T1, A1), (T2, A2), (T3, A3), (T4, A4), (T5, A5), (T6, A6), (T7, A7), (T8, A8), (T9, A9), (T10, A10), (T11, A11), (T12, A12), (T13, A13), (T14, A14), (T15, A15), (T16, A16), (T17, A17), (T18, A18), (T19, A19), (T20, A20), (T21, A21))), __VA_ARGS__)


