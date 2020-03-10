#pragma once

/*
Copyright (c) 2019 - 2020, Roland Bock
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

#include <sqlpp20/type_traits.h>
#include <sqlpp20/type_vector.h>

namespace sqlpp::detail {
template <typename L, typename... Rs>
constexpr auto is_in_rhs() -> bool {
  return (false || ... || std::is_same_v<L, Rs>);
}

template <typename L, typename... Rs>
constexpr auto is_not_in_rhs() -> bool {
  return not is_in_rhs<L, Rs...>();
}
}  // namespace sqlpp::detail

namespace sqlpp {
template <typename... Ls, typename... Rs>
constexpr auto type_vector_is_subset_of(::sqlpp::type_vector<Ls...>,
                                        ::sqlpp::type_vector<Rs...>) {
  return (true && ... && detail::is_in_rhs<Ls, Rs...>());
}

}  // namespace sqlpp
