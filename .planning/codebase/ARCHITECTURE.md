<!-- refreshed: 2026-05-15 -->
# Architecture

**Analysis Date:** 2026-05-15

## System Overview

```text
┌─────────────────────────────────────────────────────────────┐
│                  wxWidgets Desktop Application               │
├──────────────────┬──────────────────┬───────────────────────┤
│  wx bootstrap    │  XRC UI shell    │  Page controllers      │
│ `src/core/`      │ `src/presentation`│ `src/presentation/pages`│
└────────┬─────────┴────────┬─────────┴──────────┬────────────┘
         │                  │                     │
         ▼                  ▼                     ▼
┌─────────────────────────────────────────────────────────────┐
│                    Application Core                          │
│ `include/core/application.h`, `src/core/application.cc`       │
│ `include/core/i_application.h`, `include/core/dto/`           │
└────────────────────────────┬────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│  Persistence, Schema, and Seed Data                           │
│  `include/database/`, `src/database/`, `schema-model/`,       │
│  `spreadsheet/`, vendored `OpenXLSX/`                         │
└─────────────────────────────────────────────────────────────┘
```

## Component Responsibilities

| Component | Responsibility | File |
|-----------|----------------|------|
| `WxBootstrapApp` | Owns wxWidgets process startup and shutdown, constructs `Application`, catches fatal initialization errors, and registers the wx entry point with `wxIMPLEMENT_APP`. | `include/core/wx_bootstrap_app.h`, `src/core/wx_bootstrap_app.cc` |
| `Application` | Coordinates database initialization, spreadsheet import, repository construction, transactions, DTO conversion, and presentation startup. It is the concrete implementation behind the UI-facing `IApplication` port. | `include/core/application.h`, `src/core/application.cc` |
| `IApplication` | Defines the application operations exposed to presentation code: creating users and meals, loading meals, checking users, and loading food details. UI code should call this interface instead of concrete database classes. | `include/core/i_application.h` |
| Domain objects | Hold core entities with IDs, names, comparison operators, and simple accessors. Keep business entity state in `Food`, `Meal`, and `User`. | `include/core/domain/food.h`, `include/core/domain/meal.h`, `include/core/domain/user.h`, `src/core/domain/meal.cc`, `src/core/domain/user.cc` |
| DTOs | Carry data between core and presentation without exposing repository internals or wxSQLite types. | `include/core/dto/food_dto.h`, `include/core/dto/meal_dto.h`, `include/core/dto/user_dto.h` |
| Repository ports | Define persistence operations the core layer expects for users, meals, and foods. Add new persistence capabilities here before implementing concrete storage. | `include/core/ports/repository/user_repository.h`, `include/core/ports/repository/meal_repository.h`, `include/core/ports/repository/food_repository.h` |
| SQLite repositories | Implement repository ports using `DatabaseAdapter`, SQL constants, prepared statements, and in-memory sets for loaded users/meals. | `include/database/sqlite_user_repository.h`, `src/database/sqlite_user_repository.cc`, `include/database/sqlite_meal_repository.h`, `src/database/sqlite_meal_repository.cc`, `include/database/sqlite_food_repository.h`, `src/database/sqlite_food_repository.cc` |
| Database adapter layer | Wraps wxSQLite3 database, statement, and result set APIs behind testable project interfaces. | `include/database/database_adapter.h`, `include/database/database_statement.h`, `include/database/database_result_set.h`, `include/database/sqlite_database_adapter.h`, `src/database/sqlite_database_adapter.cc`, `src/database/sqlite_statement.cc`, `src/database/sqlite_result_set.cc` |
| `Presentation` | Loads XRC resources, creates page controller objects, owns the `wxSimplebook`, and implements `INavigation`. | `include/presentation/presentation.h`, `src/presentation/presentation.cc` |
| Page controllers | Bind wx events to user flows and delegate application actions through `IApplication` and navigation through `INavigation`. | `include/presentation/pages/*.h`, `src/presentation/pages/*.cc` |
| `NutritionalModel` | Implements `wxDataViewModel` for selected foods and total/partial nutrient display in the create page. | `include/presentation/nutritional_model.h`, `src/presentation/nutritional_model.cc` |
| `XlsxService` | Opens `spreadsheet/Taco-4a-Edicao.xlsx` through vendored `OpenXLSX/`, parses worksheet food rows, and creates `Food` plus `FoodDTO` values. | `include/core/xlsx_service.h`, `src/core/xlsx_service.cc` |

## Pattern Overview

**Overall:** Layered desktop application with ports/adapters around persistence and presentation callbacks through core interfaces.

**Key Characteristics:**
- Use `Application` as the composition root for runtime dependencies: database adapter, repositories, presentation, and spreadsheet service are created in `src/core/application.cc`.
- Keep wxWidgets UI behavior in `src/presentation/`; page controllers receive `IApplication*` and `INavigation*` and should not include concrete SQLite repository headers.
- Keep persistence behind repository ports from `include/core/ports/repository/` and adapter interfaces from `include/database/`.
- Use DTOs from `include/core/dto/` at the core/presentation boundary.
- Keep generated and third-party code out of application layers: `build/` is generated output, and `OpenXLSX/` is a vendored dependency.

## Layers

**Bootstrap Layer:**
- Purpose: Start and stop the wxWidgets process.
- Location: `include/core/wx_bootstrap_app.h`, `src/core/wx_bootstrap_app.cc`
- Contains: `wxApp` subclass, fatal startup error handling, `wxIMPLEMENT_APP`.
- Depends on: `Application` through `core/application.h`.
- Used by: The `dietapp` executable target linked in `CMakeLists.txt`.

**Core/Application Layer:**
- Purpose: Coordinate use cases, initialize resources, manage transactions, and translate domain entities to DTOs.
- Location: `include/core/`, `src/core/`
- Contains: `Application`, `IApplication`, domain objects, DTOs, repository ports, `XlsxService`.
- Depends on: repository abstractions, concrete SQLite repositories at composition time, `Presentation`, `OpenXLSX`, wx filename/path APIs.
- Used by: `WxBootstrapApp` and presentation page controllers through `IApplication`.

**Presentation Layer:**
- Purpose: Load XRC UI resources, own page controllers, route simplebook navigation, validate controls, and bind user events.
- Location: `include/presentation/`, `src/presentation/`, `src/presentation/xrc/`
- Contains: `Presentation`, `INavigation`, `InitialPage`, `EnterPage`, `RegisterPage`, `CreatePage`, `NutritionalModel`, XRC resource files.
- Depends on: wxWidgets, DTO headers, `IApplication`.
- Used by: `Application` during startup and by tests through `test/presentation_test.cc`.

**Persistence Layer:**
- Purpose: Provide SQLite-backed implementations of repository ports and abstract wxSQLite3 statement/result-set mechanics.
- Location: `include/database/`, `src/database/`
- Contains: `DatabaseAdapter`, `DatabaseStatement`, `DatabaseResultSet`, SQLite concrete classes, repositories, `DatabaseManager`.
- Depends on: wxSQLite3, core domain classes, repository ports.
- Used by: `Application` through repository pointers and adapter interfaces.

**Resource/Data Layer:**
- Purpose: Provide UI layout, database schema, and spreadsheet seed/reference data.
- Location: `src/presentation/xrc/`, `schema-model/schema.sql`, `spreadsheet/Taco-4a-Edicao.xlsx`
- Contains: XRC frame/panel definitions, SQLite schema, TACO spreadsheet asset.
- Depends on: CMake resource copying/install paths and wx standard data directory lookup.
- Used by: `Presentation::Initialize`, `SQLiteDatabaseAdapter::Initialize`, and `XlsxService::Initialize`.

**Test Support Layer:**
- Purpose: Exercise presentation flows with Catch2, wxWidgets UI simulation, and a fake application core.
- Location: `test/`
- Contains: `test/presentation_test.cc`, `test/core/fake_application.*`, `test/wx/testableframe.*`.
- Depends on: presentation target, wxWidgets, Catch2, copied XRC/spreadsheet resources.
- Used by: `presentation_test` target in `test/CMakeLists.txt`.

## Data Flow

### Application Startup Path

1. wxWidgets invokes `WxBootstrapApp::OnInit` and constructs `Application` (`src/core/wx_bootstrap_app.cc:7`).
2. `Application` creates `DatabaseManager`, `SQLiteDatabaseAdapter`, `Presentation`, SQLite repositories, and `XlsxService` in its constructor (`src/core/application.cc:21`).
3. `Application::Initialize` creates default resource path objects and initializes SQLite, spreadsheet, and presentation resources (`src/core/application.cc:34`).
4. `SQLiteDatabaseAdapter::Initialize` opens `dietapp.db`, enables foreign keys, reads `schema-model/schema.sql`, and executes the schema (`src/database/sqlite_database_adapter.cc:11`).
5. `XlsxService::Initialize` opens `spreadsheet/Taco-4a-Edicao.xlsx` from the wx data directory (`src/core/xlsx_service.cc:40`).
6. `Presentation::Initialize` loads `src/presentation/xrc/resource.xrc`, creates page controllers, and shows the main frame (`src/presentation/presentation.cc:12`).
7. `Application::Initialize` imports worksheet data, loads cached users/meals, fills the food table in a transaction, and populates create-page food choices (`src/core/application.cc:54`).

### New User and Meal Flow

1. `InitialPage::OnButtonRegister` routes to the register page configured for a new user (`src/presentation/pages/initial_page.cc`).
2. `Presentation::NavigateToRegisterPageWithNewUser` calls `RegisterPage::ConfigureForNewUser` and selects the register page (`src/presentation/presentation.cc:64`).
3. `RegisterPage::OnButtonCreate` validates username, display name, and meal controls, then calls the configured action (`src/presentation/pages/register_page.cc:48`).
4. `RegisterPage::OnButtonCreateWithNewUser` builds `CreateUserWithMealDTO` and calls `IApplication::CreateUserWithMeal` (`src/presentation/pages/register_page.cc:113`).
5. `Application::CreateUserWithMeal` checks cached users, begins a transaction, adds `User`, adds `Meal`, commits, and returns success (`src/core/application.cc:89`).
6. The register page loads meals through `IApplication::LoadMealsFromUser` and navigates to the create page with `MealsFromUserDTO` (`src/presentation/pages/register_page.cc:66`).

### Existing User Flow

1. `InitialPage::OnButtonEnter` routes to `PageId::Enter` (`src/presentation/pages/initial_page.cc`).
2. `EnterPage::OnButtonFollowOn` validates username input and calls `IApplication::DoesUserExist` (`src/presentation/pages/enter_page.cc:31`).
3. `EnterPage` loads `UserDTO` through `IApplication::GetUserData` and asks navigation to configure the register page for an existing user (`src/presentation/pages/enter_page.cc:45`).
4. `RegisterPage::ConfigureForExistingUser` switches the create button action to `OnButtonCreateWithExistingUser` (`src/presentation/pages/register_page.cc:106`).
5. `RegisterPage::OnButtonCreateWithExistingUser` builds `AddMealToUserDTO` and calls `IApplication::AddMealToUser` (`src/presentation/pages/register_page.cc:125`).
6. `Application::AddMealToUser` looks up the user from the repository cache, inserts a meal in a transaction, and commits (`src/core/application.cc:115`).

### Food Selection Flow

1. `Application::Initialize` calls `presentation_->FillFoodChoices(data)` after spreadsheet import (`src/core/application.cc:69`).
2. `Presentation::FillFoodChoices` delegates choices to `CreatePage` (`src/presentation/presentation.cc:69`).
3. `CreatePage::OnItemSelected` reads the selected combo-box text and calls `IApplication::GetFoodData` (`src/presentation/pages/create_page.cc:78`).
4. `Application::GetFoodData` queries `FoodRepository::GetFood` and returns a `FoodDTO` (`src/core/application.cc:163`).
5. `SQLiteFoodRepository::GetFood` prepares `kSelectFoodSql`, binds `:display_name`, reads nutrient columns, and returns a `Food` (`src/database/sqlite_food_repository.cc:16`).
6. `CreatePage` adds the DTO to `NutritionalModel` and refreshes the data view (`src/presentation/pages/create_page.cc:81`).

**State Management:**
- Runtime dependency state lives in `Application` as `std::unique_ptr` and `std::shared_ptr` members (`include/core/application.h`).
- SQLite connection state lives in `SQLiteDatabaseAdapter::database_` (`include/database/sqlite_database_adapter.h`).
- Loaded users and meals are cached in `std::set<User>` and `std::set<Meal>` inside SQLite repositories (`include/database/sqlite_user_repository.h`, `include/database/sqlite_meal_repository.h`).
- UI navigation state is the selected page of `wxSimplebook` in `Presentation` (`src/presentation/presentation.cc`).
- Selected food state lives in `NutritionalModel::selected_foods_` (`include/presentation/nutritional_model.h`).

## Key Abstractions

**Application Port (`IApplication`):**
- Purpose: Decouple page controllers from the concrete `Application` and persistence details.
- Examples: `include/core/i_application.h`, `test/core/fake_application.h`
- Pattern: Interface port implemented by production core and test fake.

**Navigation Port (`INavigation`):**
- Purpose: Let individual pages request page transitions without owning the `wxSimplebook`.
- Examples: `include/presentation/navigation.h`, `include/presentation/presentation.h`, `src/presentation/presentation.cc`
- Pattern: Presentation-level interface implemented by the shell.

**Repository Ports:**
- Purpose: Define domain persistence contracts for users, meals, and foods.
- Examples: `include/core/ports/repository/user_repository.h`, `include/core/ports/repository/meal_repository.h`, `include/core/ports/repository/food_repository.h`
- Pattern: Port interfaces implemented by SQLite adapters in `include/database/` and `src/database/`.

**Database Adapter Interfaces:**
- Purpose: Hide wxSQLite3 connection, prepared-statement, and result-set APIs from repository code.
- Examples: `include/database/database_adapter.h`, `include/database/database_statement.h`, `include/database/database_result_set.h`
- Pattern: Adapter wrapper around a third-party database library.

**DTO Boundary Objects:**
- Purpose: Carry presentation-facing values without exposing domain object mutability or storage classes.
- Examples: `include/core/dto/food_dto.h`, `include/core/dto/meal_dto.h`, `include/core/dto/user_dto.h`
- Pattern: Plain structs passed through `IApplication` and `INavigation`.

**XRC Resource Shell:**
- Purpose: Keep main frame and panel layouts in declarative XRC while controllers bind behavior in C++.
- Examples: `src/presentation/xrc/resource.xrc`, `src/presentation/xrc/test_resource.xrc`, `src/presentation/presentation.cc`
- Pattern: Resource-backed wxWidgets UI with controller classes per page.

## Entry Points

**Desktop Application:**
- Location: `src/core/wx_bootstrap_app.cc`
- Triggers: wxWidgets process startup through `wxIMPLEMENT_APP(WxBootstrapApp)`.
- Responsibilities: Create `Application`, call `Initialize`, handle fatal startup failures, call `Shutdown` on exit.

**Application Composition Root:**
- Location: `src/core/application.cc`
- Triggers: `WxBootstrapApp::OnInit`.
- Responsibilities: Construct adapters/repositories/presentation, initialize resources, seed food data, and expose user workflows to UI.

**Presentation Shell:**
- Location: `src/presentation/presentation.cc`
- Triggers: `Application::Initialize` and presentation tests.
- Responsibilities: Load XRC, build page controllers, own navigation, and populate food choices.

**Test Executable:**
- Location: `test/presentation_test.cc`, `test/CMakeLists.txt`
- Triggers: `./build/test/presentation_test`.
- Responsibilities: Load `test_resource.xrc`, use `FakeApplication`, create `Presentation`, and simulate UI behavior.

## Architectural Constraints

- **Threading:** The app uses the wxWidgets GUI event loop. No worker threads are detected in `src/`, `include/`, or `test/`; keep UI mutations on the wx event thread.
- **Global state:** wxWidgets global application/resource state is used through `wxIMPLEMENT_APP`, `wxStandardPaths::Get`, and `wxXmlResource::Get` in `src/core/wx_bootstrap_app.cc`, `src/core/xlsx_service.cc`, `src/database/sqlite_database_adapter.cc`, and `src/presentation/presentation.cc`.
- **Circular imports:** No direct source-level circular include chains are detected, but `Application` composes `Presentation` while `Presentation` stores `IApplication*`. Preserve the interface boundary at `include/core/i_application.h` to avoid concrete `Application` dependencies in page classes.
- **Resource paths:** Production code derives default XRC, spreadsheet, and database paths from `wxStandardPaths::Get().GetDataDir()`. Tests pass explicit XRC/top-window resources through `Presentation::Initialize`.
- **Persistence cache:** User and meal repositories keep in-memory sets loaded at startup. Writes must update both SQLite and the in-memory set to stay visible in the same session.
- **Vendored dependency:** Treat `OpenXLSX/` as third-party code. Application code should integrate through `XlsxService` in `include/core/xlsx_service.h` and `src/core/xlsx_service.cc`.

## Anti-Patterns

### UI Code Talking Directly to SQLite

**What happens:** Page controllers include `core/i_application.h` and should use `IApplication`; direct includes of `database/sqlite_*` from `src/presentation/` bypass the intended boundary.
**Why it's wrong:** It couples wx event handlers to storage details and makes `test/core/fake_application.h` unusable for presentation tests.
**Do this instead:** Add a method to `IApplication` in `include/core/i_application.h`, implement it in `src/core/application.cc`, and call it from page code such as `src/presentation/pages/register_page.cc`.

### Adding Business Flow Inside XRC Resources

**What happens:** XRC files define controls and layout only; behavior is bound in page controller constructors with `Bind` and `XRCCTRL`.
**Why it's wrong:** Encoding behavior outside controller classes hides application flow and is not testable by the existing C++ presentation tests.
**Do this instead:** Add controls to `src/presentation/xrc/resource.xrc` and bind behavior in the matching controller under `src/presentation/pages/`.

### Mutating Repository State Without Transactions

**What happens:** Application use cases wrap user/meal/food write operations in `BeginTransaction`, `CommitTransaction`, and `RollbackTransaction`.
**Why it's wrong:** Multi-step user and meal changes can leave SQLite and repository caches inconsistent if one operation fails.
**Do this instead:** Keep transaction orchestration in `src/core/application.cc` around multi-repository writes, following `Application::CreateUserWithMeal` and `Application::AddMealToUser`.

### Editing Vendored OpenXLSX for App Behavior

**What happens:** Spreadsheet parsing behavior belongs in `XlsxService`; `OpenXLSX/` is linked as a vendored dependency by CMake.
**Why it's wrong:** Changing vendored library internals increases merge and upgrade risk for application-specific behavior.
**Do this instead:** Add DietApp-specific parsing, defaults, and DTO construction in `src/core/xlsx_service.cc`.

## Error Handling

**Strategy:** Startup failures become `std::runtime_error` from `Application::Initialize` and fatal wx dialogs from `WxBootstrapApp::OnInit`; runtime user-flow failures return `bool` or empty DTO/vector values and may display page-level error dialogs.

**Patterns:**
- Throw `std::runtime_error` with constants from `include/core/core_error_messages.h` when database, spreadsheet, or presentation startup fails in `src/core/application.cc`.
- Catch `wxSQLite3Exception`, roll back transactions, and log to `std::cerr` in core use cases (`src/core/application.cc`).
- Suppress modal error boxes in tests by checking `IApplication::IsTestsMode()` in page controllers (`src/presentation/pages/register_page.cc`, `src/presentation/pages/enter_page.cc`).
- Return `false` from initialization methods when required UI controls or resources are missing (`src/presentation/presentation.cc`).

## Cross-Cutting Concerns

**Logging:** Use `wxLogFatalError` for startup failure in `src/core/wx_bootstrap_app.cc` and `std::cerr` for database/spreadsheet exceptions in `src/core/application.cc`, `src/core/xlsx_service.cc`, and `src/database/sqlite_food_repository.cc`.

**Validation:** UI text validation uses `wxTextValidator` in `src/presentation/pages/register_page.cc` and `src/presentation/pages/enter_page.cc`. Database constraints live in `schema-model/schema.sql`.

**Authentication:** No password authentication is implemented. User lookup is username-based through `IApplication::DoesUserExist`, `IApplication::GetUserData`, and `SQLiteUserRepository::Exists`.

---

*Architecture analysis: 2026-05-15*
