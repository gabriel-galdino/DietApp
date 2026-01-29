#include <vector>

#include <wx/simplebook.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "core/domain/meal.h"
#include "presentation/presentation.h"

Presentation::Presentation(IApplication* app) : app_(app) {}

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

  book_ = XRCCTRL(*main_frame_, "m_mainBook", wxSimplebook);
  if (book_ == nullptr) {
    return false;
  }

  wxPanel* initial_page = XRCCTRL(*book_, "m_panelPageInit", wxPanel);
  wxPanel* register_page = XRCCTRL(*book_, "m_panelPageRegister", wxPanel);
  wxPanel* create_page = XRCCTRL(*book_, "m_panelPageCreate", wxPanel);

  initial_page_ = std::make_shared<InitialPage>(initial_page, this);
  register_page_ = std::make_shared<RegisterPage>(register_page, this, app_);
  create_page_ = std::make_shared<CreatePage>(create_page, this, app_);

  main_frame_->Show();
  return true;
}

void Presentation::NavigateTo(PageId page) {
  book_->SetSelection(static_cast<size_t>(page));
}

void Presentation::NavigateToCreatePageWithMeals(std::vector<Meal> meals) {
  create_page_->SetAvailableChoices(meals);
  NavigateTo(PageId::Create);
}
