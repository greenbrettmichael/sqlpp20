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

#include <sqlpp20/to_sql_string.h>
#include <sqlpp20/type_traits.h>

namespace sqlpp {
template <typename L, typename Operator, typename R>
struct binary_t {
  binary_t() = delete;
  constexpr binary_t(L l, R r) : _l(l), _r(r) {}
  binary_t(const binary_t&) = default;
  binary_t(binary_t&&) = default;
  binary_t& operator=(const binary_t&) = default;
  binary_t& operator=(binary_t&&) = default;
  ~binary_t() = default;

  L _l;
  R _r;
};

template <typename L, typename Operator, typename R>
struct nodes_of<binary_t<L, Operator, R>> {
  using type = type_vector<L, R>;
};

template <typename L, typename Operator, typename R>
struct value_type_of<binary_t<L, Operator, R>> {
  using type = integral_t;
};

template <typename L, typename Operator, typename R>
constexpr auto requires_braces_v<binary_t<L, Operator, R>> = true;

template <typename Context, typename L, typename Operator, typename R>
[[nodiscard]] auto to_sql_string(Context& context,
                                 const binary_t<L, Operator, R>& t) {
  return to_sql_string(context, embrace(t._l)) + Operator::symbol +
         to_sql_string(context, embrace(t._r));
}

template <typename Context, typename Operator, typename R>
[[nodiscard]] auto to_sql_string(Context& context,
                                 const binary_t<none_t, Operator, R>& t) {
  return Operator::symbol + to_sql_string(context, embrace(t._r));
}

}  // namespace sqlpp
