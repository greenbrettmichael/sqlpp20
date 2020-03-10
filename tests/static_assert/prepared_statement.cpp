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

#include <sqlpp20/clause/select.h>
#include <sqlpp20/name_tag.h>
#include <sqlpp20/parameter.h>
#include <sqlpp20_test/mock_db.h>
#include <sqlpp20_test/tables/TabDepartment.h>
#include <sqlpp20_test/tables/TabEmpty.h>
#include <sqlpp20_test/tables/TabPerson.h>

#include <string>

#include "assert_bad_expression.h"

using ::sqlpp::test::assert_bad_expression;
using ::sqlpp::test::assert_good_expression;

// Turning off static_assert for statements
namespace sqlpp {
template <typename... T>
constexpr auto wrong<assert_statement_all_required_tables_are_provided, T...> =
    true;

template <typename... T>
constexpr auto wrong<assert_execute_without_parameters, T...> = true;

template <typename... T>
constexpr auto wrong<assert_statement_parameters_have_unique_names, T...> =
    true;

}  // namespace sqlpp

namespace {
SQLPP_CREATE_NAME_TAG(foo);
SQLPP_CREATE_NAME_TAG(bar);
}  // namespace

int main() {
  auto db = ::sqlpp::test::mock_db{};

  auto pi = test::tabPerson.as(foo);
  auto qu = test::tabDepartment.as(foo);

  // bad: using two table aliases with the same name
  assert_bad_expression(
      sqlpp::assert_statement_all_required_tables_are_provided{},
      db.prepare(select(all_of(pi)).from(qu).unconditionally()));

  // bad: using two parameters with identical names
  assert_bad_expression(
      sqlpp::assert_statement_parameters_have_unique_names{},
      db.prepare(
          select(test::tabPerson.id)
              .from(test::tabPerson)
              .where(::sqlpp::parameter<int>(foo) < test::tabPerson.id and
                     test::tabPerson.id < ::sqlpp::parameter<float>(foo))));

  // good: using a parameter in a prepared statement
#warning: should as() be a member function? Maybe introduce field()? `field(::sqlpp::parameter<int>(foo)).as(foo)`
  assert_good_expression(
      db.prepare(select(as(::sqlpp::parameter<int>(foo), foo))));

  // good: using two parameters with different names
  assert_good_expression(db.prepare(
      select(test::tabPerson.id)
          .from(test::tabPerson)
          .where(::sqlpp::parameter<int>(foo) < test::tabPerson.id and
                 test::tabPerson.id < ::sqlpp::parameter<float>(bar))));
}
