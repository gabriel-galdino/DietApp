#ifndef DIETAPP_INCLUDE_PRESENTATION_PAGES_INITIAL_PAGE_H
#define DIETAPP_INCLUDE_PRESENTATION_PAGES_INITIAL_PAGE_H

#include <wx/wx.h>

#include "presentation/navigation.h"

class InitialPage {
 public:
  InitialPage(wxPanel* initial_page, INavigation* navigator);

  void OnButtonRegister(wxCommandEvent& event);

  wxButton* GetRegisterButton() { return register_button_; }

 private:
  wxPanel* initial_page_{nullptr};
  wxButton* register_button_{nullptr};
  INavigation* navigator_{nullptr};
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PAGES_INITIAL_PAGE_H
