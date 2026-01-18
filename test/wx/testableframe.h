///////////////////////////////////////////////////////////////////////////////
// Name:        testableframe.h
// Purpose:     An improved wxFrame for unit-testing
// Author:      Steven Lamerton
// Copyright:   (c) 2010 Steven Lamerton
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/event.h"
#include "wx/frame.h"
#include "wx/hashmap.h"

class wxTestableFrame : public wxFrame {
  wxDECLARE_DYNAMIC_CLASS(wxTestableFrame);

 public:
  wxTestableFrame();

  void OnEvent(wxEvent& evt);
  int GetEventCount(wxEventType type);

 private:
  friend class EventCounter;

  void ClearEventCount(wxEventType type);

  wxLongToLongHashMap m_count;
};

class EventCounter {
 public:
  EventCounter(wxWindow* win, wxEventType type);
  ~EventCounter();

  int GetCount() { return m_frame->GetEventCount(m_type); }
  void Clear() { m_frame->ClearEventCount(m_type); }

  // Sometimes we need to yield a few times before getting the event we
  // expect, so provide a function waiting for the expected event for up to
  // the given number of milliseconds (supposed to be divisible by 50).
  //
  // Return true if we did receive the event or false otherwise.
  bool WaitEvent(int timeInMs = 1000);

 private:
  wxEventType m_type;
  wxTestableFrame* m_frame;
  wxWindow* m_win;
};
