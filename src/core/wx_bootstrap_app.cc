#include "core/wx_bootstrap_app.h"

#include <wx/app.h>

#include "core/application.h"

bool WxBootstrapApp::OnInit() {
  app_ = new Application();
  return true;
}

wxIMPLEMENT_APP(WxBootstrapApp);