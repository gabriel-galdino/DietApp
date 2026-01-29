#include "presentation/pages/create_page.h"

#include <vector>

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/domain/meal.h"
#include "core/i_application.h"
#include "presentation/navigation.h"

CreatePage::CreatePage(wxPanel* create_page, INavigation* navigator,
                       IApplication* app)
    : create_page_(create_page), navigator_(navigator), app_(app) {
  choice_meals_ = XRCCTRL(*create_page, "m_choiceMeals", wxChoice);
  if (choice_meals_ == nullptr) {
    return;
  }
}

bool CreatePage::SetAvailableChoices(std::vector<Meal> meals) {
  for (const auto& meal : meals) {
    choice_meals_->AppendString(wxString(wxString::FromUTF8(meal.name())));
  }
  choice_meals_->SetSelection(0);
  return true;
}
