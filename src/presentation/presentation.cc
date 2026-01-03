#include <wx/stdpaths.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "presentation/presentation.h"

Presentation::Presentation(Application* app) : app_(app) {}

bool Presentation::Initialize(wxFileName& xrc_resources) {

  wxXmlResource::Get()->InitAllHandlers();
  if (xrc_resources.IsOk()) {
    wxXmlResource::Get()->Load(xrc_resources.GetFullPath());
  } else {
    wxStandardPaths& paths = wxStandardPaths::Get();
    xrc_resources = wxFileName(paths.GetDataDir(), "");
    xrc_resources.AppendDir("xrc");
    xrc_resources.SetFullName("resource.xrc");
    wxXmlResource::Get()->Load(xrc_resources.GetFullPath());
  }

  wxFrame* frame = wxXmlResource::Get()->LoadFrame(nullptr, "Initial");
  if (frame == nullptr) {
    return false;
  }
  frame->Show();
  return true;
}