#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H

#include <vector>

#include <wx/wx.h>

#include "core/domain/meal.h"
#include "core/i_application.h"
#include "presentation/navigation.h"

class CreatePage {
 public:
  CreatePage(wxPanel* create_page, INavigation* navigator, IApplication* app);

  wxChoice* choice_meals() { return choice_meals_; }

  bool SetAvailableChoices(std::vector<Meal> meals);

 private:
  wxPanel* create_page_;
  INavigation* navigator_;
  IApplication* app_;
  wxChoice* choice_meals_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_CREATE_PAGE_H
