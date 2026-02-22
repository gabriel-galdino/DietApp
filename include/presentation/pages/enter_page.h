#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_ENTER_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_ENTER_PAGE_H

#include <wx/wx.h>

#include "core/i_application.h"
#include "presentation/navigation.h"

class EnterPage {
 public:
  EnterPage(wxPanel* enter_page, INavigation* navigator, IApplication* app);

  void OnButtonFollowOn(wxCommandEvent& event);

  void OnButtonBack(wxCommandEvent& event);

 private:
  void ShowError(const char* err_msg);

  wxPanel* enter_page_;
  INavigation* navigator_;
  IApplication* app_;
  wxButton* follow_on_button_;
  wxButton* back_button_;
  wxTextCtrl* username_ctrl_;
  wxString username_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_ENTER_PAGE_H
