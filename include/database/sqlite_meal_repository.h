#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H

#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core/domain/meal.h"
#include "core/ports/repository/meal_repository.h"
#include "database_adapter.h"

constexpr const char* kInsertMealSql = R"sql(
  INSERT INTO meals(user_id, name)
  VALUES (:user_id, :name)
)sql";

constexpr const char* kLoadAllMeals = R"sql(
  SELECT * FROM meals
)sql";

constexpr const char* kGetMealsFromUser = R"sql(
  SELECT name
  FROM meals
  WHERE user_id=(:user_id)
)sql";

class SQLiteMealRepository : public MealRepository {
 public:
  SQLiteMealRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  const Meal& GetMeal(const std::string& meal_name, int user_id) const override;

  std::vector<Meal> GetMealsFromUser(int user_id) override;

  bool AddMeal(Meal& meal, int user_id) override;

  bool DelMeal(const Meal& meal, int user_id) override;

  bool UpdateMeal(const Meal& meal, int user_id) override;

  bool LoadAllMeals() override;

 private:
  auto FindByName(const std::string& meal_name, int user_id) const {
    return std::find_if(meals_.begin(), meals_.end(),
                        [&meal_name, user_id](const Meal& meal) {  //
                          return meal.name() == meal_name &&       //
                                 meal.user_id() == user_id;        //
                        });
  }

  std::set<Meal> meals_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H
