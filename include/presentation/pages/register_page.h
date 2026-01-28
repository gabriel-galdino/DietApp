#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H

#include <wx/wx.h>

#include "core/i_application.h"
#include "presentation/navigation.h"

class RegisterPage {
 public:
  RegisterPage(wxPanel* register_page, INavigation* navigator,
               IApplication* app);

  wxButton* GetCreateButton() { return create_button_; }

  wxTextCtrl* GetNameCtrl() { return name_ctrl_; }

  wxTextCtrl* GetMealCtrl() { return meal_ctrl_; }

  void OnButtonCreate(wxCommandEvent& event);

 private:
  void ShowError(const std::string& err_msg);

  wxPanel* register_page_{nullptr};
  INavigation* navigator_{nullptr};
  IApplication* app_;
  wxButton* create_button_{nullptr};
  wxTextCtrl* name_ctrl_{nullptr};
  wxTextCtrl* meal_ctrl_{nullptr};
  wxString name_;
  wxString meal_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_REGISTER_PAGE_H
