# Testing Patterns

**Analysis Date:** 2026-05-15

## Test Framework

**Runner:**
- Catch2, provided by `pkg_check_modules(CATCH2 REQUIRED catch2-with-main)` in `CMakeLists.txt`.
- Config: `test/CMakeLists.txt`.
- The test binary is `presentation_test`, built from `test/presentation_test.cc`, `test/wx/testableframe.cc`, and `test/core/fake_application.cc`.

**Assertion Library:**
- Catch2 macros from `<catch2/catch_test_macros.hpp>`.
- Use `REQUIRE` for setup and state that must hold before continuing, and `CHECK` for assertions where the section can continue: `test/presentation_test.cc`, `test/wx/testableframe.cc`.

**Run Commands:**
```bash
cmake -S . -B build -DBUILD_TESTING=ON              # Configure tests
cmake --build build --target presentation_test      # Build test binary
./build/test/presentation_test                      # Run all tests
./build/test/presentation_test "[UI Flow]"          # Run the UI flow test tag
```

## Test File Organization

**Location:**
- Tests live under `test/`.
- Main UI flow tests are in `test/presentation_test.cc`.
- Test doubles live under `test/core/`: `test/core/fake_application.h`, `test/core/fake_application.cc`.
- wxWidgets UI simulation helpers live under `test/wx/`: `test/wx/testableframe.h`, `test/wx/testableframe.cc`.

**Naming:**
- Use `_test.cc` for test executables: `test/presentation_test.cc`.
- Use descriptive helper names matching their role: `FakeApplication`, `wxTestableFrame`, `EventCounter`.
- Use descriptive `TEST_CASE` and `SECTION` names that state user-visible behavior, such as `"InitialPageRegisterButtonSucceed"` and `"RegisterPageCreateButtonNoUsername"` in `test/presentation_test.cc`.

**Structure:**
```text
test/
├── CMakeLists.txt
├── presentation_test.cc
├── core/
│   ├── fake_application.h
│   └── fake_application.cc
└── wx/
    ├── testableframe.h
    └── testableframe.cc
```

## Test Structure

**Suite Organization:**
```cpp
#define CATCH_CONFIG_RUNNER

class PresentationTest : public wxApp {
 public:
  bool OnInit() override { return true; }
};

wxIMPLEMENT_APP_NO_MAIN(PresentationTest);

int main(int argc, char** argv) {
  wxEntryStart(argc, argv);
  wxTheApp->CallOnInit();
  int result = Catch::Session().run(argc, argv);
  wxTheApp->OnExit();
  wxEntryCleanup();
  return result;
}

TEST_CASE("Presentation", "[UI Flow]") {
  // Load XRC, construct Presentation, then exercise user-visible sections.
}
```

**Patterns:**
- Initialize wxWidgets manually before running Catch2: `test/presentation_test.cc`.
- Load XRC resources from the built test resource directory before creating the frame: `test/presentation_test.cc`.
- Use a `FakeApplication` through the `IApplication` interface to isolate presentation behavior from real database and spreadsheet services: `test/core/fake_application.h`.
- Use nested `SECTION` blocks for page-specific UI scenarios inside the `"Presentation"` test case: `test/presentation_test.cc`.
- Use `wxUIActionSimulator`, `wxYield()`, and control focus to simulate real user input: `test/presentation_test.cc`.
- Wait for text control values in loops after `sim.Text(...)` so GTK event processing catches up: `test/presentation_test.cc`.

## Mocking

**Framework:** Manual fakes; no mocking library is detected.

**Patterns:**
```cpp
FakeApplication* fake = static_cast<FakeApplication*>(app);
fake->AddMealToUserWillReturn(false);
fake->LoadMealsFromUserWillReturn(MealDTO{.name = "dragons", .user_id = 1});
```

**What to Mock:**
- Mock the `IApplication` boundary for presentation tests using `FakeApplication` in `test/core/fake_application.h`.
- Configure fake return values through explicit `WillReturn` methods before the UI action under test.
- Use `IsTestsMode()` to suppress modal presentation errors in UI tests: `test/core/fake_application.h`, `src/presentation/pages/register_page.cc`.

**What NOT to Mock:**
- Do not mock wxWidgets controls for UI flow tests. Load `src/presentation/xrc/test_resource.xrc` through `wxXmlResource` and exercise real widgets from `test/presentation_test.cc`.
- Do not introduce test-only code into production targets; add helpers under `test/core/` or `test/wx/`.
- Do not touch `OpenXLSX/` for tests unless the vendored dependency itself is under test.

## Fixtures and Factories

**Test Data:**
```cpp
MealDTO meal{.name = std::string("dragons"), .user_id = 1};
static_cast<FakeApplication*>(app)->LoadMealsFromUserWillReturn(meal);
```

**Location:**
- Test data is currently created inline in `test/presentation_test.cc`.
- Fake state and default DTO behavior live in `test/core/fake_application.h` and `test/core/fake_application.cc`.
- Resource fixtures are copied by the `copy_test_resources` target from `src/presentation/xrc/` and `spreadsheet/` into `build/share/dietapp/`.

## Coverage

**Requirements:** None enforced. No coverage target, coverage flags, or coverage threshold configuration is detected.

**View Coverage:**
```bash
# Not configured
```

## Test Types

**Unit Tests:**
- Limited direct unit testing is present. `test/core/fake_application.cc` and `test/wx/testableframe.cc` support tests, but production core/database unit test binaries are not configured in `test/CMakeLists.txt`.

**Integration Tests:**
- `test/presentation_test.cc` is a UI integration-style test: it initializes wxWidgets, loads XRC resources, constructs `Presentation`, and exercises page navigation and form validation through real controls.

**E2E Tests:**
- No external E2E framework is used. The closest coverage is the wxWidgets UI simulation in `test/presentation_test.cc`.

## Common Patterns

**Async Testing:**
```cpp
wxUIActionSimulator sim;
sim.MouseMove(button->GetScreenPosition() + wxPoint(10, 10));
wxYield();
sim.MouseClick();
wxYield();
```

**Error Testing:**
```cpp
static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(false);
sim.MouseClick();
wxYield();
REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(true);
```

---

*Testing analysis: 2026-05-15*
