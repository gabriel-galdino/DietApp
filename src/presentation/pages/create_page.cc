#include "presentation/pages/create_page.h"

#include <cstdint>
#include <vector>

#include <wx/dataview.h>
#include <wx/string.h>
#include <wx/string.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/dto/meal_dto.h"
#include "core/i_application.h"
#include "presentation/navigation.h"
#include "presentation/nutritional_model.h"

CreatePage::CreatePage(wxPanel* create_page, INavigation* navigator,
                       IApplication* app)
    : create_page_(create_page), navigator_(navigator), app_(app) {
  user_meals_choice_ctrl_ = XRCCTRL(*create_page, "m_choiceMeals", wxChoice);
  food_combo_box_ctrl_ = XRCCTRL(*create_page, "m_comboBoxItems", wxComboBox);
  wxPanel* right_panel = XRCCTRL(*create_page, "m_panelRight", wxPanel);
  selected_foods_dataview_ctrl_ = new wxDataViewCtrl(
      right_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxDV_ROW_LINES | wxDV_HORIZ_RULES | wxDV_VERT_RULES);
  wxASSERT(user_meals_choice_ctrl_);
  wxASSERT(food_combo_box_ctrl_);
  wxASSERT(selected_foods_dataview_ctrl_);

  wxSizer* sizer = right_panel->GetSizer();
  sizer->Add(selected_foods_dataview_ctrl_, 1, wxALL | wxEXPAND, 5);

  const std::vector<Column> columns{
      {.type = "string", .name = ""},
      {.type = "string", .name = "Proteínas (g)"},
      {.type = "string", .name = "Carboidratos (g)"},
      {.type = "string", .name = "Gorduras (g)"},
      {.type = "string", .name = "kcal"},
  };

  for (int col_index = 0; col_index < columns.size(); col_index++) {
    const auto& column = columns[col_index];
    wxDataViewTextRenderer* renderer = new wxDataViewTextRenderer(
        wxString::FromUTF8(column.type), wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* col =
        new wxDataViewColumn(wxString::FromUTF8(column.name), renderer,
                             col_index, 150, wxALIGN_LEFT);
    selected_foods_dataview_ctrl_->AppendColumn(col);
  }

  model_ = new NutritionalModel();
  selected_foods_dataview_ctrl_->AssociateModel(model_.get());

  quantity_ctrl_ = XRCCTRL(*create_page, "m_textCtrlQty", wxTextCtrl);
  wxASSERT(quantity_ctrl_);
  quantity_ctrl_->Enable(false);

  food_combo_box_ctrl_->Bind(wxEVT_COMBOBOX, &CreatePage::OnItemSelected, this,
                             XRCID(food_combo_box_ctrl_->GetName()));

  selected_foods_dataview_ctrl_->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU,
                                      &CreatePage::OnContextMenuActivated,
                                      this);

  selected_foods_dataview_ctrl_->Bind(wxEVT_KEY_DOWN, &CreatePage::OnDeleteKey,
                                      this);

  selected_foods_dataview_ctrl_->Bind(wxEVT_DATAVIEW_SELECTION_CHANGED,
                                      &CreatePage::OnSelectionChanged, this);

  quantity_ctrl_->Bind(wxEVT_TEXT, &CreatePage::OnQuantityChanged, this,
                       XRCID(quantity_ctrl_->GetName()));

  user_meals_choice_ctrl_->Bind(wxEVT_CHOICE, &CreatePage::OnMealSelected, this,
                                XRCID(user_meals_choice_ctrl_->GetName()));

  wxButton* save_button = XRCCTRL(*create_page, "m_buttonSave", wxButton);
  wxASSERT(save_button);
  save_button->Bind(wxEVT_BUTTON, &CreatePage::OnButtonSave, this,
                    XRCID(save_button->GetName()));
}

void CreatePage::SetMealsFromUser(const MealsFromUserDTO& data) {
  user_meals_choice_ctrl_->Clear();
  meals_from_user_data_ = data;

  for (const auto& meal : meals_from_user_data_.meals) {
    user_meals_choice_ctrl_->AppendString(wxString(meal.name));
  }
  const auto &new_selected_meal = meals_from_user_data_.meals.back().name;
  user_meals_choice_ctrl_->SetStringSelection(wxString::FromUTF8(new_selected_meal));

  for (const auto& food : meals_from_user_data_.meals.front().foods) {
    model_->AddFoodToSelection(food);
  }

  model_->ClearFoods();
  model_->UpdateSelectedFoods();
}

void CreatePage::FillFoodsComboBox(const std::vector<FoodDTO>& data) {
  foods_combo_box_data_ = data;
  for (const auto& food : foods_combo_box_data_) {
    food_combo_box_ctrl_->AppendString(wxString::FromUTF8(food.name));
  }
  food_combo_box_ctrl_->SetSelection(0);
}

void CreatePage::OnItemSelected(wxCommandEvent& event) {
  wxString selected_food = food_combo_box_ctrl_->GetStringSelection();
  FoodDTO food = app_->GetFoodData(selected_food.utf8_string());
  model_->AddFoodToSelection(food);
  model_->UpdateSelectedFoods();

  // Select the newly added food row in the table
  unsigned int new_row = model_->GetFoodsCount() - 1;
  wxDataViewItem item(reinterpret_cast<void*>(new_row + 1));
  selected_foods_dataview_ctrl_->Select(item);

  // Manually update quantity control to reflect selection
  quantity_ctrl_->Enable(true);
  quantity_ctrl_->ChangeValue(wxString::Format("%.2lf", food.quantity));
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
    wxDataViewItem item = selected_foods_dataview_ctrl_->GetSelection();
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

void CreatePage::OnSelectionChanged(wxDataViewEvent& event) {
  if (is_updating_quantity_)
    return;

  wxDataViewItem item = event.GetItem();
  if (item.IsOk() == false) {
    quantity_ctrl_->ChangeValue("");
    quantity_ctrl_->Enable(false);
    return;
  }

  unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
  if (row < model_->GetFoodsCount()) {
    double qty = model_->GetFoodQuantity(row);
    quantity_ctrl_->Enable(true);
    quantity_ctrl_->ChangeValue(wxString::Format("%.2lf", qty));
  } else {
    quantity_ctrl_->ChangeValue("");
    quantity_ctrl_->Enable(false);
  }
}

void CreatePage::OnQuantityChanged(wxCommandEvent& event) {
  if (is_updating_quantity_)
    return;

  wxDataViewItem item = selected_foods_dataview_ctrl_->GetSelection();
  if (item.IsOk() == false)
    return;

  unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
  if (row >= model_->GetFoodsCount())
    return;

  double qty = 0.0;
  wxString val = quantity_ctrl_->GetValue();
  if (val.IsEmpty()) {
    qty = 0.0;
  } else if (val.ToDouble(&qty) == false || qty < 0.0) {
    return;
  }

  is_updating_quantity_ = true;
  model_->UpdateFoodQuantity(row, qty);
  model_->UpdateSelectedFoods();
  selected_foods_dataview_ctrl_->Select(item);
  is_updating_quantity_ = false;
}

void CreatePage::OnMealSelected(wxCommandEvent& event) {
  const std::string& new_selected_meal =
      user_meals_choice_ctrl_->GetStringSelection().utf8_string();
  for (const auto& meal : meals_from_user_data_.meals) {
    if (meal.name == new_selected_meal) {
      model_->set_selected_foods(meal.foods);
      model_->Cleared();
      model_->UpdateSelectedFoods();
      break;
    }
  }
}

void CreatePage::OnButtonSave(wxCommandEvent& event) {
  std::string curr_selected_meal =
      user_meals_choice_ctrl_->GetStringSelection().utf8_string();
  for (auto& meal : meals_from_user_data_.meals) {
    if (meal.name == curr_selected_meal) {
      meal.foods = model_->selected_foods();
      app_->SaveMealFoods(meal);
      break;
    }
  }
}
