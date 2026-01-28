#ifndef DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H
#define DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H

#include <string>

#include "core/domain/meal.h"

class MealRepository {
 public:
  virtual ~MealRepository() = default;

  virtual Meal& GetMeal(const std::string& meal_name, int user_id) = 0;
  virtual bool AddMeal(const Meal& meal, int user_id) = 0;
  virtual bool DelMeal(const Meal& meal, int user_id) = 0;
  virtual bool UpdateMeal(const Meal& meal, int user_id) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H
