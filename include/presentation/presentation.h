#ifndef DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
#define DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H

#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/wx.h>

#include "core/application.h"

class Presentation {
 public:
  Presentation(Application* app);

  bool Initialize(wxFileName& xrc_resources, wxFrame* top_window);

  void OnButtonRegister(wxCommandEvent& event);

  wxFrame* GetMainFrame() { return main_frame_; }

  wxButton* GetRegisterButton() { return register_button_; }

  wxSimplebook* GetBook() { return book_; }

 private:
  Application* app_;
  wxFrame* main_frame_{nullptr};
  wxButton* register_button_{nullptr};
  wxSimplebook* book_{nullptr};
  const int initial_page_idx_{0};
  const int register_page_idx_{1};
  const int create_page_idx_{2};
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
