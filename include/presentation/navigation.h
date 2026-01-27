#ifndef DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H
#define DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H

enum class PageId { Initial, Register, Create };

class INavigation {
 public:
  virtual ~INavigation() = default;
  virtual void NavigateTo(PageId page) = 0;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H
