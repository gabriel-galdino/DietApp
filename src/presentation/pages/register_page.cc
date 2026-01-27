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

  name_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlName", wxTextCtrl);
  name_ctrl_->SetValidator(
      wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &name_));

  if (name_ctrl_ == nullptr) {
    return;
  }

  meal_ctrl_ = XRCCTRL(*register_page_, "m_textCtrlMeal", wxTextCtrl);
  meal_ctrl_->SetValidator(
      wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &meal_));
  if (meal_ctrl_ == nullptr) {
    return;
  }

  create_button_->Bind(wxEVT_BUTTON, &RegisterPage::OnButtonCreate, this,
                       XRCID(create_button_->GetName()));
}

void RegisterPage::OnButtonCreate(wxCommandEvent& event) {
  if (name_ctrl_->Validate() == false ||
      name_ctrl_->TransferDataFromWindow() == false) {
    return;
  }

  if (meal_ctrl_->Validate() == false ||
      meal_ctrl_->TransferDataFromWindow() == false) {
    return;
  }

  if (app_->AddMealToUser(name_.utf8_string(), meal_.utf8_string()) == false) {
    return;
  }

  navigator_->NavigateTo(PageId::Create);
}