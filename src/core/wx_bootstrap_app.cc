#include "core/wx_bootstrap_app.h"

#include <wx/app.h>

#include "core/application.h"

bool WxBootstrapApp::OnInit() {
  try {
    app_ = new Application();
    app_->Initialize();
  } catch (const std::exception& e) {
    wxLogError("Falha ao iniciar aplicação: %s.", e.what());
    wxMessageBox(
        wxString::Format("O DietApp não pôde ser iniciado:\n%s", e.what()),
        "Erro Fatal", wxOK | wxICON_ERROR);
    delete app_;
    app_ = nullptr;
    return false;
  }
  return true;
}

int WxBootstrapApp::OnExit() {
  delete app_;
  app_ = nullptr;
  return 0;
}

wxIMPLEMENT_APP(WxBootstrapApp);
