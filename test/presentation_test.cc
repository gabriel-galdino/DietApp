#define CATCH_CONFIG_RUNNER
#include "presentation/presentation.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/application.h"

class PresentationTest : public wxApp {
 public:
  bool OnInit() override {
    std::vector<const char*> cmd_line_args;
    for (int i = 0; i < argc; ++i) {
      cmd_line_args.push_back(wxString(argv[i]).utf8_string().c_str());
    }
    int result =
        Catch::Session().run(cmd_line_args.size(), cmd_line_args.data());
    Exit();
    return false;
  }
};

wxIMPLEMENT_APP(PresentationTest);

TEST_CASE("Presentation", "[Initialize]") {
  wxFileName xrc_resources(wxStandardPaths::Get().GetExecutablePath(), "");
  xrc_resources.RemoveLastDir();
  xrc_resources.RemoveLastDir();
  xrc_resources.AppendDir("share");
  xrc_resources.AppendDir("dietapp");
  xrc_resources.AppendDir("xrc");
  xrc_resources.SetFullName("resource.xrc");

  Presentation pres(nullptr);
  REQUIRE(pres.Initialize(xrc_resources));

  SECTION("Initialize") {
    wxFrame* initial_frame =
        wxXmlResource::Get()->LoadFrame(nullptr, "Initial");
    wxFrame* register_frame =
        wxXmlResource::Get()->LoadFrame(nullptr, "Register");
    wxFrame* create_frame = wxXmlResource::Get()->LoadFrame(nullptr, "Create");
    REQUIRE(initial_frame != nullptr);
    REQUIRE(register_frame != nullptr);
    REQUIRE(create_frame != nullptr);
  }
}