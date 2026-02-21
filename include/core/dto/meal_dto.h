#ifndef DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H
#define DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H

#include <string>
#include <vector>

struct MealDTO {
  std::string name;
  int user_id;
};

struct MealsFromUserDTO {
  std::vector<MealDTO> meals;
  int selected_meal;
  int user_id;
};

#endif // DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H
