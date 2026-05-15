# External Integrations

**Analysis Date:** 2026-05-15

## APIs & External Services

**Desktop UI Framework:**
- wxWidgets - Provides GUI runtime, XRC resource loading, standard paths, widgets, and UI test simulation.
  - SDK/Client: System wxWidgets GTK3 libraries discovered through `pkg-config` in `CMakeLists.txt`.
  - Auth: Not applicable.
  - Key files: `src/presentation/presentation.cc`, `include/presentation/presentation.h`, `src/presentation/xrc/resource.xrc`, and `test/CMakeLists.txt`.

**Spreadsheet Reader:**
- OpenXLSX - Reads the local nutrition spreadsheet `spreadsheet/Taco-4a-Edicao.xlsx`.
  - SDK/Client: Vendored `OpenXLSX/` submodule linked as `OpenXLSX::OpenXLSX` in `src/core/CMakeLists.txt`.
  - Auth: Not applicable.
  - Key files: `src/core/xlsx_service.cc`, `include/core/xlsx_service.h`, and `.gitmodules`.

**External Network APIs:**
- Not detected. Non-vendored application code contains no HTTP, REST, OAuth, webhook, analytics, cloud SDK, or remote API client usage.

## Data Storage

**Databases:**
- Local SQLite database through wxSQLite3.
  - Connection: Local file path from `wxStandardPaths::Get().GetDataDir()` with filename `dietapp.db`; no environment variable is used.
  - Client: wxSQLite3 (`wxSQLite3Database`, `wxSQLite3Statement`, `wxSQLite3ResultSet`) wrapped by `SQLiteDatabaseAdapter`, `SQLiteStatement`, and `SQLiteResultSet`.
  - Schema: `schema-model/schema.sql`.
  - Tables: `users`, `foods`, `meals`, and `meal_foods`.
  - Initialization: `src/database/sqlite_database_adapter.cc` opens the database, enables foreign keys, reads `../schema-model/schema.sql`, and executes the schema.
  - Repositories: `src/database/sqlite_user_repository.cc`, `src/database/sqlite_meal_repository.cc`, and `src/database/sqlite_food_repository.cc`.

**File Storage:**
- Local filesystem only.
- UI resources: `src/presentation/xrc/resource.xrc` and `src/presentation/xrc/test_resource.xrc`.
- Spreadsheet asset: `spreadsheet/Taco-4a-Edicao.xlsx`.
- Build/test resource copies: `build/share/dietapp/xrc/` and `build/share/dietapp/spreadsheet/` from `CMakeLists.txt`.
- Installed resources: `src/presentation/xrc` installed under `${CMAKE_INSTALL_DATAROOTDIR}/dietapp` from `CMakeLists.txt`.

**Caching:**
- No external cache detected.
- In-process repository caches use `std::set` members in `include/database/sqlite_user_repository.h` and `include/database/sqlite_meal_repository.h`; they are loaded by `SQLiteUserRepository::LoadAllUsers()` and `SQLiteMealRepository::LoadAllMeals()`.

## Authentication & Identity

**Auth Provider:**
- Custom/local placeholder only.
  - Implementation: `users` table stores `username` and `display_name` in `schema-model/schema.sql`; no password column exists.
  - Credential check: `Application::ValidateUserCredentials()` in `src/core/application.cc` returns `false`, and `SQLiteUserRepository::ValidateCredentials()` in `src/database/sqlite_user_repository.cc` returns `false`.
  - UI-facing user existence and creation flows use `Application::DoesUserExist()` and `Application::CreateUserWithMeal()` in `src/core/application.cc`.
  - External auth provider: Not detected.

## Monitoring & Observability

**Error Tracking:**
- None detected. No Sentry, telemetry, analytics, crash reporting, or hosted error tracking integration appears in non-vendored code.

**Logs:**
- Console/stderr only.
- SQLite and OpenXLSX errors are written to `std::cerr` in `src/core/application.cc`, `src/core/xlsx_service.cc`, and `src/database/sqlite_food_repository.cc`.
- CMake emits configuration failures for missing dependencies in `CMakeLists.txt`.

## CI/CD & Deployment

**Hosting:**
- Not detected. The project builds a native desktop executable and defines a CMake install target in `CMakeLists.txt`.

**CI Pipeline:**
- GitHub Actions.
  - Workflow: `.github/workflows/build.yml`.
  - Trigger: pull requests targeting `develop`.
  - Runner: `ubuntu-24.04`.
  - Steps: checkout with submodules, install apt packages, configure CMake with `BUILD_TESTING=ON`, build, and run `./test/presentation_test` through `xvfb-run`.

## Environment Configuration

**Required env vars:**
- None required by application code.
- Build-time environment:
  - `PKG_CONFIG_PATH` is modified inside `CMakeLists.txt` to include `$HOME/.local/lib/pkgconfig`.
  - `WXWIDGETS_VERSION` is defined by `.github/workflows/build.yml` for apt package pinning in CI.

**Secrets location:**
- Not detected.
- No `.env` files detected.
- No credential, secret, certificate, or private key files detected in the scanned non-vendored project files.

## Webhooks & Callbacks

**Incoming:**
- None detected. This is a local desktop application with no server endpoints.

**Outgoing:**
- None detected. Non-vendored code contains no HTTP client, webhook sender, queue, or remote callback integration.

---

*Integration audit: 2026-05-15*
