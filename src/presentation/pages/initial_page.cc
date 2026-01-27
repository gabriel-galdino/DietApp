#include "presentation/pages/initial_page.h"

#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

InitialPage::InitialPage(wxPanel* initial_page, INavigation* navigator)
    : initial_page_(initial_page), navigator_(navigator) {
  register_button_ = XRCCTRL(*initial_page, "m_buttonRegister", wxButton);
  if (register_button_ == nullptr) {
    return;
  }
  register_button_->Bind(wxEVT_BUTTON, &InitialPage::OnButtonRegister, this,
                         XRCID(register_button_->GetName()));
}

void InitialPage::OnButtonRegister(wxCommandEvent& event) {
  navigator_->NavigateTo(PageId::Register);
}