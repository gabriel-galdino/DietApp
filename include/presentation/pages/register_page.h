#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H

#include <algorithm>
#include <functional>

#include <wx/wx.h>

#include "core/i_application.h"
#include "presentation/navigation.h"

constexpr char kCreateUserLabel[] = "Criar Usuário";
constexpr char kAddMealLabel[] = "Adicionar Refeição";

class RegisterPage {
 public:
  RegisterPage(wxPanel* register_page, INavigation* navigator,
               IApplication* app);

  wxButton* GetCreateButton() { return create_button_; }

  wxTextCtrl* GetUsernameCtrl() { return username_ctrl_; }

  wxTextCtrl* GetDisplayNameCtrl() { return display_name_ctrl_; }

  wxTextCtrl* GetMealCtrl() { return meal_ctrl_; }

  void OnButtonCreate(wxCommandEvent& event);

  void OnButtonBack(wxCommandEvent& event);

  void LoadExistingUser(const UserDTO& user_data);

  void ConfigureForNewUser();

  void ConfigureForExistingUser();

 private:
  void ShowError(const char* err_msg);

  void ResetTexCtrls();

  bool OnButtonCreateWithNewUser();

  bool OnButtonCreateWithExistingUser();

  bool ValidateTextCtrl(wxTextCtrl* ctrl, wxString& data, const char* err_msg);

  auto FindCurrentMealPos(std::vector<MealDTO>& meals_data) const {
    return std::find_if(meals_data.begin(), meals_data.end(),       //
                        [this](const MealDTO& meal) {               //
                          return meal_.utf8_string() == meal.name;  //
                        });
  }

  wxPanel* register_page_{nullptr};
  INavigation* navigator_{nullptr};
  IApplication* app_;
  wxButton* create_button_{nullptr};
  wxButton* back_button_{nullptr};
  wxTextCtrl* display_name_ctrl_{nullptr};
  wxTextCtrl* meal_ctrl_{nullptr};
  wxTextCtrl* username_ctrl_{nullptr};
  wxString display_name_;
  wxString meal_;
  wxString username_;
  std::function<bool()> current_action_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H
