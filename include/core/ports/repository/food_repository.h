#ifndef DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_FOOD_REPOSITORY_H
#define DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_FOOD_REPOSITORY_H

#include <string>
#include <vector>

#include "core/domain/food.h"

class FoodRepository {
 public:
  virtual ~FoodRepository() = default;

  virtual const Food& GetFood(const std::string& food_name) const = 0;

  virtual bool AddFood(Food& food) = 0;

  virtual bool DelFood(const Food& food) = 0;

  virtual bool UpdateFood(const Food& food) = 0;

  virtual bool LoadAllFoods() = 0;

  virtual bool FillFoodsTable(const std::vector<Food>& foods) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_FOOD_REPOSITORY_H
