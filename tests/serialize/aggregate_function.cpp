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

#include <sqlpp20/function.h>
#include <sqlpp20/operator.h>
#include <sqlpp20_test/mock_db.h>
#include <sqlpp20_test/tables/TabDepartment.h>
#include <sqlpp20_test/tables/TabEmpty.h>
#include <sqlpp20_test/tables/TabPerson.h>

#include "assert_equality.h"

using ::sqlpp::test::assert_equality;
using ::sqlpp::test::mock_context_t;
using ::test::tabPerson;

int main() {
  try {
    assert_equality("MIN(tab_person.id)",
                    to_sql_string_c(mock_context_t{}, min(tabPerson.id)));
    assert_equality("MAX(tab_person.id)",
                    to_sql_string_c(mock_context_t{}, max(tabPerson.id)));
    assert_equality("COUNT(*)",
                    to_sql_string_c(mock_context_t{}, ::sqlpp::count(::sqlpp::asterisk)));
    assert_equality("COUNT(tab_person.id)",
                    to_sql_string_c(mock_context_t{}, count(tabPerson.id)));
    assert_equality("COUNT(DISTINCT tab_person.id)",
                    to_sql_string_c(mock_context_t{},
                                    count(::sqlpp::distinct, tabPerson.id)));
    assert_equality("AVG(tab_person.id)",
                    to_sql_string_c(mock_context_t{}, avg(tabPerson.id)));
    assert_equality("AVG(DISTINCT tab_person.id)",
                    to_sql_string_c(mock_context_t{},
                                    avg(::sqlpp::distinct, tabPerson.id)));
    assert_equality("SUM(tab_person.id)",
                    to_sql_string_c(mock_context_t{}, sum(tabPerson.id)));
    assert_equality("SUM(DISTINCT tab_person.id)",
                    to_sql_string_c(mock_context_t{},
                                    sum(::sqlpp::distinct, tabPerson.id)));
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}
