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

#include <sqlpp20/join/join.h>
#include <sqlpp20/join/on.h>
#include <sqlpp20/unconditional.h>

namespace sqlpp {
SQLPP_WRAPPED_STATIC_ASSERT(assert_on_is_boolean_expression,
                            "argument is not a boolean expression in on()");
SQLPP_WRAPPED_STATIC_ASSERT(assert_join_on_no_foreign_table_dependencies,
                            "on() condition must not depend on other tables");

namespace detail {
template <typename ConditionlessJoin, typename Expr>
constexpr auto check_join() {
  if constexpr (!is_expression_v<Expr> || !has_boolean_value_v<Expr>) {
    return failed<assert_on_is_boolean_expression>{};
  } else if constexpr (is_a_required_table_missing(
                           provided_tables_of_v<ConditionlessJoin>,
                           type_t<Expr>{})) {
    return failed<assert_join_on_no_foreign_table_dependencies>{};
  } else
    return succeeded{};
}
}  // namespace detail

template <Table Lhs, typename JoinType, OptionalTable Rhs>
class conditionless_join_t {
 public:
  constexpr conditionless_join_t(Lhs lhs, JoinType, Rhs rhs)
      : _lhs(lhs), _rhs(rhs) {}

  template <typename Expr>
  [[nodiscard]] constexpr auto on(const Expr& expr) const {
    constexpr auto _check = detail::check_join<conditionless_join_t, Expr>();
    if constexpr (_check) {
      return join_t{_lhs, JoinType{}, _rhs, on_t<Expr>{expr}};
    } else {
      return ::sqlpp::bad_expression_t{_check};
    }
  }

  [[nodiscard]] constexpr auto unconditionally() const {
    return join_t{_lhs, JoinType{}, _rhs, on_t<unconditional_t>{}};
  }

  Lhs _lhs;
  Rhs _rhs;
};

template <typename Lhs, typename JoinType, typename Rhs>
struct nodes_of<conditionless_join_t<Lhs, JoinType, Rhs>> {
  using type = type_vector<Lhs, Rhs>;
};

template <typename Lhs, typename JoinType, typename Rhs>
constexpr auto
    is_conditionless_join_v<conditionless_join_t<Lhs, JoinType, Rhs>> = true;

template <typename Lhs, typename JoinType, typename Rhs>
constexpr auto columns_of_v<conditionless_join_t<Lhs, JoinType, Rhs>> =
    columns_of_v<Lhs> + columns_of_v<Rhs>;

}  // namespace sqlpp
