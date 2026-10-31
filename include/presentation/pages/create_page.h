#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H

#include <vector>

#include <wx/dataview.h>
#include <wx/wx.h>

#include "core/dto/food_dto.h"
#include "core/dto/meal_dto.h"
#include "core/i_application.h"
#include "presentation/navigation.h"
#include "presentation/nutritional_model.h"

struct Column {
  std::string type;
  std::string name;
};

class CreatePage {
 public:
  CreatePage(wxPanel* create_page, INavigation* navigator, IApplication* app);

  wxChoice* user_meals_choice_ctrl() { return user_meals_choice_ctrl_; }

  wxComboBox* food_combo_box_ctrl() { return food_combo_box_ctrl_; }

  wxDataViewCtrl* selected_foods_dataview_ctrl() {
    return selected_foods_dataview_ctrl_;
  }

  wxTextCtrl* quantity_ctrl() { return quantity_ctrl_; }

  NutritionalModel* model() { return model_.get(); }

  void SetMealsFromUser(const MealsFromUserDTO& data);

  void FillFoodsComboBox(const std::vector<FoodDTO>& data);

 private:
  void OnItemSelected(wxCommandEvent& event);

  void OnContextMenuActivated(wxDataViewEvent& event);

  void OnDeleteKey(wxKeyEvent& event);

  void OnQuantityChanged(wxCommandEvent& event);

  void OnSelectionChanged(wxDataViewEvent& event);

  void OnMealSelected(wxCommandEvent& event);

  void OnButtonSave(wxCommandEvent& event);

  wxPanel* create_page_;
  INavigation* navigator_;
  IApplication* app_;
  wxChoice* user_meals_choice_ctrl_;
  wxComboBox* food_combo_box_ctrl_;
  wxDataViewCtrl* selected_foods_dataview_ctrl_;
  wxTextCtrl* quantity_ctrl_;
  bool is_updating_quantity_ = false;
  MealsFromUserDTO meals_from_user_data_;
  wxObjectDataPtr<NutritionalModel> model_;
  std::vector<FoodDTO> foods_combo_box_data_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H
