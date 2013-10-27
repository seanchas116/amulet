#pragma once

#define BOOST_PP_VARIADICS

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
#define AMULET_SAFE_DECLTYPE(expr) typename std::remove_reference<decltype(expr)>::type

// if head = (AMULET_FROM_INDEX, x, xs)
//   xs.flatMap([=](decltype(xs)::value_type x){
//     using _AmuletMonadType = decltype(xs);
//     return tail;})
// else if head = (AMULET_LET_INDEX, x, y)
//   [=](decltype(y) x){return tail;}(y)
#define AMULET_DO_FOLDER(d, tail, head) \
  BOOST_PP_IF( \
    BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_FROM_INDEX), \
    BOOST_PP_TUPLE_ELEM(3,2,head).flatMap([=](const AMULET_SAFE_DECLTYPE(BOOST_PP_TUPLE_ELEM(3,2,head))::value_type & BOOST_PP_TUPLE_ELEM(3,1,head)) { \
      using _AmuletMonadType = AMULET_SAFE_DECLTYPE(BOOST_PP_TUPLE_ELEM(3,2,head)); \
      return tail; }),\
    BOOST_PP_IF( \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_LET_INDEX), \
      [=](const AMULET_SAFE_DECLTYPE(BOOST_PP_TUPLE_ELEM(3,2,head)) & BOOST_PP_TUPLE_ELEM(3,1,head)){ return tail; }(BOOST_PP_TUPLE_ELEM(3,2,head)), \
      BOOST_PP_IF( \
        BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_WHERE_INDEX), \
        (tail).filter([=](const AMULET_SAFE_DECLTYPE(tail)::value_type &){return BOOST_PP_TUPLE_ELEM(3,1,head);}), \
      ) \
    ) \
  )

#define AMULET_DO_REVERSED_LIST(reversed_list) \
  BOOST_PP_LIST_FOLD_LEFT( \
    AMULET_DO_FOLDER, \
    BOOST_PP_LIST_FIRST(reversed_list), \
    BOOST_PP_LIST_REST(reversed_list) \
  )

#define AMULET_DO(...) \
  AMULET_DO_REVERSED_LIST(BOOST_PP_LIST_REVERSE(BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))
