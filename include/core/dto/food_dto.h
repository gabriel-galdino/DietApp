#ifndef DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H
#define DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H

#include <string>

struct FoodDTO {
  int id;
  std::string name;
  std::string display_name;
  double protein_per_100g;
  double fat_per_100g;
  double carb_per_100g;
};

#endif // DIETAPP_INCLUDE_CORE_DTO_FOOD_DTO_H
