#ifndef DIETAPP_INCLUDE_CORE_DTO_USER_DTO_H
#define DIETAPP_INCLUDE_CORE_DTO_USER_DTO_H

#include <string>

struct CreateUserWithMealDTO {
  std::string username;
  std::string display_name;
  std::string meal_name;
};

struct AddMealToUserDTO {
  std::string username;
  std::string meal_name;
};

struct UserDTO {
  std::string username;
  std::string display_name;
};

#endif // DIETAPP_INCLUDE_CORE_DTO_USER_DTO_H
