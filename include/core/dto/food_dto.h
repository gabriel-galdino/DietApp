#ifndef DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H
#define DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H

#include <string>

struct FoodDTO {
  int id;
  std::string name;
  std::string display_name;
  double proteins_per_100g;
  double carbs_per_100g;
  double fats_per_100g;
  double quantity = 100.0;
};

#endif // DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H
