#ifndef DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
#define DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H

#include <memory>
#include <vector>

#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/wx.h>

#include "core/dto/food_dto.h"
#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "core/i_application.h"
#include "presentation/navigation.h"
#include "presentation/pages/create_page.h"
#include "presentation/pages/enter_page.h"
#include "presentation/pages/initial_page.h"
#include "presentation/pages/register_page.h"

class Presentation : public INavigation {
 public:
  void NavigateTo(PageId page) override;

  void NavigateToCreatePageWithMeals(const MealsFromUserDTO& data) override;

  void NavigateToRegisterPageWithNewUser() override;

  void NavigateToRegisterPageWithExistingUser(
      const UserDTO& user_data) override;

  Presentation(IApplication* app);

  bool Initialize(wxFileName& xrc_resources, wxFrame* top_window);

  wxFrame* GetMainFrame() { return main_frame_; }

  wxSimplebook* GetBook() { return book_; }

  std::shared_ptr<InitialPage> GetInitialPage() { return initial_page_; }

  std::shared_ptr<RegisterPage> GetRegisterPage() { return register_page_; }

  bool FillFoodChoices(const std::vector<FoodDTO>& data);

 private:
  IApplication* app_;
  wxFrame* main_frame_{nullptr};
  wxSimplebook* book_{nullptr};
  std::shared_ptr<InitialPage> initial_page_;
  std::shared_ptr<RegisterPage> register_page_;
  std::shared_ptr<CreatePage> create_page_;
  std::shared_ptr<EnterPage> enter_page_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
