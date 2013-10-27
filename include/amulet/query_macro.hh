#pragma once

#ifndef BOOST_PP_VARIADICS
#define BOOST_PP_VARIADICS
#endif

#include "bind_operator.hh"
#include <boost/preprocessor.hpp>

#define AMULET_FROM_INDEX 1
#define AMULET_LET_INDEX 2
#define AMULET_WHERE_INDEX 3

#define AMULET_FROM(elemName, ...) (AMULET_FROM_INDEX, elemName, (__VA_ARGS__))
#define AMULET_LET(name, ...) (AMULET_LET_INDEX, name, (__VA_ARGS__))
#define AMULET_WHERE(...) (AMULET_WHERE_INDEX, (__VA_ARGS__), )
#define AMULET_SELECT(...) _AmuletMonadType::fromValue(__VA_ARGS__)

// returns reference-removed decltype of expr
#define AMULET_GET_TYPE(expr) typename std::remove_reference<decltype(expr)>::type

// if (index, arg1, arg2) = (AMULET_FROM_INDEX, x, xs)
//   xs.flatMap([=](const auto &x){
//     using _AmuletMonadType = AMULET_GET_TYPE(xs);
//     return tail;})
// else if (index, arg1, arg2) = (AMULET_LET_INDEX, x, y)
//   [=](const auto x){return tail;}(y)
// else if (index, arg1, arg2) = (AMULET_WHERE_INDEX, expr, )
//   (tail).filter([=](const auto &){return expr;})
#define AMULET_DO_FOLDER_IMPL(index, arg1, arg2, tail) \
  BOOST_PP_IF( \
    BOOST_PP_EQUAL(index, AMULET_FROM_INDEX), \
    arg2.flatMap([=](const AMULET_GET_TYPE(arg2)::value_type & arg1) { \
      using _AmuletMonadType = AMULET_GET_TYPE(arg2); \
      return tail; }),\
    BOOST_PP_IF( \
      BOOST_PP_EQUAL(index, AMULET_LET_INDEX), \
      [=](const AMULET_GET_TYPE(arg2) & arg1){ return tail; }(arg2), \
      BOOST_PP_IF( \
        BOOST_PP_EQUAL(index, AMULET_WHERE_INDEX), \
        (tail).filter([=](const AMULET_GET_TYPE(tail)::value_type &){return arg1;}), \
      ) \
    ) \
  )

#define AMULET_DO_FOLDER(d, tail, head) \
  (AMULET_DO_FOLDER_IMPL(BOOST_PP_TUPLE_ELEM(3,0,head), BOOST_PP_TUPLE_ELEM(3,1,head), BOOST_PP_TUPLE_ELEM(3,2,head), tail))

#define AMULET_DO_REVERSED_LIST(reversed_list) \
  BOOST_PP_LIST_FOLD_LEFT( \
    AMULET_DO_FOLDER, \
    BOOST_PP_LIST_FIRST(reversed_list), \
    BOOST_PP_LIST_REST(reversed_list) \
  )

#define AMULET_DO(...) \
  AMULET_DO_REVERSED_LIST(BOOST_PP_LIST_REVERSE(BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))

