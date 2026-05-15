# Technology Stack

**Analysis Date:** 2026-05-15

## Languages

**Primary:**
- C++17 - Application, domain, persistence, and wxWidgets presentation code in `src/` and public headers in `include/`; enforced by `CMakeLists.txt`.

**Secondary:**
- SQL - SQLite schema in `schema-model/schema.sql` and repository query constants in `include/database/sqlite_food_repository.h`, `include/database/sqlite_user_repository.h`, and `include/database/sqlite_meal_repository.h`.
- XRC XML - wxWidgets UI resources in `src/presentation/xrc/resource.xrc` and test resources in `src/presentation/xrc/test_resource.xrc`.
- YAML - GitHub Actions CI workflow in `.github/workflows/build.yml`.
- pkg-config metadata - Local wxWidgets detection helper in `pkgconfig/libwx_gtk3u_core-3.2.pc`.

## Runtime

**Environment:**
- Native Linux desktop application using wxWidgets GTK3; CI runs on `ubuntu-24.04` in `.github/workflows/build.yml`.
- wxWidgets runtime requires version `3.2.4+`; `CMakeLists.txt` checks `wx-config --version` and fails configuration below `3.2.4`.
- SQLite runs embedded through wxSQLite3; the application opens `dietapp.db` under `wxStandardPaths::Get().GetDataDir()` in `src/database/sqlite_database_adapter.cc`.

**Package Manager:**
- System package manager: apt, documented in `README.md` and used in `.github/workflows/build.yml`.
- C/C++ package discovery: `pkg-config`, required by `CMakeLists.txt`.
- Lockfile: Not detected.
- Vendored dependency: `OpenXLSX/` is a Git submodule declared in `.gitmodules` at commit `5723411d47643ce3b5b9994064c26ca8cd841f13` (`v0.3.2-227-g5723411`).

## Frameworks

**Core:**
- wxWidgets 3.2.4+ - Desktop UI framework, XRC resource loading, standard data paths, and GUI event/runtime support; configured in `CMakeLists.txt`, `src/presentation/CMakeLists.txt`, and `src/core/CMakeLists.txt`.
- wxSQLite3 3.2 - Embedded SQLite database client; configured in `CMakeLists.txt` and `src/database/CMakeLists.txt`, implemented by `src/database/sqlite_database_adapter.cc`.
- OpenXLSX vendored submodule - XLSX spreadsheet reader used by `src/core/xlsx_service.cc`; linked as `OpenXLSX::OpenXLSX` in `src/core/CMakeLists.txt`.

**Testing:**
- Catch2 with main - Test runner and assertion framework discovered via `pkg_check_modules(CATCH2 REQUIRED catch2-with-main)` in `CMakeLists.txt`.
- wxWidgets UI simulation - Tests compile with `wxUSE_UIACTIONSIMULATOR=1` in `test/CMakeLists.txt` and run under `xvfb-run` in `.github/workflows/build.yml`.

**Build/Dev:**
- CMake 3.5 minimum - Top-level build orchestration in `CMakeLists.txt`, with layered object-library targets in `src/core/CMakeLists.txt`, `src/database/CMakeLists.txt`, and `src/presentation/CMakeLists.txt`.
- GNUInstallDirs - Install path handling in `CMakeLists.txt`.
- clang-format - Formatting configuration in `.clang-format`; style is Google-derived with 2-space indentation and same-line braces.
- clangd - Editor compile flag hints in `.clangd`.

## Key Dependencies

**Critical:**
- `libwxgtk3.2-dev` / wxWidgets GTK3 - Required for the GUI, `wxStandardPaths`, XRC loading, and test UI simulation; imported throughout `include/presentation/` and `src/presentation/`.
- `libwxsqlite3-3.2-dev` / wxSQLite3 - Required for database storage and prepared statements; imported by `include/database/sqlite_database_adapter.h`, `include/database/sqlite_statement.h`, and repository implementations in `src/database/`.
- `OpenXLSX` - Required to load `spreadsheet/Taco-4a-Edicao.xlsx` and seed food nutrition data; used by `include/core/xlsx_service.h` and `src/core/xlsx_service.cc`.
- `catch2` - Required even when configuring by default because `BUILD_TESTING` defaults to `ON` in `CMakeLists.txt`.

**Infrastructure:**
- `pkg-config` - Required by `CMakeLists.txt` for Catch2, wxWidgets, and wxSQLite3 module discovery.
- `libgtk-3-dev` - Linux GUI development dependency installed in `.github/workflows/build.yml` and documented in `README.md`.
- `build-essential` - Linux compiler/toolchain dependency documented in `README.md` and installed in `.github/workflows/build.yml`.
- `xvfb` - CI-only virtual display dependency for running wxWidgets tests headlessly in `.github/workflows/build.yml`.

## Configuration

**Environment:**
- No `.env` files detected in the repository root.
- `CMakeLists.txt` writes `pkgconfig/libwx_gtk3u_core-3.2.pc` into `$HOME/.local/lib/pkgconfig` and prepends that directory to `PKG_CONFIG_PATH` during configuration.
- Runtime resources are resolved through `wxStandardPaths::Get().GetDataDir()`:
  - Database: `dietapp.db` in `src/database/sqlite_database_adapter.cc`.
  - Spreadsheet: `spreadsheet/Taco-4a-Edicao.xlsx` in `src/core/xlsx_service.cc`.
  - XRC: `xrc/resource.xrc` in `src/presentation/presentation.cc`.
- Test-enabled builds copy `src/presentation/xrc/` and `spreadsheet/` into `build/share/dietapp/` via the `copy_test_resources` target in `CMakeLists.txt`.

**Build:**
- Primary config: `CMakeLists.txt`.
- Layer configs: `src/CMakeLists.txt`, `src/core/CMakeLists.txt`, `src/database/CMakeLists.txt`, `src/presentation/CMakeLists.txt`, and `test/CMakeLists.txt`.
- Formatting config: `.clang-format`.
- Editor tooling config: `.clangd`.
- CI config: `.github/workflows/build.yml`.
- Generated build outputs: `build/` and `compile_commands.json`; treat `build/` as generated.

## Platform Requirements

**Development:**
- Linux desktop development environment with CMake, a C++17 compiler, `pkg-config`, wxWidgets GTK3 3.2.4+, wxSQLite3 3.2, Catch2, and GTK3 development libraries.
- Use out-of-tree builds:
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
  - `cmake --build build`
- Use `BUILD_TESTING=ON` for test builds; this is the default in `CMakeLists.txt`.

**Production:**
- Native executable target: `dietapp`, created in `CMakeLists.txt`.
- Install target places `dietapp` in `${CMAKE_INSTALL_BINDIR}` and installs `src/presentation/xrc` into `${CMAKE_INSTALL_DATAROOTDIR}/dietapp`.
- No container, package manifest, app bundle, or hosted deployment target detected.

---

*Stack analysis: 2026-05-15*
