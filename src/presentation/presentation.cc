#include <vector>

#include <stdexcept>

#include <wx/simplebook.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "presentation/presentation.h"

Presentation::Presentation(IApplication* app) : app_(app) {}

void Presentation::Initialize(wxFileName& xrc_resources, wxFrame* top_window) {

  if (xrc_resources.IsOk()) {
    main_frame_ = top_window;
  } else {
    wxXmlResource::Get()->InitAllHandlers();
    wxStandardPaths& paths = wxStandardPaths::Get();
    xrc_resources = wxFileName(paths.GetDataDir(), "");
    xrc_resources.AppendDir("xrc");
    xrc_resources.SetFullName("resource.xrc");
    if (wxXmlResource::Get()->Load(xrc_resources.GetFullPath()) == false) {
      throw std::runtime_error("Falha ao carregar XRC: " +
                               xrc_resources.GetFullPath().utf8_string());
    }
  }

  if (top_window == nullptr) {
    main_frame_ = wxXmlResource::Get()->LoadFrame(nullptr, "MainFrame");
  }
  if (main_frame_ == nullptr) {
    throw std::runtime_error("Falha ao carregar janela principal.");
  }

  book_ = XRCCTRL(*main_frame_, "m_mainBook", wxSimplebook);
  if (book_ == nullptr) {
    throw std::runtime_error("Falha ao carregar controle principal.");
  }

  wxPanel* initial_page = XRCCTRL(*book_, "m_panelPageInit", wxPanel);
  wxPanel* register_page = XRCCTRL(*book_, "m_panelPageRegister", wxPanel);
  wxPanel* create_page = XRCCTRL(*book_, "m_panelPageCreate", wxPanel);
  wxPanel* enter_page = XRCCTRL(*book_, "m_panelPageEnter", wxPanel);
  if (initial_page == nullptr || register_page == nullptr ||
      create_page == nullptr || enter_page == nullptr) {
    throw std::runtime_error("Falha ao carregar paginas da interface.");
  }

  initial_page_ = std::make_shared<InitialPage>(initial_page, this);
  register_page_ = std::make_shared<RegisterPage>(register_page, this, app_);
  create_page_ = std::make_shared<CreatePage>(create_page, this, app_);
  enter_page_ = std::make_shared<EnterPage>(enter_page, this, app_);

  main_frame_->Show();
}

void Presentation::NavigateTo(PageId page) {
  book_->SetSelection(static_cast<size_t>(page));
}

void Presentation::NavigateToCreatePageWithMeals(const MealsFromUserDTO& data) {
  create_page_->SetMealsFromUser(data);
  NavigateTo(PageId::Create);
}

void Presentation::NavigateToRegisterPageWithExistingUser(
    const UserDTO& user_data) {
  register_page_->LoadExistingUser(user_data);
  register_page_->ConfigureForExistingUser();
  book_->SetSelection(static_cast<size_t>(PageId::Register));
}

void Presentation::NavigateToRegisterPageWithNewUser() {
  register_page_->ConfigureForNewUser();
  book_->SetSelection(static_cast<size_t>(PageId::Register));
};

void Presentation::FillFoodChoices(const std::vector<FoodDTO>& data) {
  create_page_->FillFoodChoices(data);
}
