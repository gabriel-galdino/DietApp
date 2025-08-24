#ifndef DIETAPP_INCLUDE_PRESENTATION_MY_APP_H
#define DIETAPP_INCLUDE_PRESENTATION_MY_APP_H

#include <memory>

#include <wx/wx.h>

#include "core/application.h"

class MyApp : public wxApp {
 public:
  bool OnInit() override;

 private:
  std::unique_ptr<Application> app_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_MY_APP_H
