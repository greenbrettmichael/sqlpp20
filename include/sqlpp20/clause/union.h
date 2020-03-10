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

#include <sqlpp20/clause_fwd.h>
#include <sqlpp20/flags.h>
#include <sqlpp20/result_row.h>
#include <sqlpp20/statement.h>
#include <sqlpp20/type_traits.h>
#include <sqlpp20/wrapped_static_assert.h>

namespace sqlpp {
template <typename Flag, typename LeftSelect, typename RightSelect>
struct union_t {
  Flag _flag;
  LeftSelect _left;
  RightSelect _right;
};

template <typename Flag, typename LeftSelect, typename RightSelect>
struct nodes_of<union_t<Flag, LeftSelect, RightSelect>> {
  using type = type_vector<LeftSelect, RightSelect>;
};

template <typename Flag, typename LeftSelect, typename RightSelect>
constexpr auto is_result_clause_v<union_t<Flag, LeftSelect, RightSelect>> =
    true;

template <typename Flag, typename LeftSelect, typename RightSelect>
struct clause_result_type<union_t<Flag, LeftSelect, RightSelect>> {
  using type = select_result;
};

template <typename Flag, typename LeftSelect, typename RightSelect>
constexpr auto clause_tag<union_t<Flag, LeftSelect, RightSelect>> =
    ::std::string_view{"union"};

template <typename Flag, typename LeftSelect, typename RightSelect,
          typename Statement>
class clause_base<union_t<Flag, LeftSelect, RightSelect>, Statement> {
 public:
  template <typename OtherStatement>
  clause_base(const clause_base<union_t<Flag, LeftSelect, RightSelect>,
                                OtherStatement>& s)
      : _left(s._left), _right(s._right) {}

  clause_base(const union_t<Flag, LeftSelect, RightSelect>& f)
      : _flag(f._flag), _left(f._left), _right(f._right) {}

  Flag _flag;
  LeftSelect _left;
  RightSelect _right;
};

template <typename Flag, typename LeftSelect, typename RightSelect,
          typename Statement>
struct result_row_of<
    clause_base<union_t<Flag, LeftSelect, RightSelect>, Statement>> {
  using type = result_row_of_t<LeftSelect>;
};

template <typename Context, typename Flag, typename LeftSelect,
          typename RightSelect, typename Statement>
[[nodiscard]] auto to_sql_string(
    Context& context,
    const clause_base<union_t<Flag, LeftSelect, RightSelect>, Statement>& t) {
  return to_sql_string(context, t._left) + " UNION " +
         to_sql_string(context, t._right);
}

SQLPP_WRAPPED_STATIC_ASSERT(assert_union_args_are_statements,
                            "union_() args must be sql statements");
SQLPP_WRAPPED_STATIC_ASSERT(assert_union_args_have_result_rows,
                            "union_() args have result rows (like select)");
SQLPP_WRAPPED_STATIC_ASSERT(assert_union_args_have_compatible_rows,
                            "union_() args must have compatible result rows");
SQLPP_WRAPPED_STATIC_ASSERT(assert_union_rhs_arg_without_with,
                            "union_() rhs arg must not contain a WITH clause");

template <typename LeftSelect, typename RightSelect>
constexpr auto check_union_args(const LeftSelect& l, const RightSelect& r) {
  if constexpr (not(is_statement_v<LeftSelect> &&
                    is_statement_v<RightSelect>)) {
    return failed<assert_union_args_are_statements>{};
  } else if constexpr (not(has_result_row_v<LeftSelect> &&
                           has_result_row_v<RightSelect>)) {
    return failed<assert_union_args_have_result_rows>{};
  } else if constexpr (not result_rows_are_compatible_v<
                           result_row_of_t<LeftSelect>,
                           result_row_of_t<RightSelect>>) {
    return failed<assert_union_args_have_compatible_rows>{};
  } else if constexpr (not provided_ctes_of_v<RightSelect>.empty()) {
    return failed<assert_union_rhs_arg_without_with>{};
  } else {
    return succeeded{};
  }
}

struct no_union_t {};

template <typename Statement>
class clause_base<no_union_t, Statement> {
 public:
  template <typename OtherStatement>
  constexpr clause_base(const clause_base<no_union_t, OtherStatement>& s) {}

  constexpr clause_base() = default;

  template <typename RightSelect>
  [[nodiscard]] constexpr auto union_all(RightSelect rhs) const {
    return union_all(statement_of(*this), rhs);
  }

  template <typename RightSelect>
  [[nodiscard]] constexpr auto union_distinct(RightSelect rhs) const {
    return union_all(statement_of(*this), rhs);
  }
};

template <typename Context, typename Statement>
[[nodiscard]] auto to_sql_string(Context& context,
                                 const clause_base<no_union_t, Statement>&) {
  return std::string{};
}

template <typename LeftSelect, typename RightSelect>
[[nodiscard]] constexpr auto union_all(LeftSelect l, RightSelect r) {
  constexpr auto _check = check_union_args(l, r);
  if constexpr (_check) {
    using u = union_t<all_t, LeftSelect, RightSelect>;
    return statement<u>{detail::statement_constructor_arg(u{all, l, r})};
  } else {
    return ::sqlpp::bad_expression_t{_check};
  }
}

template <typename LeftSelect, typename RightSelect>
[[nodiscard]] constexpr auto union_distinct(LeftSelect l, RightSelect r) {
  constexpr auto _check = check_union_args(l, r);
  if constexpr (_check) {
    using u = union_t<distinct_t, LeftSelect, RightSelect>;
    return statement<u>{detail::statement_constructor_arg(u{distinct, l, r})};
  } else {
    return ::sqlpp::bad_expression_t{_check};
  }
}
}  // namespace sqlpp
