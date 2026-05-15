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

  wxChoice* user_meals() { return user_meals_; }

  bool SetMealsFromUser(const MealsFromUserDTO& data);

  void FillFoodChoices(const std::vector<FoodDTO>& data);

 private:
  void OnItemSelected(wxCommandEvent& event);

  void OnContextMenuActivated(wxDataViewEvent& event);

  void OnDeleteKey(wxKeyEvent& event);

  wxPanel* create_page_;
  INavigation* navigator_;
  IApplication* app_;
  wxChoice* user_meals_;
  wxComboBox* food_choices_ctrl_;
  wxDataViewCtrl* selected_foods_ctrl_;
  wxObjectDataPtr<NutritionalModel> model_;
  std::vector<FoodDTO> foods_data_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H
