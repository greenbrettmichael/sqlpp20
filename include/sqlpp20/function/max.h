#pragma once

/*
Copyright (c) 2018 - 2020, Roland Bock
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

#include <sqlpp20/aggregate.h>
#include <sqlpp20/bad_expression.h>
#include <sqlpp20/flags.h>
#include <sqlpp20/type_traits.h>
#include <sqlpp20/wrapped_static_assert.h>

namespace sqlpp {
SQLPP_WRAPPED_STATIC_ASSERT(assert_max_arg_is_expression,
                            "max() arg must be a value expression");
SQLPP_WRAPPED_STATIC_ASSERT(assert_max_arg_is_not_alias,
                            "max() arg must not be an alias");
SQLPP_WRAPPED_STATIC_ASSERT(assert_max_arg_is_not_aggregate,
                            "max() arg must not be an aggregate");

template <typename Expression>
constexpr auto check_max_args() {
  if constexpr (not is_expression_v<Expression>) {
    return failed<assert_max_arg_is_expression>{};
  } else if constexpr (is_alias_v<Expression>) {
    return failed<assert_max_arg_is_not_alias>{};
  } else if constexpr (::sqlpp::is_aggregate_v<Expression>) {
    return failed<assert_max_arg_is_not_aggregate>{};
  } else
    return succeeded{};
}

template <typename ValueType>
struct max_t {
  static constexpr auto name = std::string_view{"MAX"};
  using flag_type = no_flag_t;
  using value_type = ValueType;
};

template <typename Expression>
[[nodiscard]] constexpr auto max(Expression expression) {
  if constexpr (constexpr auto _check = check_max_args<Expression>(); _check) {
    return aggregate_t<max_t<value_type_of_t<Expression>>, Expression>{
        expression};
  } else {
    return ::sqlpp::bad_expression_t{_check};
  }
}

}  // namespace sqlpp
