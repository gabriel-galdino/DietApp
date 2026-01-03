#ifndef DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
#define DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H

#include <wx/filename.h>
#include <wx/wx.h>

#include "core/application.h"

class Presentation {
 public:
  Presentation(Application* app);

  bool Initialize(wxFileName& xrc_resources);

 private:
  Application* app_;
  wxFrame initial_frame_;
  wxFrame register_frame_;
  wxFrame create_frame_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_PRESENTATION_H
