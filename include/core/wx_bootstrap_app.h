#ifndef DIETAPP_INCLUDE_CORE_WX_BOOTSTRAP_APP_H
#define DIETAPP_INCLUDE_CORE_WX_BOOTSTRAP_APP_H

#include <wx/wx.h>

#include "core/i_application.h"

class WxBootstrapApp : public wxApp {
 public:
  WxBootstrapApp() = default;

  ~WxBootstrapApp() = default;

  bool OnInit() override;

 private:
  IApplication* app_{nullptr};
};

#endif  // DIETAPP_INCLUDE_CORE_WX_BOOTSTRAP_APP_H
