# Coding Conventions

**Analysis Date:** 2026-05-15

## Naming Patterns

**Files:**
- Use `.cc` for C++ implementation files and `.h` for public headers: `src/core/application.cc`, `include/core/application.h`, `src/presentation/pages/register_page.cc`.
- Keep headers under `include/` and implementations under matching layer directories in `src/`: `include/database/sqlite_user_repository.h` pairs with `src/database/sqlite_user_repository.cc`.
- Use lowercase snake case for file names: `sqlite_user_repository.cc`, `nutritional_model.cc`, `fake_application.cc`.
- Do not edit `OpenXLSX/` unless work explicitly targets the vendored OpenXLSX dependency.

**Functions:**
- Use `PascalCase` for public methods and overridden interface methods: `Application::Initialize()` in `src/core/application.cc`, `RegisterPage::OnButtonCreate()` in `src/presentation/pages/register_page.cc`.
- Use clear verb-based names for behavior: `CreateUserWithMeal`, `LoadMealsFromUser`, `NavigateToCreatePageWithMeals`, `FillFoodChoices`.
- Keep simple domain accessors lowercase with trailing parentheses: `User::username()` and `User::id()` in `include/core/domain/user.h`.
- Local lambdas may use camelCase when scoped to a function: `parseValue` in `src/core/xlsx_service.cc`.

**Variables:**
- Use lowercase snake case for local variables: `worksheet_name`, `selected_index`, `display_name_validator`.
- Use trailing underscore for private members: `db_manager_` in `include/core/application.h`, `register_page_` in `include/presentation/pages/register_page.h`, `selected_foods_` in `include/presentation/nutritional_model.h`.
- Use `k`-prefixed constants for labels, SQL, and error messages: `kCreateUserLabel` in `include/presentation/pages/register_page.h`, `kInsertUserSql` in `include/database/sqlite_user_repository.h`.
- Prefer `const auto` or `const std::string` for immutable local values: `const std::string worksheet_name` in `src/core/application.cc`.

**Types:**
- Use `PascalCase` for classes and structs: `Application`, `DatabaseManager`, `SQLiteUserRepository`, `MealDTO`, `FoodDTO`.
- Use interface names prefixed with `I` only for application-facing abstractions already following that pattern: `IApplication` in `include/core/i_application.h`, `INavigation` in `include/presentation/navigation.h`.
- DTO structs live in `include/core/dto/` and use public fields without accessors: `include/core/dto/meal_dto.h`, `include/core/dto/food_dto.h`.

## Code Style

**Formatting:**
- Use clang-format with `.clang-format`.
- Style is Google-based C++ with 2-space indentation, 80-column limit, no tabs, same-line opening braces, and left-aligned pointers.
- Keep control statements braced and avoid single-line `if` bodies; `.clang-format` sets `AllowShortIfStatementsOnASingleLine: Never`.
- Preserve include blocks separated by blank lines: project header where present, standard library, third-party, then project headers. Examples: `src/core/application.cc`, `src/presentation/pages/register_page.cc`, `src/presentation/nutritional_model.cc`.

**Linting:**
- No `.clang-tidy`, Cppcheck, or CI lint configuration is detected.
- Use `.clang-format` as the authoritative style tool.
- CMake enforces C++17 via `CMakeLists.txt`.

## Import Organization

**Order:**
1. Matching project header first for implementation files when present: `#include "core/application.h"` in `src/core/application.cc`.
2. Standard library headers: `<memory>`, `<string>`, `<vector>`, `<algorithm>`.
3. Third-party and framework headers: `<wx/...>`, `<OpenXLSX.hpp>`, `<catch2/...>`.
4. Project headers with full include-root paths: `"core/dto/user_dto.h"`, `"database/database_adapter.h"`, `"presentation/navigation.h"`.

**Path Aliases:**
- CMake exposes `${CMAKE_SOURCE_DIR}/include`, so production code should include project headers from the include root: `"core/i_application.h"`, `"presentation/presentation.h"`.
- Test targets expose `${CMAKE_SOURCE_DIR}/test`, so test helpers may include `"core/fake_application.h"` and `"wx/testableframe.h"` from `test/`.
- Some headers include nearby database headers without the full prefix, such as `"database_adapter.h"` in `include/database/sqlite_user_repository.h`; prefer matching the existing local pattern inside `include/database/`.

## Error Handling

**Patterns:**
- Startup failures throw `std::runtime_error` from `Application::Initialize()` in `src/core/application.cc`.
- Database transactions use `BeginTransaction()`, `CommitTransaction()`, and rollback in `catch (const wxSQLite3Exception& e)` blocks: `Application::CreateUserWithMeal()` and `Application::AddMealToUser()` in `src/core/application.cc`.
- Recoverable application operations return `bool` or empty vectors and log exception messages to `std::cerr`: `Application::LoadMealsFromUser()` in `src/core/application.cc`, `XlsxService::Initialize()` in `src/core/xlsx_service.cc`.
- Presentation validation uses early returns and suppresses modal message boxes in test mode through `app_->IsTestsMode()`: `RegisterPage::ShowError()` in `src/presentation/pages/register_page.cc`, `EnterPage::ShowError()` in `src/presentation/pages/enter_page.cc`.
- UI resource lookups use `wxASSERT` after `XRCCTRL` calls: `src/presentation/pages/register_page.cc`, `src/presentation/pages/create_page.cc`, `src/presentation/pages/initial_page.cc`.

## Logging

**Framework:** console

**Patterns:**
- Use `std::cerr << e.GetMessage() << std::endl;` for caught `wxSQLite3Exception` values in application and repository code: `src/core/application.cc`, `src/database/sqlite_food_repository.cc`.
- Use `std::cerr << e.what() << std::endl;` for `OpenXLSX::XLException` and standard exceptions in `src/core/xlsx_service.cc`.
- User-facing errors in presentation code use `wxMessageBox` only outside test mode: `src/presentation/pages/register_page.cc`, `src/presentation/pages/enter_page.cc`.

## Comments

**When to Comment:**
- Keep comments sparse and only explain non-obvious operational constraints, such as the GTK `wxUIActionSimulator` yielding requirement in `test/presentation_test.cc`.
- Use short inline comments for spreadsheet column meanings in `src/core/xlsx_service.cc`.
- Avoid comments that restate simple code flow.

**JSDoc/TSDoc:**
- Not applicable. This is a C++ codebase and no Doxygen-style API documentation pattern is detected.

## Function Design

**Size:** Keep functions focused by layer responsibility. UI event handlers may orchestrate validation and navigation, as in `RegisterPage::OnButtonCreate()` in `src/presentation/pages/register_page.cc`; shared calculations should be private helpers, as in `NutritionalModel::CalculateTotalKcal()` in `src/presentation/nutritional_model.cc`.

**Parameters:** Pass strings and DTOs by `const&` when they are read-only: `LoadMealsFromUser(const std::string& username)` in `include/core/i_application.h`, `LoadExistingUser(const UserDTO& user_data)` in `include/presentation/pages/register_page.h`. Use raw wxWidgets pointers for UI controls and parent panels following existing wx ownership patterns.

**Return Values:** Use `bool` for success/failure operations, DTO values for data transfer, and vectors for collections. Return empty collections on recoverable load failure as in `Application::LoadMealsFromUser()` in `src/core/application.cc`.

## Module Design

**Exports:** Public headers under `include/` define the module surface. Implementations under `src/` should include their matching public header and keep layer responsibilities separated: orchestration in `src/core/`, persistence in `src/database/`, UI behavior in `src/presentation/`.

**Barrel Files:** No barrel header pattern is used. Include concrete headers directly, such as `"presentation/pages/register_page.h"` or `"database/sqlite_user_repository.h"`.

---

*Convention analysis: 2026-05-15*
