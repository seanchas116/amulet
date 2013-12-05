#pragma once

#include <boost/preprocessor.hpp>

#define AMULET_FROM_INDEX 1
#define AMULET_LET_INDEX 2
#define AMULET_WHERE_INDEX 3
#define AMULET_REFERRING_INDEX 4

#define AMULET_ADD_AMPS_OP(d, data, elem) &elem
#define AMULET_ADD_AMPS(list) \
  BOOST_PP_LIST_TRANSFORM(AMULET_ADD_AMPS_OP, , list)

#define AMULET_FROM(elemName, ...) (AMULET_FROM_INDEX, elemName, (__VA_ARGS__))
#define AMULET_LET(name, ...) (AMULET_LET_INDEX, name, (__VA_ARGS__))
#define AMULET_WHERE(...) (AMULET_WHERE_INDEX, (__VA_ARGS__), )
#define AMULET_SELECT(...) _AmuletMonadType::fromValue(__VA_ARGS__)
#define AMULET_REFERRING(...) \
  (AMULET_REFERRING_INDEX, AMULET_ADD_AMPS(BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))

// returns reference-removed decltype of expr
#define AMULET_GET_TYPE(expr) typename std::remove_reference<decltype(expr)>::type

#define AMULET_LIST_ENUM_PREFIX_COMMA(list) \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_LIST_SIZE(list), 0)) \
  BOOST_PP_LIST_ENUM(list)

// if (index, arg1, arg2) = (AMULET_FROM_INDEX, x, xs)
//   xs.flatMap([=, captures...](const auto &x){
//     using _AmuletMonadType = AMULET_GET_TYPE(xs);
//     return tail;})
// else if (index, arg1, arg2) = (AMULET_LET_INDEX, x, y)
//   [=, captures...](const auto x){return tail;}(y)
// else if (index, arg1, arg2) = (AMULET_WHERE_INDEX, expr, )
//   (tail).filter([=, captures...](const auto &){return expr;})
#define AMULET_DO_FOLDER_IMPL(index, arg1, arg2, captures, tail) \
  BOOST_PP_IF( \
    BOOST_PP_EQUAL(index, AMULET_FROM_INDEX), \
    arg2.flatMap([= AMULET_LIST_ENUM_PREFIX_COMMA(captures)](const AMULET_GET_TYPE(arg2)::value_type & arg1) { \
      using _AmuletMonadType = AMULET_GET_TYPE(arg2); \
      return tail; }),\
    BOOST_PP_IF( \
      BOOST_PP_EQUAL(index, AMULET_LET_INDEX), \
      ([= AMULET_LIST_ENUM_PREFIX_COMMA(captures)](const AMULET_GET_TYPE(arg2) & arg1){ return tail; }(arg2)), \
      BOOST_PP_IF( \
        BOOST_PP_EQUAL(index, AMULET_WHERE_INDEX), \
        (tail).filter([= AMULET_LIST_ENUM_PREFIX_COMMA(captures)](const AMULET_GET_TYPE(tail)::value_type &){return arg1;}), \
      ) \
    ) \
  )

#define AMULET_ADD_CAPTURES_OP(d, data, elem) \
  BOOST_PP_LIST_TO_TUPLE((data, BOOST_PP_TUPLE_TO_LIST(elem)))

#define AMULET_ADD_CAPTURES(list, captures) \
  BOOST_PP_LIST_TRANSFORM(AMULET_ADD_CAPTURES_OP, captures, list)

#define AMULET_DO_FOLDER(d, tail, head) \
  (AMULET_DO_FOLDER_IMPL( \
    BOOST_PP_TUPLE_ELEM(4,1,head), \
    BOOST_PP_TUPLE_ELEM(4,2,head), \
    BOOST_PP_TUPLE_ELEM(4,3,head), \
    BOOST_PP_TUPLE_ELEM(4,0,head), \
    tail))

#define AMULET_DO_REVERSED_LIST(reversed_list, captures) \
  BOOST_PP_LIST_FOLD_LEFT( \
    AMULET_DO_FOLDER, \
    BOOST_PP_LIST_FIRST(reversed_list), \
    AMULET_ADD_CAPTURES(BOOST_PP_LIST_REST(reversed_list), captures) \
  )

#define AMULET_DO_LIST(list) \
  BOOST_PP_IF( \
    BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,BOOST_PP_LIST_FIRST(list)), AMULET_REFERRING_INDEX), \
    AMULET_DO_REVERSED_LIST( \
      BOOST_PP_LIST_REVERSE(BOOST_PP_LIST_REST(list)), \
      BOOST_PP_TUPLE_ELEM(3,1,BOOST_PP_LIST_FIRST(list)) \
    ), \
    AMULET_DO_REVERSED_LIST(BOOST_PP_LIST_REVERSE(list), BOOST_PP_NIL) \
  )

#define AMULET_DO(...) \
  AMULET_DO_LIST(BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

