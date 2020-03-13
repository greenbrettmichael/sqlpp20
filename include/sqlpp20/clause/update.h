#pragma once

/*
Copyright (c) 2016 - 2020, Roland Bock
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sqlpp20/clause/update_set.h>
#include <sqlpp20/clause/where.h>
#include <sqlpp20/clause_fwd.h>
#include <sqlpp20/type_traits.h>

namespace sqlpp {
template <typename Table>
struct update_t {
  Table _table;
};

template <typename Table>
struct nodes_of<update_t<Table>> {
  using type = type_vector<Table>;
};

template <typename Table>
constexpr auto clause_tag<update_t<Table>> = ::std::string_view{"update"};

template <typename Table, typename Statement>
class clause_base<update_t<Table>, Statement> {
 public:
  template <typename OtherStatement>
  clause_base(const clause_base<update_t<Table>, OtherStatement>& s)
      : _table(s._table) {}

  clause_base(Table table) : _table(table) {}

  Table _table;
};

template <typename Context, typename Table, typename Statement>
[[nodiscard]] auto to_sql_string(
    Context& context, const clause_base<update_t<Table>, Statement>& t) {
  return std::string("UPDATE ") + to_sql_string(context, t._table);
}

template <PrimaryTable Table>
[[nodiscard]] constexpr auto update(Table table) {
  return statement<update_t<Table>>{table}
         << statement<no_update_set_t, no_where_t>{};
}

}  // namespace sqlpp
