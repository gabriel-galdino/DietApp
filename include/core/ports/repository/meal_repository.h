#ifndef DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H
#define DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H

#include <string>
#include <vector>

#include "core/domain/meal.h"
#include "core/dto/food_dto.h"
#include "core/dto/meal_dto.h"

class MealRepository {
 public:
  virtual ~MealRepository() = default;

  virtual const Meal& GetMeal(const std::string& meal_name,
                              int user_id) const = 0;

  virtual std::vector<Meal> GetMealsFromUser(int user_id) = 0;

  virtual bool AddMeal(Meal& meal, int user_id) = 0;

  virtual bool DelMeal(const Meal& meal, int user_id) = 0;

  virtual bool UpdateMeal(const Meal& meal, int user_id) = 0;

  virtual bool LoadAllMeals() = 0;

  virtual std::vector<FoodDTO> GetFoodsForMeal(int meal_id) = 0;

  virtual bool SaveMealFoods(const MealDTO& meal) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_MEAL_REPOSITORY_H
