#pragma once

#define BOOST_PP_VARIADICS

#include "bind_operator.hh"
#include <boost/preprocessor.hpp>

#define AMULET_FROM_INDEX 1
#define AMULET_LET_INDEX 2
#define AMULET_WHERE_INDEX 3

#define AMULET_FROM(elemName, monad) (AMULET_FROM_INDEX, elemName, monad)
#define AMULET_LET(name, expr) (AMULET_LET_INDEX, name, expr)
#define AMULET_WHERE(expr) (AMULET_WHERE_INDEX, expr, )
#define AMULET_SELECT(...) _AmuletMonadType::fromValue(__VA_ARGS__)

// if head = (AMULET_FROM_INDEX, x, xs)
//   xs.flatMap([=](decltype(xs)::value_type x){
//     using _AmuletMonadType = decltype(xs);
//     return tail;})
// else if head = (AMULET_LET_INDEX, x, y)
//   [=](decltype(y) x){return tail;}(y)
#define AMULET_DO_FOLDER(d, tail, head) \
  BOOST_PP_IF( \
    BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_FROM_INDEX), \
    BOOST_PP_TUPLE_ELEM(3,2,head).flatMap([=](const decltype(BOOST_PP_TUPLE_ELEM(3,2,head))::value_type & BOOST_PP_TUPLE_ELEM(3,1,head)) { \
      using _AmuletMonadType = decltype(BOOST_PP_TUPLE_ELEM(3,2,head)); \
      return tail; }),\
    BOOST_PP_IF( \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_LET_INDEX), \
      [=](const decltype(BOOST_PP_TUPLE_ELEM(3,2,head)) & BOOST_PP_TUPLE_ELEM(3,1,head)){ return tail; }(BOOST_PP_TUPLE_ELEM(3,2,head)), \
      BOOST_PP_IF( \
        BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(3,0,head), AMULET_WHERE_INDEX), \
        (tail).filter([=](const decltype(tail)::value_type &){return BOOST_PP_TUPLE_ELEM(3,1,head);}), \
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
