#include <wx/simplebook.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "presentation/presentation.h"

Presentation::Presentation(Application* app) : app_(app) {}

/*TODO: When running the bin of app a warning about the wxTestableFrame not
        found is shown, but this does not affect the app per se, but anyway it
        is anoying and should be removed */
bool Presentation::Initialize(wxFileName& xrc_resources, wxFrame* top_window) {

  if (xrc_resources.IsOk()) {
    main_frame_ = top_window;
  } else {
    wxXmlResource::Get()->InitAllHandlers();
    wxStandardPaths& paths = wxStandardPaths::Get();
    xrc_resources = wxFileName(paths.GetDataDir(), "");
    xrc_resources.AppendDir("xrc");
    xrc_resources.SetFullName("resource.xrc");
    wxXmlResource::Get()->Load(xrc_resources.GetFullPath());
  }

  if (top_window == nullptr) {
    main_frame_ = wxXmlResource::Get()->LoadFrame(nullptr, "MainFrame");
  }

  register_button_ = XRCCTRL(*main_frame_, "m_buttonRegister", wxButton);
  if (register_button_ == nullptr) {
    return false;
  }
  register_button_->Bind(wxEVT_BUTTON, &Presentation::OnButtonRegister, this,
                         XRCID(register_button_->GetName()));

  book_ = XRCCTRL(*main_frame_, "m_mainBook", wxSimplebook);
  if (book_ == nullptr) {
    return false;
  }

  main_frame_->Show();
  return true;
}

void Presentation::OnButtonRegister(wxCommandEvent& event) {
  book_->SetSelection(register_page_idx_);
}