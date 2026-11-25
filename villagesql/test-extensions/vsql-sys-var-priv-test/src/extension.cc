// Copyright (c) 2026 VillageSQL Contributors
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2.0,
// as published by the Free Software Foundation.
//
// This program is designed to work with certain software (including
// but not limited to OpenSSL) that is licensed under separate terms,
// as designated in a particular file or component or in included license
// documentation.  The authors of MySQL hereby grant you an additional
// permission to link the program and your derivative works with the
// separately licensed software that they have either included with
// the program or referenced in the documentation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License, version 2.0, for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA

// VillageSQL test extension for system variables that require a privilege to
// be set.
//
//   guarded    INT, requires VSQL_SYS_VAR_PRIV_TEST_ADMIN, a dynamic privilege
//              the server registers when the extension loads. Declared in
//              lower case to show the server upper-cases it.
//   key_label  STR, requires ENCRYPTION_KEY_ADMIN, a dynamic privilege MySQL
//              already defines.
//   open       INT, requires nothing beyond what MySQL asks for.
//
// write_guarded() sets guarded through SYS_VARS.set(), which is not a SET
// statement and so is not subject to the privilege.

#include <villagesql/preview/sys_var.h>
#include <villagesql/vsql.h>

using namespace vsql;
namespace sv = vsql::preview_sys_var;

static long long g_guarded;
static char *g_key_label;
static long long g_open;

static auto SYS_VARS = sv::make_capability({
    sv::make_int("guarded", "Settable with VSQL_SYS_VAR_PRIV_TEST_ADMIN",
                 &g_guarded, 1, 0, 1000)
        .require_privilege("vsql_sys_var_priv_test_admin"),
    sv::make_str("key_label", "Settable with ENCRYPTION_KEY_ADMIN",
                 &g_key_label, "none")
        .require_privilege("ENCRYPTION_KEY_ADMIN"),
    sv::make_int("open", "Settable with no extra privilege", &g_open, 1, 0,
                 1000),
});

// Sets guarded through the capability. Returns 1 if the set reported an
// error, 0 if it succeeded.
void write_guarded_impl(IntArg value, IntResult out) {
  if (value.is_null()) {
    out.set(1);
    return;
  }
  out.set(
      SYS_VARS.set("vsql_sys_var_priv_test", "guarded", nullptr, value.value())
          ? 1
          : 0);
}

VEF_GENERATE_ENTRY_POINTS(make_extension().with(SYS_VARS).func(
    make_func<&write_guarded_impl>("write_guarded")
        .returns(INT)
        .param(INT)
        .build()))
