# DietApp

## What This Is

DietApp is an existing native Linux desktop application for calculating meal calories and nutrition from the Brazilian TACO food composition spreadsheet. It uses wxWidgets/XRC for the UI, SQLite for local data, and a Catch2 presentation test suite to exercise the main user flows through real wx controls.

The current project focus is a narrow stabilization effort: diagnose and fix the segmentation violation in the Catch2 presentation test path `Presentation > InitialPageRegisterButtonSucceed > RegisterPageCreateButtonAppFails`.

## Core Value

The presentation test suite must run reliably enough to expose real UI regressions instead of crashing during the register-page failure scenario.

## Requirements

### Validated

- ✓ Desktop application shell exists with wxWidgets startup, XRC resources, and a `Presentation` controller that owns page navigation — existing
- ✓ Register, enter, initial, and create page controllers are wired through `IApplication` and `INavigation` boundaries — existing
- ✓ Presentation tests run under Catch2 with a `FakeApplication`, `wxUIActionSimulator`, copied XRC resources, and a testable frame — existing
- ✓ GitHub Actions headless GTK setup has already been addressed with `xvfb-run --auto-servernum -- ./build/test/presentation_test` — existing context, not current scope

### Active

- [ ] Reproduce or isolate the SIGSEGV in `Presentation > InitialPageRegisterButtonSucceed > RegisterPageCreateButtonAppFails` when running the Catch2 `presentation_test` suite in an environment with wx/GTK initialized.
- [ ] Identify the crashing owner/lifetime/event-handling path in the register-page failure scenario.
- [ ] Fix the production or test-support code responsible for the segmentation violation without broadening scope to unrelated UI flows.
- [ ] Verify `./build/test/presentation_test` completes the affected scenario without SIGSEGV.

### Out of Scope

- Fixing the GTK `DISPLAY` initialization error — this was a separate GitHub Actions container issue already addressed with Xvfb.
- Reworking the full presentation test architecture — useful later, but the current effort is only the known SIGSEGV.
- Adding new DietApp features such as meal-food persistence, quantity input, authentication, or database migrations — unrelated to this stabilization task.
- Editing vendored `OpenXLSX/` — the crash is in the presentation test flow, not the spreadsheet dependency.

## Context

The failing path is a nested Catch2 section:

`Presentation > InitialPageRegisterButtonSucceed > RegisterPageCreateButtonAppFails`

That path first navigates from the initial page to the register page, then fills username, display name, and meal controls, forces `FakeApplication::AddMealToUser` to return `false`, clicks the register page create button, and expects the page selection to remain `PageId::Register`.

Relevant files:

- `test/presentation_test.cc` — Catch2/wxWidgets UI-flow test and failing nested section.
- `test/core/fake_application.h` and `test/core/fake_application.cc` — fake `IApplication` implementation used by the presentation suite.
- `src/presentation/pages/register_page.cc` and `include/presentation/pages/register_page.h` — register page validation, action selection, failure handling, and navigation.
- `src/presentation/presentation.cc` and `include/presentation/presentation.h` — page creation and navigation shell.
- `src/presentation/xrc/test_resource.xrc` — XRC resource loaded by the test binary.

Important distinction: the local sandbox cannot initialize GTK because no `DISPLAY` is available, producing `Error: Unable to initialize GTK+, is DISPLAY set properly?`. That is not the reported SIGSEGV. The SIGSEGV appears when the suite runs with Catch2 in a display-capable environment.

The codebase map also flags presentation UI tests as broad and stateful: one large nested Catch2 test uses `wxUIActionSimulator`, fixed screen positions, repeated `wxYield`, and shared setup state. This makes section-specific crashes harder to localize and should guide debugging, but large-scale restructuring is not part of this project.

## Constraints

- **Scope**: Only fix the current `presentation_test` SIGSEGV — the user explicitly excluded broader test hardening for now.
- **Environment**: wxWidgets/GTK tests require a display or Xvfb — local no-display failures are not evidence of the target bug.
- **Test command**: The canonical affected binary is `./build/test/presentation_test`; in headless CI it should run under `xvfb-run --auto-servernum --`.
- **Architecture**: Keep presentation behavior behind existing `IApplication` and `INavigation` boundaries.
- **Dependency boundary**: Do not modify `OpenXLSX/` unless work explicitly targets that vendored library.

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| Keep scope limited to the Catch2 SIGSEGV in `RegisterPageCreateButtonAppFails` | The GTK initialization error is separate and already handled with Xvfb; broad hardening would dilute the immediate debugging target. | — Pending |
| Treat existing codebase behavior as brownfield context | The app and test suite already exist, so project initialization should preserve existing capabilities while targeting the crash. | — Pending |

## Evolution

This document evolves at phase transitions and milestone boundaries.

**After each phase transition** (via `$gsd-transition`):
1. Requirements invalidated? → Move to Out of Scope with reason
2. Requirements validated? → Move to Validated with phase reference
3. New requirements emerged? → Add to Active
4. Decisions to log? → Add to Key Decisions
5. "What This Is" still accurate? → Update if drifted

**After each milestone** (via `$gsd-complete-milestone`):
1. Full review of all sections
2. Core Value check — still the right priority?
3. Audit Out of Scope — reasons still valid?
4. Update Context with current state

---
*Last updated: 2026-05-15 after initialization*
