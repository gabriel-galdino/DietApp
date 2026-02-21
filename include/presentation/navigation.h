#ifndef DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H
#define DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H

#include <vector>

#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"

enum class PageId { Initial, Enter, Register, Create };

class INavigation {
 public:
  virtual ~INavigation() = default;
  virtual void NavigateTo(PageId page) = 0;
  virtual void NavigateToCreatePageWithMeals(const MealsFromUserDTO& data) = 0;
  virtual void NavigateToRegisterPageWithNewUser() = 0;
  virtual void NavigateToRegisterPageWithExistingUser(
      const UserDTO& user_data) = 0;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_NAVIGATION_H
