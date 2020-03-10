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

#include <sqlpp20/name_tag.h>
#include <sqlpp20_test/tables/TabDepartment.h>
#include <sqlpp20_test/tables/TabEmpty.h>
#include <sqlpp20_test/tables/TabPerson.h>

SQLPP_CREATE_NAME_TAG(foo);

template <char... Ls, char... Rs>
constexpr auto compare(::sqlpp::char_sequence<Ls...> lhs,
                       ::sqlpp::char_sequence<Rs...> rhs) -> bool {
  if constexpr (lhs == rhs) {
    return true;
  } else {
    lhs.print_me;
    rhs.print_me;
    return false;
  }
}

template <typename T>
constexpr auto char_sequence_of(const T&) {
  return ::sqlpp::char_sequence_of_t<T>{};
}

// Tables
static_assert(compare(
    char_sequence_of(test::tabEmpty),
    ::sqlpp::char_sequence<'t', 'a', 'b', '_', 'e', 'm', 'p', 't', 'y'>{}));

static_assert(compare(char_sequence_of(test::tabPerson),
                      ::sqlpp::char_sequence<'t', 'a', 'b', '_', 'p', 'e', 'r',
                                             's', 'o', 'n'>{}));

// Table aliases
static_assert(compare(char_sequence_of(test::tabPerson.as(foo)),
                      ::sqlpp::char_sequence<'f', 'o', 'o'>{}));
static_assert(
    compare(char_sequence_of(test::tabPerson.as(test::tabDepartment.id)),
            ::sqlpp::char_sequence<'i', 'd'>{}));

// Columns
static_assert(compare(char_sequence_of(test::tabPerson.id),
                      ::sqlpp::char_sequence<'i', 'd'>{}));

// Column aliases
static_assert(compare(char_sequence_of(test::tabPerson.id.as(foo)),
                      ::sqlpp::char_sequence<'f', 'o', 'o'>{}));

int main() {}
