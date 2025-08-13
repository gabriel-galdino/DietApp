#include "my_app.h"
#include "my_frame.h"

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show();
    return true;
}

// This defines the equivalent of main() for the current platform.
wxIMPLEMENT_APP(MyApp);