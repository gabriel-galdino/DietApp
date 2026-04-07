#include "core/wx_bootstrap_app.h"

#include <wx/app.h>

#include "core/application.h"

bool WxBootstrapApp::OnInit() {
  app_ = new Application();
  app_->Initialize();
  return true;
}

int WxBootstrapApp::OnExit() {
  return app_->Shutdown();
}

wxIMPLEMENT_APP(WxBootstrapApp);