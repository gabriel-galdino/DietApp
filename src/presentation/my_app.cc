#include "presentation/my_app.h"
#include "presentation/my_frame.h"

bool MyApp::OnInit() {
  app_ = std::unique_ptr<Application>(new Application());
  MyFrame* frame = new MyFrame();
  frame->Show();
  return true;
}

// This defines the equivalent of main() for the current platform.
wxIMPLEMENT_APP(MyApp);