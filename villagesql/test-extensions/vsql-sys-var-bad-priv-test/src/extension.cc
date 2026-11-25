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

// VillageSQL test extension whose system variable requires SUPER. SUPER is a
// static privilege, which a variable cannot require, so INSTALL EXTENSION must
// fail before any variable is registered.

#include <villagesql/preview/sys_var.h>
#include <villagesql/vsql.h>

using namespace vsql;
namespace sv = vsql::preview_sys_var;

static long long g_first;
static long long g_needs_super;

static auto SYS_VARS = sv::make_capability({
    sv::make_int("first", "Declared before the bad variable", &g_first, 1, 0,
                 1000),
    sv::make_int("needs_super", "Requires a static privilege", &g_needs_super,
                 1, 0, 1000)
        .require_privilege("SUPER"),
});

VEF_GENERATE_ENTRY_POINTS(make_extension().with(SYS_VARS))
