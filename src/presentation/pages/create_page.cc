#include "presentation/pages/create_page.h"

#include <vector>

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/i_application.h"
#include "presentation/navigation.h"

CreatePage::CreatePage(wxPanel* create_page, INavigation* navigator,
                       IApplication* app)
    : create_page_(create_page), navigator_(navigator), app_(app) {
  choice_meals_ = XRCCTRL(*create_page, "m_choiceMeals", wxChoice);
  wxASSERT(choice_meals_);
}

bool CreatePage::SetAvailableChoices(const MealsFromUserDTO& data) {
  for (const auto& meal : data.meals) {
    choice_meals_->AppendString(wxString(meal.name));
  }
  auto meal_str = wxString(data.meals[data.selected_meal].name);
  choice_meals_->SetStringSelection(meal_str);
  return true;
}
