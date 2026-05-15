# Codebase Concerns

**Analysis Date:** 2026-05-15

## Tech Debt

**Incomplete application and repository contracts:**
- Issue: Several public methods required by interfaces or exposed by `Application` are stubs that return fixed values instead of real behavior.
- Files: `src/core/application.cc:76`, `src/core/application.cc:80`, `src/core/application.cc:84`, `src/database/sqlite_user_repository.cc:35`, `src/database/sqlite_user_repository.cc:39`, `src/database/sqlite_user_repository.cc:43`, `src/database/sqlite_food_repository.cc:37`, `src/database/sqlite_food_repository.cc:41`, `src/database/sqlite_food_repository.cc:45`, `src/database/sqlite_food_repository.cc:49`, `src/database/sqlite_meal_repository.cc:45`, `src/database/sqlite_meal_repository.cc:49`
- Impact: Future callers can receive false negatives, null pointers, or success responses for work that did not happen. Delete/update/authentication features are unsafe to build on top of the current repository contracts.
- Fix approach: Either implement each method against SQLite and keep the in-memory caches synchronized, or remove unused interface methods until the features exist. Add repository-level tests for every method in `include/core/ports/repository/*.h`.

**Food table population ignores insert failure status:**
- Issue: `SQLiteFoodRepository::FillFoodsTable` initializes `global_success` to `false` and never sets it to `true` after successful inserts.
- Files: `src/database/sqlite_food_repository.cc:53`
- Impact: The method reports failure even when all rows insert successfully. `Application::Initialize` currently ignores the return value, so startup can silently proceed with partial or misleading food-load state.
- Fix approach: Track attempted row count and failed row count, return `true` only when all inserts succeed, and make `Application::Initialize` fail or surface an actionable error when food seeding fails.

**Startup orchestration mixes infrastructure setup, spreadsheet parsing, repository cache loading, and UI creation:**
- Issue: `Application::Initialize` initializes SQLite, opens the workbook, initializes presentation, loads spreadsheet rows, loads repository caches, seeds foods, and fills UI choices in one method.
- Files: `src/core/application.cc:34`
- Impact: Startup failures are hard to isolate, unit testing requires broad fakes, and UI initialization is coupled to database/spreadsheet availability.
- Fix approach: Split startup into explicit services: database/schema initialization, food data import, repository cache hydration, and presentation bootstrap. Keep `Application::Initialize` as orchestration only.

**Hard-coded resource names and worksheet layout:**
- Issue: The workbook filename, worksheet name, spreadsheet columns, and application database filename are hard-coded.
- Files: `src/core/application.cc:40`, `src/core/xlsx_service.cc:43`, `src/core/xlsx_service.cc:67`, `src/database/sqlite_database_adapter.cc:12`
- Impact: Spreadsheet updates or localized workbook changes can break startup at runtime. Tests cannot easily exercise alternate sheets or missing-column scenarios.
- Fix approach: Move workbook path, worksheet name, and column mappings behind configuration or constructor parameters. Add validation that fails with a domain error when required columns or sheets are missing.

## Known Bugs

**Food display DTO overwrites the canonical food name:**
- Symptoms: `FoodDTO::name` is assigned from the spreadsheet raw name and then immediately overwritten with the generated display name.
- Files: `src/core/xlsx_service.cc:105`
- Trigger: Loading any food row through `XlsxService::LoadFoodDataFromSheet`.
- Workaround: Current UI uses `FoodDTO::name` as the combo-box display label, so the bug is masked for display use cases.
- Fix approach: Assign `food_data.display_name = display_name` and preserve `food_data.name = name`. Add a unit test for `XlsxService::LoadFoodDataFromSheet` DTO mapping.

**Unchecked repository lookups can dereference `end()`:**
- Symptoms: Missing users or meals can cause undefined behavior because `GetUser` and `GetMeal` dereference search iterators without checking for a match.
- Files: `src/database/sqlite_user_repository.cc:17`, `src/database/sqlite_meal_repository.cc:14`, `src/core/application.cc:120`, `src/core/application.cc:137`, `src/core/application.cc:157`
- Trigger: Calling `Application::AddMealToUser`, `Application::LoadMealsFromUser`, or `Application::GetUserData` with a username not present in the in-memory repository cache.
- Workaround: UI entry flow checks `DoesUserExist` before `GetUserData`, but other application paths have no guard.
- Fix approach: Return `std::optional<User>` / `std::optional<Meal>` or throw a domain exception, then handle the missing case in `Application`.

**Create page assumes non-empty meal and food-choice data:**
- Symptoms: `CreatePage::SetMealsFromUser` indexes `data.meals[data.selected_meal]` and `CreatePage::FillFoodChoices` selects index `0` without checking list size.
- Files: `src/presentation/pages/create_page.cc:60`, `src/presentation/pages/create_page.cc:69`
- Trigger: User creation succeeds but `LoadMealsFromUser` returns an empty vector, or spreadsheet loading returns no food choices.
- Workaround: Current happy-path UI tests seed one meal through `FakeApplication`.
- Fix approach: Guard empty collections, show a user-visible error, and add UI tests for empty meal and empty food-data cases.

**Register page can navigate with an invalid selected meal index:**
- Symptoms: `FindCurrentMealPos` can return `meals.end()` and `std::distance` then produces a value equal to `meals.size()`.
- Files: `src/presentation/pages/register_page.cc:66`, `include/presentation/pages/register_page.h:57`
- Trigger: `current_action_` succeeds but `LoadMealsFromUser` does not include the just-created or just-added meal name.
- Workaround: None detected.
- Fix approach: Check `it != meals.end()` before navigation and keep the user on the register page with an error when the created meal cannot be reloaded.

**Schema file loading is relative to the process working directory:**
- Symptoms: The SQLite adapter reads `../schema-model/schema.sql`, which depends on where the executable is launched.
- Files: `src/database/sqlite_database_adapter.cc:27`
- Trigger: Running `dietapp` from an installed path, an IDE, or a working directory that does not make `../schema-model/schema.sql` resolve to the repo schema.
- Workaround: Running from the expected build directory layout can make the relative path work.
- Fix approach: Install the schema as a data resource or embed schema SQL in a compiled resource/module. Check `wxFile::IsOpened` and fail initialization if schema loading fails.

## Security Considerations

**No authentication or password model despite credential APIs:**
- Risk: The code exposes credential validation methods but no password storage, hashing, or authentication flow is implemented.
- Files: `src/core/application.cc:84`, `src/database/sqlite_user_repository.cc:43`, `schema-model/schema.sql:1`, `include/core/ports/repository/user_repository.h:24`
- Current mitigation: Usernames are unique in `schema-model/schema.sql:3`; there is no actual credential check.
- Recommendations: Do not use `ValidateUserCredentials` until the schema includes a password hash column and repository validation uses a proven password-hashing library. Hide or remove the credential API until implemented.

**Unbounded local SQLite data growth and no deletion path:**
- Risk: Users, meals, and foods can be inserted but delete/update methods are stubs or missing from UI flows.
- Files: `src/database/sqlite_user_repository.cc:35`, `src/database/sqlite_meal_repository.cc:45`, `src/database/sqlite_food_repository.cc:41`, `schema-model/schema.sql:1`
- Current mitigation: Local SQLite database only; no network exposure detected.
- Recommendations: Implement deletion/update with foreign-key behavior and explicit cascade decisions before adding features that let users manage stored meals or personal records.

**Vendored dependency should remain isolated:**
- Risk: `OpenXLSX/` is vendored and linked directly from the root build.
- Files: `CMakeLists.txt:63`, `OpenXLSX/`
- Current mitigation: Project guidance treats `OpenXLSX/` as a vendored dependency.
- Recommendations: Do not edit `OpenXLSX/` for application fixes. Track upstream version and apply dependency updates as explicit maintenance work.

## Performance Bottlenecks

**Food spreadsheet import runs during every application startup:**
- Problem: Startup opens the XLSX workbook, parses every row, and attempts to seed the foods table before showing a usable app state.
- Files: `src/core/application.cc:46`, `src/core/application.cc:54`, `src/core/xlsx_service.cc:67`, `src/database/sqlite_food_repository.cc:53`
- Cause: Food data import is synchronous and tied to `Application::Initialize`.
- Improvement path: Cache an import version in SQLite, skip workbook parsing when the shipped dataset version is already loaded, and move expensive import work behind a progress/error state.

**Repository caches load all users and meals into sets:**
- Problem: `LoadAllUsers` and `LoadAllMeals` load every row into memory and later query by linear search over `std::set`.
- Files: `src/database/sqlite_user_repository.cc:47`, `src/database/sqlite_meal_repository.cc:53`, `include/database/sqlite_user_repository.h:39`, `include/database/sqlite_meal_repository.h:45`
- Cause: Repositories maintain process-local caches instead of querying SQLite for lookup operations.
- Improvement path: Use SQL queries for `Exists`, `GetUser`, and `GetMealsFromUser`, or maintain maps keyed by username and user id with clear cache invalidation rules.

**Food seeding prepares an insert statement per row:**
- Problem: `FillFoodsTable` prepares `kInsertFoodSql` inside the loop for every food row.
- Files: `src/database/sqlite_food_repository.cc:61`
- Cause: Statement preparation is repeated rather than prepared once per import transaction.
- Improvement path: Prepare the insert statement once before the loop, bind/execute/reset per row if supported by the statement wrapper, and keep the existing transaction boundary in `Application::Initialize`.

## Fragile Areas

**wxDataView item IDs are treated as vector indexes:**
- Files: `src/presentation/nutritional_model.cc:40`, `src/presentation/nutritional_model.cc:103`, `src/presentation/nutritional_model.cc:114`, `src/presentation/pages/create_page.cc:96`, `src/presentation/pages/create_page.cc:110`
- Why fragile: DataView item IDs are manually encoded as `reinterpret_cast<void*>(i)` and converted back to row indexes. The total row and stale selections can produce out-of-range access through `GetFoodByRow`.
- Safe modification: Add bounds checks to `GetFoodByRow`, avoid allowing delete/context-menu actions on the summary row, and prefer a stable row object or model-owned item token over integer pointer casts.
- Test coverage: No direct tests cover `NutritionalModel`; `test/presentation_test.cc` does not exercise food selection deletion or summary-row behavior.

**XRC control lookup relies on debug-only assertions:**
- Files: `src/presentation/pages/register_page.cc:18`, `src/presentation/pages/create_page.cc:17`, `src/presentation/pages/enter_page.cc:11`
- Why fragile: `wxASSERT` can be compiled out, leaving later null dereferences when an XRC control name changes or a resource fails to load.
- Safe modification: Replace critical control assertions with runtime checks that fail page initialization cleanly and include the missing XRC control name.
- Test coverage: `test/presentation_test.cc:38` loads `test_resource.xrc`, but there are no tests for missing or renamed controls.

**Schema migration is one-shot `CREATE TABLE IF NOT EXISTS`:**
- Files: `schema-model/schema.sql:1`, `src/database/sqlite_database_adapter.cc:27`
- Why fragile: Adding columns, indexes, constraints, or data migrations cannot be handled safely by replaying the current schema file against an existing database.
- Safe modification: Add a `schema_version` table or `PRAGMA user_version` migration path and make every schema change an ordered migration.
- Test coverage: No database integration tests exercise schema creation or upgrades.

**Presentation UI flow tests are broad and stateful:**
- Files: `test/presentation_test.cc:37`
- Why fragile: One large nested `TEST_CASE` uses `wxUIActionSimulator`, fixed screen positions, repeated `wxYield`, and manual fake resets. Failures can be hard to localize and may vary by GTK/display environment.
- Safe modification: Split scenarios into smaller tests, factor repeated text-entry helpers, and add model/page-level tests for non-visual logic where possible.
- Test coverage: UI navigation and validation happy/error paths are covered, but core, database, spreadsheet, and nutritional model behavior are not covered.

## Scaling Limits

**Single local SQLite database file:**
- Current capacity: Local desktop database at the wx standard data directory with one process-local adapter.
- Limit: No concurrent multi-user access model, no repository invalidation across processes, and no migration strategy for existing user data.
- Scaling path: Keep SQLite for desktop scope but add migrations, repository queries, and explicit transaction/error semantics before expanding data volume or multi-window/process behavior.

**Meal-food persistence not connected to the UI model:**
- Current capacity: The schema contains `meal_foods`, but selected foods live only in `NutritionalModel`.
- Limit: Food selections, quantities, and calories cannot persist across sessions.
- Scaling path: Add a meal-food repository/service around `schema-model/schema.sql:22`, then make `CreatePage` persist selections through `IApplication`.

## Dependencies at Risk

**System package and Ubuntu-version coupling:**
- Risk: CI pins wxWidgets package versions for `ubuntu-24.04`, while root CMake shells out to `wx-config` and writes a pkg-config file under `$HOME`.
- Impact: Local builds and CI can fail when package names or exact versions shift, and configure mutates developer home directories.
- Migration plan: Prefer standard `find_package(wxWidgets)` or a documented toolchain file. Keep CI package versions in one place and avoid writing generated pkg-config files outside the build tree.

**Catch2 is required even when tests are disabled:**
- Risk: `pkg_check_modules(CATCH2 REQUIRED catch2-with-main)` runs unconditionally before the `BUILD_TESTING` branch.
- Impact: Production-only builds still require Catch2 installation.
- Migration plan: Move Catch2 discovery into the `if(BUILD_TESTING)` block in `CMakeLists.txt:69` and keep production configure dependencies limited to runtime/build requirements.

## Missing Critical Features

**No persistence path for selected foods in a meal:**
- Problem: Users can select foods in the create page, but there is no application/repository method to save `meal_foods` rows or quantities.
- Blocks: A complete diet tracking workflow where meals retain foods and nutrition data after closing the app.

**No quantity input for selected foods:**
- Problem: Nutritional totals use per-100g macro values directly and do not account for `quantity_grams`.
- Blocks: Accurate calorie/macronutrient totals for real meal portions.

**No schema migration/versioning:**
- Problem: The app can create initial tables but cannot upgrade existing databases safely.
- Blocks: Adding password hashes, meal-food persistence changes, indexes, or constraints without manual database resets.

## Test Coverage Gaps

**Core application workflows:**
- What's not tested: `Application::Initialize`, transaction rollback paths, duplicate-user handling, missing-user behavior, and food lookup mapping.
- Files: `src/core/application.cc`
- Risk: Startup and transaction regressions can pass CI because only presentation tests run against a fake application.
- Priority: High

**SQLite repositories and schema creation:**
- What's not tested: User/meal/food inserts, cache loading, lookup miss behavior, foreign-key enforcement, and schema file loading.
- Files: `src/database/sqlite_user_repository.cc`, `src/database/sqlite_meal_repository.cc`, `src/database/sqlite_food_repository.cc`, `src/database/sqlite_database_adapter.cc`, `schema-model/schema.sql`
- Risk: Data loss, silent failed writes, and undefined behavior can ship unnoticed.
- Priority: High

**Spreadsheet parsing:**
- What's not tested: Worksheet existence, column mapping, non-numeric nutrient cells, display-name generation, empty workbook data, and missing workbook errors.
- Files: `src/core/xlsx_service.cc`, `spreadsheet/Taco-4a-Edicao.xlsx`
- Risk: Dataset updates can break startup or corrupt displayed food names.
- Priority: Medium

**Nutritional model calculations and row operations:**
- What's not tested: Adding foods, deleting duplicate food IDs, summary-row protection, partial totals, and out-of-range row access.
- Files: `src/presentation/nutritional_model.cc`, `src/presentation/pages/create_page.cc`
- Risk: UI food selection can crash or display incorrect totals.
- Priority: Medium

**Build variants:**
- What's not tested: `BUILD_TESTING=OFF`, installed executable resource lookup, and launching outside the build directory.
- Files: `CMakeLists.txt`, `src/database/sqlite_database_adapter.cc`, `src/core/xlsx_service.cc`
- Risk: CI can pass while release or installed builds fail at configure time or startup.
- Priority: Medium

---

*Concerns audit: 2026-05-15*
