#ifndef DIETAPP_INCLUDE_PRESENTATION_MY_FRAME_H
#define DIETAPP_INCLUDE_PRESENTATION_MY_FRAME_H

#include <wx/wx.h>

enum { ID_Hello = 1 };

class MyFrame : public wxFrame {
 public:
  MyFrame();

 private:
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_MY_FRAME_H
