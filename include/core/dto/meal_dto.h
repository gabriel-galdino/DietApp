#ifndef DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H
#define DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H

#include <string>
#include <vector>

#include "core/dto/food_dto.h"

struct MealDTO {
  int id;
  std::string name;
  std::string username;
  std::vector<FoodDTO> foods;
};

struct MealsFromUserDTO {
  std::vector<MealDTO> meals;
  std::string username;

};

#endif // DIETAPP_INCLUDE_CORE_DTO_MEAL_DTO_H
