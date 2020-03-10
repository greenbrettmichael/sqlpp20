#pragma once

/*
Copyright (c) 2017 - 2020, Roland Bock
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

#include <sqlpp20/clause_fwd.h>
#include <sqlpp20/statement.h>
#include <sqlpp20/type_traits.h>
#include <sqlpp20/wrapped_static_assert.h>

namespace sqlpp::mysql {
namespace clause {
struct using_ {};
}  // namespace clause

template <typename Table>
struct using_t {
  Table _table;
};

template <typename Table>
constexpr auto clause_tag<using_t<Table>> = clause::using_{};

template <typename Table, typename Statement>
class clause_base<using_t<Table>, Statement> {
 public:
  template <typename OtherStatement>
  clause_base(const clause_base<using_t<Table>, OtherStatement>& s)
      : _table(s._table) {}

  clause_base(const using_t<Table>& f) : _table(f._table) {}

  Table _table;
};

template <typename Context, typename Table, typename Statement>
decltype(auto) operator<<(Context& context,
                          const clause_base<using_t<Table>, Statement>& t) {
  return context << " INTO " << t._table;
}

#warning allow joins here, too
SQLPP_WRAPPED_STATIC_ASSERT(assert_using_arg_is_table,
                            "using_() arg has to be a table");
SQLPP_WRAPPED_STATIC_ASSERT(assert_using_arg_no_read_only_table,
                            "using_() arg must not be read-only table");
SQLPP_WRAPPED_STATIC_ASSERT(assert_using_arg_no_required_tables,
                            "using_() arg must not depend on other tables");

template <typename T>
constexpr auto check_using_arg(const T&) {
  if constexpr (!is_table_v<T>) {
    return failed<assert_using_arg_is_table>{};
  } else if constexpr (is_read_only_table_v<T>) {
    return failed<assert_using_arg_no_read_only_table>{};
  } else if constexpr (!required_tables_of_v<T>.empty()) {
    return failed<assert_using_arg_no_required_tables>{};
  } else
    return succeeded{};
}

struct no_using_t {};

template <typename Statement>
class clause_base<no_using_t, Statement> {
 public:
  template <typename OtherStatement>
  constexpr clause_base(const clause_base<no_using_t, OtherStatement>& s) {}

  constexpr clause_base() = default;

  template <typename Table>
  [[nodiscard]] constexpr auto using_(Table t) const {
    constexpr auto _check = check_using_arg(t);
    if constexpr (_check) {
      return Statement::of(this).template replace_clause<no_using_t>(
          using_t<Table>{t});
    } else {
      return ::sqlpp::bad_expression_t{_check};
    }
  }
};

template <typename Context, typename Statement>
decltype(auto) operator<<(Context& context,
                          const clause_base<no_using_t, Statement>&) {
  return context;
}

template <typename Table>
[[nodiscard]] constexpr auto using_(Table&& t) {
  return statement<no_using_t>{}.using_(std::forward<Table>(t));
}
}  // namespace sqlpp::mysql
