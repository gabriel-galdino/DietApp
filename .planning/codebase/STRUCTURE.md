# Codebase Structure

**Analysis Date:** 2026-05-15

## Directory Layout

```text
DietApp/
├── AGENTS.md                 # Repository-specific contributor and agent guidance
├── CMakeLists.txt            # Top-level build, dependency, executable, install, and test resource setup
├── README.md                 # Project README
├── include/                  # Public project headers grouped by architectural layer
│   ├── core/                 # Application, domain, DTO, and repository port headers
│   ├── database/             # Database adapter and SQLite repository headers
│   └── presentation/         # wxWidgets presentation, navigation, model, and page headers
├── src/                      # C++ implementation files and CMake object-library targets
│   ├── core/                 # Application coordinator, domain implementation, spreadsheet service, wx bootstrap
│   ├── database/             # SQLite adapter/repository implementations
│   └── presentation/         # XRC-backed presentation shell, pages, and data-view model
├── src/presentation/xrc/     # wxWidgets XRC resources for production and tests
├── test/                     # Catch2/wxWidgets tests and fakes
├── schema-model/             # SQLite schema file consumed at database initialization
├── spreadsheet/              # TACO spreadsheet asset consumed by XlsxService
├── pkgconfig/                # Local wxWidgets pkg-config helper file copied during CMake configure
├── OpenXLSX/                 # Vendored third-party spreadsheet library dependency
├── build/                    # Generated CMake build output
└── .planning/codebase/       # Generated GSD codebase map documents
```

## Directory Purposes

**`include/core/`:**
- Purpose: Define the application boundary and core domain types.
- Contains: `Application`, `IApplication`, domain objects, DTO structs, core error messages, repository port interfaces, and `XlsxService`.
- Key files: `include/core/application.h`, `include/core/i_application.h`, `include/core/xlsx_service.h`, `include/core/domain/user.h`, `include/core/domain/meal.h`, `include/core/domain/food.h`

**`include/core/ports/repository/`:**
- Purpose: Define persistence contracts owned by the core layer.
- Contains: `UserRepository`, `MealRepository`, and `FoodRepository` interfaces.
- Key files: `include/core/ports/repository/user_repository.h`, `include/core/ports/repository/meal_repository.h`, `include/core/ports/repository/food_repository.h`

**`include/core/dto/`:**
- Purpose: Define cross-layer data-transfer structs used by `IApplication` and navigation flows.
- Contains: `FoodDTO`, `MealDTO`, `MealsFromUserDTO`, `CreateUserWithMealDTO`, `AddMealToUserDTO`, `UserDTO`.
- Key files: `include/core/dto/food_dto.h`, `include/core/dto/meal_dto.h`, `include/core/dto/user_dto.h`

**`include/database/`:**
- Purpose: Declare database abstraction interfaces and SQLite-backed concrete classes.
- Contains: `DatabaseAdapter`, `DatabaseStatement`, `DatabaseResultSet`, `DatabaseManager`, SQLite adapter/statement/result-set/repository headers.
- Key files: `include/database/database_adapter.h`, `include/database/database_manager.h`, `include/database/sqlite_database_adapter.h`, `include/database/sqlite_user_repository.h`, `include/database/sqlite_meal_repository.h`, `include/database/sqlite_food_repository.h`

**`include/presentation/`:**
- Purpose: Declare wxWidgets presentation shell, navigation interface, nutritional data model, and page controllers.
- Contains: `Presentation`, `INavigation`, `NutritionalModel`, error message constants, and page headers.
- Key files: `include/presentation/presentation.h`, `include/presentation/navigation.h`, `include/presentation/nutritional_model.h`, `include/presentation/pages/register_page.h`, `include/presentation/pages/create_page.h`

**`src/core/`:**
- Purpose: Implement application orchestration, wx bootstrap, domain constructors, and spreadsheet import logic.
- Contains: `application.cc`, `wx_bootstrap_app.cc`, `xlsx_service.cc`, domain implementation files, and core CMake target definition.
- Key files: `src/core/application.cc`, `src/core/wx_bootstrap_app.cc`, `src/core/xlsx_service.cc`, `src/core/CMakeLists.txt`

**`src/database/`:**
- Purpose: Implement SQLite database adapter, statement/result-set wrappers, and repository behavior.
- Contains: `database_manager.cc`, `sqlite_database_adapter.cc`, `sqlite_statement.cc`, `sqlite_result_set.cc`, and SQLite repository implementations.
- Key files: `src/database/sqlite_database_adapter.cc`, `src/database/sqlite_user_repository.cc`, `src/database/sqlite_meal_repository.cc`, `src/database/sqlite_food_repository.cc`, `src/database/CMakeLists.txt`

**`src/presentation/`:**
- Purpose: Implement the presentation shell, wx data model, and page event controllers.
- Contains: `presentation.cc`, `nutritional_model.cc`, `pages/*.cc`, `xrc/*.xrc`, and presentation CMake target definition.
- Key files: `src/presentation/presentation.cc`, `src/presentation/nutritional_model.cc`, `src/presentation/pages/initial_page.cc`, `src/presentation/pages/enter_page.cc`, `src/presentation/pages/register_page.cc`, `src/presentation/pages/create_page.cc`

**`src/presentation/xrc/`:**
- Purpose: Store wxWidgets XML resource layouts.
- Contains: Production `resource.xrc` and test `test_resource.xrc`.
- Key files: `src/presentation/xrc/resource.xrc`, `src/presentation/xrc/test_resource.xrc`

**`test/`:**
- Purpose: Store Catch2 tests, wx test utilities, and test doubles for core interfaces.
- Contains: `presentation_test.cc`, `FakeApplication`, `TestableFrame`, and test CMake target definition.
- Key files: `test/presentation_test.cc`, `test/core/fake_application.h`, `test/core/fake_application.cc`, `test/wx/testableframe.h`, `test/wx/testableframe.cc`, `test/CMakeLists.txt`

**`schema-model/`:**
- Purpose: Store the SQLite schema loaded by `SQLiteDatabaseAdapter`.
- Contains: Table definitions for users, foods, meals, and meal-food rows.
- Key files: `schema-model/schema.sql`

**`spreadsheet/`:**
- Purpose: Store the source food spreadsheet consumed by `XlsxService`.
- Contains: TACO spreadsheet workbook.
- Key files: `spreadsheet/Taco-4a-Edicao.xlsx`

**`OpenXLSX/`:**
- Purpose: Vendored third-party library used by `XlsxService`.
- Contains: External OpenXLSX source and CMake definitions.
- Key files: Treat the directory as a dependency boundary; use `include/core/xlsx_service.h` and `src/core/xlsx_service.cc` for application-specific spreadsheet behavior.

## Key File Locations

**Entry Points:**
- `src/core/wx_bootstrap_app.cc`: wxWidgets application entry point and process lifecycle.
- `src/core/application.cc`: application composition root and use-case coordinator.
- `test/presentation_test.cc`: presentation test executable entry and UI simulation setup.

**Configuration:**
- `CMakeLists.txt`: project version, C++ standard, dependencies, executable target, resource copying, and install rules.
- `src/CMakeLists.txt`: subdirectory registration for layer targets.
- `src/core/CMakeLists.txt`: `application` object library and OpenXLSX/wxWidgets linkage.
- `src/database/CMakeLists.txt`: `database` object library and wxSQLite3 linkage.
- `src/presentation/CMakeLists.txt`: `presentation` object library and wxWidgets linkage.
- `test/CMakeLists.txt`: `presentation_test` executable and test dependencies.
- `pkgconfig/libwx_gtk3u_core-3.2.pc`: local pkg-config helper copied into `~/.local/lib/pkgconfig` by CMake.

**Core Logic:**
- `include/core/i_application.h`: application interface consumed by presentation and fakes.
- `include/core/application.h`, `src/core/application.cc`: dependency ownership, startup sequence, and user/meal/food workflows.
- `include/core/domain/*.h`, `src/core/domain/*.cc`: domain entities.
- `include/core/dto/*.h`: transfer objects used across presentation/core boundaries.
- `include/core/xlsx_service.h`, `src/core/xlsx_service.cc`: spreadsheet loading and food DTO/domain construction.

**Persistence:**
- `include/core/ports/repository/*.h`: repository contracts.
- `include/database/database_adapter.h`: database connection/transaction/prepare interface.
- `include/database/database_statement.h`, `include/database/database_result_set.h`: statement and result-set interfaces.
- `include/database/sqlite_*.h`, `src/database/sqlite_*.cc`: SQLite concrete implementations.
- `schema-model/schema.sql`: database schema applied during initialization.

**Presentation:**
- `include/presentation/presentation.h`, `src/presentation/presentation.cc`: XRC load, page construction, navigation implementation.
- `include/presentation/navigation.h`: page navigation interface.
- `include/presentation/pages/*.h`, `src/presentation/pages/*.cc`: page controllers.
- `include/presentation/nutritional_model.h`, `src/presentation/nutritional_model.cc`: create-page food selection model.
- `src/presentation/xrc/resource.xrc`: production UI resource file.
- `src/presentation/xrc/test_resource.xrc`: test UI resource file.

**Testing:**
- `test/presentation_test.cc`: Catch2 tests for presentation behavior.
- `test/core/fake_application.*`: fake implementation of `IApplication`.
- `test/wx/testableframe.*`: wx frame test helper.
- `test/CMakeLists.txt`: test executable target definition.

## Naming Conventions

**Files:**
- Use `.cc` for C++ implementation files: `src/core/application.cc`, `src/presentation/pages/create_page.cc`.
- Use `.h` for public headers under `include/`: `include/database/sqlite_database_adapter.h`.
- Name concrete SQLite persistence classes with the `sqlite_` file prefix and `SQLite...` class prefix: `include/database/sqlite_user_repository.h`, `SQLiteUserRepository`.
- Name page controllers with the `_page` file suffix and `...Page` class suffix: `include/presentation/pages/register_page.h`, `RegisterPage`.
- Use resource files with `.xrc`: `src/presentation/xrc/resource.xrc`.
- Use CMake target files named `CMakeLists.txt` in each build directory.

**Directories:**
- Match architectural layers at the top of `include/` and `src/`: `core/`, `database/`, `presentation/`.
- Place domain headers under `include/core/domain/`.
- Place DTO headers under `include/core/dto/`.
- Place repository port headers under `include/core/ports/repository/`.
- Place page controller headers under `include/presentation/pages/` and implementations under `src/presentation/pages/`.

## Where to Add New Code

**New Application Use Case:**
- Primary interface: `include/core/i_application.h`
- Implementation: `src/core/application.cc`
- DTOs: `include/core/dto/`
- Tests/fakes: update `test/core/fake_application.h` and `test/core/fake_application.cc` when presentation tests need the use case.

**New Domain Entity:**
- Header: `include/core/domain/<entity>.h`
- Implementation: `src/core/domain/<entity>.cc` when constructors or non-inline behavior are needed.
- Persistence port: `include/core/ports/repository/<entity>_repository.h`
- SQLite implementation: `include/database/sqlite_<entity>_repository.h`, `src/database/sqlite_<entity>_repository.cc`
- Build registration: add new `.cc` files to `src/core/CMakeLists.txt` or `src/database/CMakeLists.txt`.

**New Database Operation:**
- Contract: add or extend the relevant repository port in `include/core/ports/repository/`.
- SQL constants and class declarations: relevant `include/database/sqlite_*_repository.h`.
- Implementation: relevant `src/database/sqlite_*_repository.cc`.
- Transaction orchestration: keep multi-step write coordination in `src/core/application.cc`.
- Schema changes: `schema-model/schema.sql`.

**New UI Page:**
- XRC layout: `src/presentation/xrc/resource.xrc` and `src/presentation/xrc/test_resource.xrc` if tests need it.
- Page header: `include/presentation/pages/<name>_page.h`
- Page implementation: `src/presentation/pages/<name>_page.cc`
- Navigation enum/methods: `include/presentation/navigation.h`
- Shell construction and routing: `include/presentation/presentation.h`, `src/presentation/presentation.cc`
- Build registration: `src/presentation/CMakeLists.txt`

**New UI Control on Existing Page:**
- Add the control to `src/presentation/xrc/resource.xrc`.
- Retrieve it with `XRCCTRL` in the matching controller under `src/presentation/pages/`.
- Bind events in the controller constructor.
- Add or update tests in `test/presentation_test.cc` and `src/presentation/xrc/test_resource.xrc` when the behavior is user-visible.

**Utilities:**
- Core-only utilities: `include/core/` and `src/core/`.
- Database utilities: `include/database/` and `src/database/`.
- Presentation utilities: `include/presentation/` and `src/presentation/`.
- Test-only utilities: `test/core/` or `test/wx/`.

**Build Changes:**
- Add source files to the matching object library in `src/core/CMakeLists.txt`, `src/database/CMakeLists.txt`, or `src/presentation/CMakeLists.txt`.
- Add test files to `test/CMakeLists.txt`.
- Keep the final executable target in top-level `CMakeLists.txt` linked against `application`, `database`, and `presentation`.

## Special Directories

**`build/`:**
- Purpose: Out-of-tree generated CMake build output.
- Generated: Yes
- Committed: No

**`OpenXLSX/`:**
- Purpose: Vendored third-party spreadsheet library dependency.
- Generated: No
- Committed: Yes
- Guidance: Do not edit unless work explicitly targets the vendored library; place DietApp spreadsheet behavior in `src/core/xlsx_service.cc`.

**`.planning/codebase/`:**
- Purpose: Generated GSD codebase map documents used by planning and execution workflows.
- Generated: Yes
- Committed: Project-dependent

**`src/presentation/xrc/`:**
- Purpose: wxWidgets XML UI resources loaded by `Presentation`.
- Generated: No
- Committed: Yes

**`spreadsheet/`:**
- Purpose: Spreadsheet assets copied into `build/share/dietapp/spreadsheet/` for test-enabled builds and loaded by `XlsxService`.
- Generated: No
- Committed: Yes

**`schema-model/`:**
- Purpose: SQLite schema source loaded at app startup.
- Generated: No
- Committed: Yes

---

*Structure analysis: 2026-05-15*
