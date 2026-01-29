#include "presentation/pages/register_page.h"

#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/i_application.h"
#include "presentation/navigation.h"

RegisterPage::RegisterPage(wxPanel* register_page, INavigation* navigator,
                           IApplication* app)
    : register_page_(register_page), navigator_(navigator), app_(app) {
  create_button_ = XRCCTRL(*register_page, "m_buttonCreate", wxButton);
  if (create_button_ == nullptr) {
    return;
  }

  uint filter = wxFILTER_ASCII |  //
                wxFILTER_EMPTY |  //
                wxFILTER_INCLUDE_CHAR_LIST;
  auto name_validator = wxTextValidator(filter, &name_);
  auto meal_validator = wxTextValidator(filter, &meal_);
  wxString included_chars = "ç~^´";
  name_validator.AddCharIncludes(included_chars);
  meal_validator.AddCharIncludes(included_chars);

  name_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlName", wxTextCtrl);
  name_ctrl_->SetValidator(name_validator);
  if (name_ctrl_ == nullptr) {
    return;
  }

  meal_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlMeal", wxTextCtrl);
  meal_ctrl_->SetValidator(meal_validator);
  if (meal_ctrl_ == nullptr) {
    return;
  }

  create_button_->Bind(wxEVT_BUTTON, &RegisterPage::OnButtonCreate, this,
                       XRCID(create_button_->GetName()));
}

void RegisterPage::OnButtonCreate(wxCommandEvent& event) {
  name_ctrl_->TransferDataFromWindow();
  auto name_validator =
      wxDynamicCast(name_ctrl_->GetValidator(), wxTextValidator);
  if (name_validator->IsValid(name_).empty() == false) {
    ShowError(std::string("Campo nome é inválido."));
    return;
  }

  meal_ctrl_->TransferDataFromWindow();
  auto meal_validator =
      wxDynamicCast(meal_ctrl_->GetValidator(), wxTextValidator);
  if (meal_validator->IsValid(meal_).empty() == false) {
    ShowError(std::string("Campo refeição é inválido."));
    return;
  }

  if (app_->AddMealToUser(name_.utf8_string(), meal_.utf8_string()) == false) {
    ShowError(std::string("Falha em salvar dados."));
    return;
  }

  auto user_meals = app_->LoadMealsFromUser(name_.utf8_string());

  navigator_->NavigateToCreatePageWithMeals(user_meals);
}

void RegisterPage::ShowError(const std::string& err_msg) {
  if (app_->IsTestsMode() == false) {
    wxMessageBox(wxString(wxString::FromUTF8(err_msg)), wxT("Erro"),
                 wxICON_ERROR);
  }
}