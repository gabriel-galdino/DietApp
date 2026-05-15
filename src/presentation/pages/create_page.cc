#include "presentation/pages/create_page.h"

#include <cstdint>
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
  food_choices_ctrl_ = XRCCTRL(*create_page, "m_comboBoxItems", wxComboBox);
  wxPanel* right_panel = XRCCTRL(*create_page, "m_panelRight", wxPanel);
  selected_foods_ctrl_ = new wxDataViewCtrl(
      right_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxDV_ROW_LINES | wxDV_HORIZ_RULES | wxDV_VERT_RULES);
  wxASSERT(user_meals_);
  wxASSERT(food_choices_ctrl_);
  wxASSERT(selected_foods_ctrl_);

  wxSizer* sizer = right_panel->GetSizer();
  sizer->Add(selected_foods_ctrl_, 1, wxALL | wxEXPAND, 5);

  const std::vector<Column> columns{
      {.type = "string", .name = ""},
      {.type = "string", .name = "Proteínas (kcal)"},
      {.type = "string", .name = "Carboidratos (kcal)"},
      {.type = "string", .name = "Gorduras (kcal)"},
      {.type = "string", .name = "Parcial (kcal)"},
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

  model_ = new NutritionalModel();
  selected_foods_ctrl_->AssociateModel(model_.get());

  food_choices_ctrl_->Bind(wxEVT_COMBOBOX, &CreatePage::OnItemSelected, this,
                           XRCID(food_choices_ctrl_->GetName()));

  selected_foods_ctrl_->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU,
                             &CreatePage::OnContextMenuActivated, this);

  selected_foods_ctrl_->Bind(wxEVT_KEY_DOWN, &CreatePage::OnDeleteKey, this);
}

bool CreatePage::SetMealsFromUser(const MealsFromUserDTO& data) {
  for (const auto& meal : data.meals) {
    user_meals_->AppendString(wxString(meal.name));
  }
  auto meal_str = wxString(data.meals[data.selected_meal].name);
  user_meals_->SetStringSelection(meal_str);
  return true;
}

void CreatePage::FillFoodChoices(const std::vector<FoodDTO>& data) {
  foods_data_ = data;
  for (const auto& food : foods_data_) {
    food_choices_ctrl_->AppendString(wxString::FromUTF8(food.name));
  }
  food_choices_ctrl_->SetSelection(0);
}

void CreatePage::OnItemSelected(wxCommandEvent& event) {
  wxString selected_food = food_choices_ctrl_->GetStringSelection();
  FoodDTO food = app_->GetFoodData(selected_food.utf8_string());
  model_->AddFoodToSelection(food);
  model_->UpdateSelectedFoods();
}

void CreatePage::OnContextMenuActivated(wxDataViewEvent& event) {
  wxDataViewItem item = event.GetItem();
  if (!item.IsOk()) {
    return;
  }

  wxMenu menu;
  menu.Append(wxID_DELETE, "Excluir Alimento");

  int sel = create_page_->GetPopupMenuSelectionFromUser(menu);
  if (sel == wxID_DELETE) {
    unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
    const FoodDTO& food = model_->GetFoodByRow(row);
    model_->DeleteFoodFromSelection(food);
    model_->UpdateSelectedFoods();
  }
}

void CreatePage::OnDeleteKey(wxKeyEvent& event) {
  if (event.GetKeyCode() == WXK_DELETE) {
    wxDataViewItem item = selected_foods_ctrl_->GetSelection();
    if (!item.IsOk()) {
      return;
    }

    unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
    const FoodDTO& food = model_->GetFoodByRow(row);
    model_->DeleteFoodFromSelection(food);
    model_->UpdateSelectedFoods();
  } else {
    event.Skip();
  }
}
