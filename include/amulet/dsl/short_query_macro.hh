#pragma once

#include <amulet/dsl/query_macro.hh>

#define _from(...) AMULET_FROM(__VA_ARGS__)
#define _let(...) AMULET_LET(__VA_ARGS__)
#define _select(...) AMULET_SELECT(__VA_ARGS__)
#define _where(...) AMULET_WHERE(__VA_ARGS__)
#define _do(...) AMULET_DO(__VA_ARGS__)
#define _referring(...) AMULET_REFERRING(__VA_ARGS__)
