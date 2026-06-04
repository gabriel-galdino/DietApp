#define CATCH_CONFIG_RUNNER
#include "presentation/presentation.h"

#include <wx/app.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/uiaction.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/fake_application.h"
#include "wx/testableframe.h"

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

#if wxUSE_UIACTIONSIMULATOR

TEST_CASE("Presentation", "[UI Flow]") {
  wxXmlResource::Get()->InitAllHandlers();
  wxFileName xrc_resources(wxStandardPaths::Get().GetExecutablePath(), "");
  xrc_resources.RemoveLastDir();
  xrc_resources.RemoveLastDir();
  xrc_resources.AppendDir("share");
  xrc_resources.AppendDir("dietapp");
  xrc_resources.AppendDir("xrc");
  xrc_resources.SetFullName("test_resource.xrc");
  wxXmlResource::Get()->Load(xrc_resources.GetFullPath());

  IApplication* app = new FakeApplication();
  Presentation pres(app);
  wxTestableFrame* test_frame = wxDynamicCast(
      wxXmlResource::Get()->LoadFrame(nullptr, "MainFrame"), wxTestableFrame);
  wxTheApp->SetTopWindow(test_frame);
  // Use fixed position to facilitate debugging.
  test_frame->Move(200, 200);
  REQUIRE_NOTHROW(pres.Initialize(xrc_resources, test_frame));

  std::shared_ptr<InitialPage> initial_page = pres.GetInitialPage();

  SECTION("InitialPageRegisterButtonSucceed",
          "On register button click should go to the RegisterPage.") {
    REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Initial);

    EventCounter clicked(initial_page->GetRegisterButton(), wxEVT_BUTTON);

    wxUIActionSimulator sim;
    wxYield();

    //We move in slightly to account for window decorations, we need to yield
    //after every wxUIActionSimulator action to keep everything working in GTK
    sim.MouseMove(initial_page->GetRegisterButton()->GetScreenPosition() +
                  wxPoint(10, 10));
    wxYield();

    sim.MouseClick();
    wxYield();

    CHECK(clicked.GetCount() == 1);
    REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
    clicked.Clear();

    std::shared_ptr<RegisterPage> register_page = pres.GetRegisterPage();
    wxButton* reg_create_button = register_page->GetCreateButton();
    wxTextCtrl* reg_username_ctrl = register_page->GetUsernameCtrl();
    wxTextCtrl* reg_display_name_ctrl = register_page->GetDisplayNameCtrl();
    wxTextCtrl* reg_meal_ctrl = register_page->GetMealCtrl();
    EventCounter username_updated(reg_username_ctrl, wxEVT_TEXT);
    EventCounter display_name_updated(reg_display_name_ctrl, wxEVT_TEXT);
    EventCounter meal_updated(reg_meal_ctrl, wxEVT_TEXT);

    SECTION("RegisterPageCreateButtonNoData",
            "On create button click with all forms empty should remain in the "
            "same page") {
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
    }

    SECTION("RegisterPageCreateButtonNoUsername",
            "On create button click without filling the username field should "
            "remain in the same page.") {
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(false);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      register_page->GetDisplayNameCtrl()->SetFocus();
      sim.Text("The Abysswalker");
      while (reg_display_name_ctrl->GetValue() != wxString("The Abysswalker")) {
        wxYield();
      }
      display_name_updated.Clear();

      register_page->GetMealCtrl()->SetFocus();
      sim.Text("darkness");
      while (reg_meal_ctrl->GetValue() != wxString("darkness")) {
        wxYield();
      }
      meal_updated.Clear();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(true);
    }

    SECTION("RegisterPageCreateButtonNoDisplayName",
            "On create button click without filling the name field should "
            "remain in the same page.") {
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(false);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      register_page->GetUsernameCtrl()->SetFocus();
      sim.Text("artorias");
      while (reg_username_ctrl->GetValue() != wxString("artorias")) {
        wxYield();
      }
      username_updated.Clear();

      register_page->GetMealCtrl()->SetFocus();
      sim.Text("darkness");
      while (reg_meal_ctrl->GetValue() != wxString("darkness")) {
        wxYield();
      }
      meal_updated.Clear();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(true);
    }

    SECTION("RegisterPageCreateButtonNoMealName",
            "On create button click without filling the meal field should "
            "remain in the same page.") {
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(false);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      register_page->GetUsernameCtrl()->SetFocus();
      sim.Text("artorias");
      while (reg_username_ctrl->GetValue() != wxString("artorias")) {
        wxYield();
      }
      username_updated.Clear();

      register_page->GetDisplayNameCtrl()->SetFocus();
      sim.Text("The Abysswalker");
      while (reg_display_name_ctrl->GetValue() != wxString("The Abysswalker")) {
        wxYield();
      }
      display_name_updated.Clear();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      static_cast<FakeApplication*>(app)->AddMealToUserWillReturn(true);
    }

    SECTION("RegisterPageCreateButtonAppFails",
            "On create button click the application layer returns false should "
            "remain in the same page.") {
      static_cast<FakeApplication*>(app)->CreateUserWithMealWillReturn(false);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      register_page->GetUsernameCtrl()->SetFocus();
      sim.Text("artorias");
      while (reg_username_ctrl->GetValue() != wxString("artorias")) {
        wxYield();
      }
      username_updated.Clear();

      register_page->GetDisplayNameCtrl()->SetFocus();
      sim.Text("The Abysswalker");
      while (reg_display_name_ctrl->GetValue() != wxString("The Abysswalker")) {
        wxYield();
      }
      display_name_updated.Clear();

      register_page->GetMealCtrl()->SetFocus();
      sim.Text("darkness");
      while (reg_meal_ctrl->GetValue() != wxString("darkness")) {
        wxYield();
      }
      meal_updated.Clear();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      static_cast<FakeApplication*>(app)->CreateUserWithMealWillReturn(true);
    }

    SECTION(
        "RegisterPageCreateButtonSucceed"
        "On create button click filling all the required fields and no error "
        "occurs on application layer should go to CreatePage.") {
      MealDTO meal{.name = std::string("dragons"), .user_id = 1};
      static_cast<FakeApplication*>(app)->LoadMealsFromUserWillReturn(meal);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Register);
      EventCounter clicked(reg_create_button, wxEVT_BUTTON);

      sim.MouseMove(reg_create_button->GetScreenPosition() + wxPoint(10, 10));
      wxYield();

      register_page->GetUsernameCtrl()->SetFocus();
      sim.Text("ornstein");
      while (reg_username_ctrl->GetValue() != wxString("ornstein")) {
        wxYield();
      }
      username_updated.Clear();

      register_page->GetDisplayNameCtrl()->SetFocus();
      sim.Text("Dragon Slayer");
      while (reg_display_name_ctrl->GetValue() != wxString("Dragon Slayer")) {
        wxYield();
      }
      display_name_updated.Clear();

      register_page->GetMealCtrl()->SetFocus();
      sim.Text("dragons");
      while (reg_meal_ctrl->GetValue() != wxString("dragons")) {
        wxYield();
      }
      meal_updated.Clear();

      sim.MouseClick();
      wxYield();

      CHECK(clicked.GetCount() == 1);
      REQUIRE(pres.GetBook()->GetSelection() == (int)PageId::Create);

      std::shared_ptr<CreatePage> create_page = pres.GetCreatePage();
      REQUIRE(create_page != nullptr);

      wxTextCtrl* quantity_ctrl = create_page->quantity_ctrl();
      wxComboBox* food_choices_ctrl = create_page->food_choices_ctrl();
      wxDataViewCtrl* selected_foods_ctrl = create_page->selected_foods_ctrl();
      NutritionalModel* model = create_page->model();

      // Initially quantity_ctrl should be disabled and empty
      CHECK_FALSE(quantity_ctrl->IsEnabled());
      CHECK(quantity_ctrl->GetValue() == "");

      // Let's populate food choices
      FoodDTO mock_food{.id = 12,
                        .name = "Abacate",
                        .display_name = "Abacate, com casca",
                        .proteins_per_100g = 1.2,
                        .carbs_per_100g = 6.0,
                        .fats_per_100g = 8.4};
      static_cast<FakeApplication*>(app)->GetFoodDataWillReturn(mock_food);

      std::vector<FoodDTO> mock_foods = {mock_food};
      pres.FillFoodChoices(mock_foods);

      // Select food from the combobox
      food_choices_ctrl->SetSelection(0);
      wxCommandEvent combo_evt(wxEVT_COMBOBOX, food_choices_ctrl->GetId());
      combo_evt.SetEventObject(food_choices_ctrl);
      food_choices_ctrl->ProcessWindowEvent(combo_evt);

      // Now the food should be added and selected
      REQUIRE(model->GetFoodsCount() == 1);
      CHECK(model->GetFoodByRow(0).name == "Abacate");
      CHECK(quantity_ctrl->IsEnabled());
      CHECK(quantity_ctrl->GetValue() == "100.00");

      // Verify that values in model are correct (100g initially)
      wxVariant val;
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 1); // proteins
      CHECK(val.GetString() == "1.20");
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 2); // carbs
      CHECK(val.GetString() == "6.00");
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 3); // fats
      CHECK(val.GetString() == "8.40");

      // Modify the quantity using the quantity_ctrl
      quantity_ctrl->SetValue("150.00");

      // Verify updated quantity
      CHECK(model->GetFoodQuantity(0) == 150.00);

      // Verify scaled values (150g -> 1.5x)
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 1); // proteins
      CHECK(val.GetString() == "1.80");
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 2); // carbs
      CHECK(val.GetString() == "9.00");
      model->GetValue(val, wxDataViewItem(reinterpret_cast<void*>(1)), 3); // fats
      CHECK(val.GetString() == "12.60");
    }
  }
}

#endif  // wxUSE_UIACTIONSIMULATOR
