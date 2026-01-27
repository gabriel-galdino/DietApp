#define CATCH_CONFIG_RUNNER
#include "presentation/presentation.h"

#include <wx/app.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/uiaction.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/fake_application.h"
#include "wx/testableframe.h"

class PresentationTest : public wxApp {
 public:
  PresentationTest() {}

  virtual bool OnInit() wxOVERRIDE {
    std::vector<const char*> cmd_line_args;
    for (int i = 0; i < argc; ++i) {
      cmd_line_args.push_back(wxString(argv[i]).utf8_string().c_str());
    }
    int result =
        Catch::Session().run(cmd_line_args.size(), cmd_line_args.data());
    return false;
  }
};

wxIMPLEMENT_APP(PresentationTest);

#if wxUSE_UIACTIONSIMULATOR

TEST_CASE("Presentation", "[UI Flow]") {
  wxXmlResource::Get()->InitAllHandlers();
  wxFileName xrc_resources(wxStandardPaths::Get().GetExecutablePath(), "");
  xrc_resources.RemoveLastDir();
  xrc_resources.RemoveLastDir();
  xrc_resources.AppendDir("share");
  xrc_resources.AppendDir("dietapp");
  xrc_resources.AppendDir("xrc");
  xrc_resources.SetFullName("test_resource.xrc");
  wxXmlResource::Get()->Load(xrc_resources.GetFullPath());

  IApplication* app = new FakeApplication();
  Presentation pres(app);
  wxTestableFrame* test_frame = wxDynamicCast(
      wxXmlResource::Get()->LoadFrame(nullptr, "MainFrame"), wxTestableFrame);
  wxTheApp->SetTopWindow(test_frame);
  // Use fixed position to facilitate debugging.
  test_frame->Move(200, 200);
  REQUIRE(pres.Initialize(xrc_resources, test_frame));

  SECTION("Initial Page") {
    REQUIRE(pres.GetBook()->GetSelection() == 0);

    EventCounter clicked(pres.GetInitialPage()->GetRegisterButton(),
                         wxEVT_BUTTON);

    wxUIActionSimulator sim;
    wxYield();

    //We move in slightly to account for window decorations, we need to yield
    //after every wxUIActionSimulator action to keep everything working in GTK
    sim.MouseMove(
        pres.GetInitialPage()->GetRegisterButton()->GetScreenPosition() +
        wxPoint(10, 10));
    wxYield();

    sim.MouseClick();
    wxYield();

    CHECK(clicked.GetCount() == 1);
    REQUIRE(pres.GetBook()->GetSelection() == 1);
  }
}

#endif  // wxUSE_UIACTIONSIMULATOR
