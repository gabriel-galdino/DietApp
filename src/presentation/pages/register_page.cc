#include "presentation/pages/register_page.h"

#include <iterator>

#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/dto/user_dto.h"
#include "core/i_application.h"
#include "presentation/error_messages.h"
#include "presentation/navigation.h"

RegisterPage::RegisterPage(wxPanel* register_page, INavigation* navigator,
                           IApplication* app)
    : register_page_(register_page), navigator_(navigator), app_(app) {
  create_button_ = XRCCTRL(*register_page, "m_buttonCreate", wxButton);
  back_button_ = XRCCTRL(*register_page, "m_buttonBack", wxButton);
  username_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlUsername", wxTextCtrl);
  display_name_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlName", wxTextCtrl);
  meal_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlMeal", wxTextCtrl);

  wxASSERT(create_button_);
  wxASSERT(back_button_);
  wxASSERT(username_ctrl_);
  wxASSERT(display_name_ctrl_);
  wxASSERT(meal_ctrl_);

  uint filter_for_alphanum = wxFILTER_ALPHANUMERIC | wxFILTER_EMPTY;
  auto username_validator = wxTextValidator(filter_for_alphanum, &username_);

  uint filter_for_ascii = wxFILTER_ASCII | wxFILTER_EMPTY;
  auto display_name_validator =
      wxTextValidator(filter_for_ascii, &display_name_);
  auto meal_validator = wxTextValidator(filter_for_ascii, &meal_);

  username_ctrl_->SetValidator(username_validator);
  display_name_ctrl_->SetValidator(display_name_validator);
  meal_ctrl_->SetValidator(meal_validator);

  create_button_->Bind(wxEVT_BUTTON, &RegisterPage::OnButtonCreate, this,
                       XRCID(create_button_->GetName()));
  back_button_->Bind(wxEVT_BUTTON, &RegisterPage::OnButtonBack, this,
                     XRCID(back_button_->GetName()));
}

void RegisterPage::OnButtonCreate(wxCommandEvent& event) {
  if (ValidateTextCtrl(username_ctrl_, username_, kUserNameErrMsg) == false) {
    return;
  }

  if (ValidateTextCtrl(display_name_ctrl_, display_name_, kDisplayNameErrMsg) ==
      false) {
    return;
  }

  if (ValidateTextCtrl(meal_ctrl_, meal_, kMealNameErrMsg) == false) {
    return;
  }

  if (current_action_() == false) {
    return;
  }

  auto meals = app_->LoadMealsFromUser(username_.utf8_string());
  auto it = FindCurrentMealPos(meals);
  int selected_index = std::distance(meals.begin(), it);
  MealsFromUserDTO data{.meals = meals, .selected_meal = selected_index};
  navigator_->NavigateToCreatePageWithMeals(std::move(data));
}

void RegisterPage::ShowError(const char* err_msg) {
  if (app_->IsTestsMode() == false) {
    wxMessageBox(wxString::FromUTF8(err_msg), wxT("Erro"), wxICON_ERROR);
  }
}

void RegisterPage::OnButtonBack(wxCommandEvent& event) {
  navigator_->NavigateTo(PageId::Initial);
  ResetTexCtrls();
}

void RegisterPage::LoadExistingUser(const UserDTO& user_data) {
  username_ = wxString(wxString::FromUTF8(user_data.username));
  display_name_ = wxString(wxString::FromUTF8(user_data.display_name));
  username_ctrl_->TransferDataToWindow();
  display_name_ctrl_->TransferDataToWindow();
  username_ctrl_->SetEditable(false);
  display_name_ctrl_->SetEditable(false);
}

void RegisterPage::ResetTexCtrls() {
  username_ctrl_->SetEditable(true);
  display_name_ctrl_->SetEditable(true);
  meal_ctrl_->SetEditable(true);
}

void RegisterPage::ConfigureForNewUser() {
  create_button_->SetLabel(wxString::FromUTF8(kCreateUserLabel));
  current_action_ = [this]() {
    return OnButtonCreateWithNewUser();
  };
}

void RegisterPage::ConfigureForExistingUser() {
  create_button_->SetLabel(wxString::FromUTF8(kAddMealLabel));
  current_action_ = [this]() {
    return OnButtonCreateWithExistingUser();
  };
}

bool RegisterPage::OnButtonCreateWithNewUser() {
  CreateUserWithMealDTO data = {.username = username_.utf8_string(),
                                .display_name = display_name_.utf8_string(),
                                .meal_name = meal_.utf8_string()};
  if (app_->CreateUserWithMeal(std::move(data)) == false) {
    ShowError(kCreateUserFailedErrMsg);
    return false;
  }

  return true;
}

bool RegisterPage::OnButtonCreateWithExistingUser() {
  ResetTexCtrls();
  AddMealToUserDTO data = {.username = username_.utf8_string(),
                           .meal_name = meal_.utf8_string()};
  if (app_->AddMealToUser(std::move(data)) == false) {
    ShowError(kMealAddFailedErrMsg);
    return false;
  }

  return true;
}

bool RegisterPage::ValidateTextCtrl(wxTextCtrl* ctrl, wxString& data,
                                    const char* err_msg) {
  ctrl->TransferDataFromWindow();
  auto validator = wxDynamicCast(ctrl->GetValidator(), wxTextValidator);
  if (validator->IsValid(data).empty() == false) {
    ShowError(err_msg);
    return false;
  }

  return true;
}