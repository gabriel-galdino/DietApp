#include "presentation/pages/create_page.h"

#include <vector>

#include <wx/dataview.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/i_application.h"
#include "presentation/navigation.h"
#include "presentation/nutritional_model.h"

CreatePage::CreatePage(wxPanel* create_page, INavigation* navigator,
                       IApplication* app)
    : create_page_(create_page), navigator_(navigator), app_(app) {
  user_meals_ = XRCCTRL(*create_page, "m_choiceMeals", wxChoice);
  food_choices_ = XRCCTRL(*create_page, "m_comboBoxItems", wxComboBox);
  wxPanel* right_panel = XRCCTRL(*create_page, "m_panelRight", wxPanel);
  selected_foods_ctrl_ = new wxDataViewCtrl(
      right_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxDV_ROW_LINES | wxDV_HORIZ_RULES | wxDV_VERT_RULES);
  wxASSERT(user_meals_);
  wxASSERT(food_choices_);
  wxASSERT(selected_foods_ctrl_);

  wxSizer* sizer = right_panel->GetSizer();
  sizer->Add(selected_foods_ctrl_, 1, wxALL | wxEXPAND, 5);

  const std::vector<Column> columns{
      {.type = "string", .name = ""},
      {.type = "double", .name = "Proteínas (kcal)"},
      {.type = "double", .name = "Gorduras (kcal)"},
      {.type = "double", .name = "Carboidratos (kcal)"},
      {.type = "double", .name = "Parcial (kcal)"},
  };

  for (int col_index = 0; col_index < columns.size(); col_index++) {
    const auto& column = columns[col_index];
    wxDataViewTextRenderer* renderer = new wxDataViewTextRenderer(
        wxString::FromUTF8(column.type), wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* col =
        new wxDataViewColumn(wxString::FromUTF8(column.name), renderer,
                             col_index, 150, wxALIGN_LEFT);
    selected_foods_ctrl_->AppendColumn(col);
  }

  model_ = new NutritionalModel({});
  selected_foods_ctrl_->AssociateModel(model_.get());

  food_choices_->Bind(wxEVT_COMBOBOX, &CreatePage::OnItemSelected, this,
                      XRCID(food_choices_->GetName()));
}

bool CreatePage::SetMealsFromUser(const MealsFromUserDTO& data) {
  for (const auto& meal : data.meals) {
    user_meals_->AppendString(wxString(meal.name));
  }
  auto meal_str = wxString(data.meals[data.selected_meal].name);
  user_meals_->SetStringSelection(meal_str);
  return true;
}

bool CreatePage::FillFoodChoices(const std::vector<FoodDTO>& data) {
  for (const auto& food : data) {
    food_choices_->AppendString(wxString::FromUTF8(food.name));
  }
  food_choices_->SetSelection(0);
  return true;
}

void CreatePage::OnItemSelected(wxCommandEvent& event) {
  std::cout << "CreatePage::OnItemSelected" << std::endl;
  return;
}