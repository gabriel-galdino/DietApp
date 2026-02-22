#include "presentation/pages/enter_page.h"

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/i_application.h"
#include "presentation/error_messages.h"
#include "presentation/navigation.h"

EnterPage::EnterPage(wxPanel* enter_page, INavigation* navigator,
                     IApplication* app)
    : enter_page_(enter_page), navigator_(navigator), app_(app) {
  follow_on_button_ = XRCCTRL(*enter_page_, "m_buttonFollowOn", wxButton);
  back_button_ = XRCCTRL(*enter_page_, "m_buttonBack", wxButton);
  username_ctrl_ = XRCCTRL(*enter_page_, "m_textCtrlUsername", wxTextCtrl);
  wxASSERT(follow_on_button_);
  wxASSERT(back_button_);
  wxASSERT(username_ctrl_);

  uint filter_for_alphanum = wxFILTER_ALPHANUMERIC | wxFILTER_EMPTY;
  auto username_validator = wxTextValidator(filter_for_alphanum, &username_);
  username_ctrl_->SetValidator(username_validator);

  follow_on_button_->Bind(wxEVT_BUTTON, &EnterPage::OnButtonFollowOn, this,
                          XRCID(follow_on_button_->GetName()));

  back_button_->Bind(wxEVT_BUTTON, &EnterPage::OnButtonBack, this,
                     XRCID(back_button_->GetName()));
}

void EnterPage::OnButtonFollowOn(wxCommandEvent& event) {
  username_ctrl_->TransferDataFromWindow();
  auto username_validator =
      wxDynamicCast(username_ctrl_->GetValidator(), wxTextValidator);
  if (username_validator->IsValid(username_).empty() == false) {
    ShowError(kUserNameErrMsg);
    return;
  }

  if (app_->DoesUserExist(username_.utf8_string()) == false) {
    ShowError(kUserNotFoundErrMsg);
    return;
  }

  auto user_data = app_->GetUserData(username_.utf8_string());

  navigator_->NavigateToRegisterPageWithExistingUser(user_data);
}

void EnterPage::ShowError(const char* err_msg) {
  if (app_->IsTestsMode() == false) {
    wxMessageBox(wxString::FromUTF8(err_msg), wxT("Erro"), wxICON_ERROR);
  }
}

void EnterPage::OnButtonBack(wxCommandEvent& event) {
  navigator_->NavigateTo(PageId::Initial);
}
