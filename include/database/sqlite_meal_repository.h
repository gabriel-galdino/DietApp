#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H

#include <memory>
#include <set>
#include <string>

#include "core/domain/meal.h"
#include "core/ports/repository/meal_repository.h"
#include "database_adapter.h"

constexpr const char* kInsertMealSql = R"sql(
  INSERT INTO meals(user_id, name)
  VALUES (:user_id, :name)
)sql";

class SQLiteMealRepository : public MealRepository {
 public:
  SQLiteMealRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  Meal& GetMeal(const std::string& meal_name, int user_id) override;

  bool AddMeal(const Meal& meal, int user_id) override;

  bool DelMeal(const Meal& meal, int user_id) override;

  bool UpdateMeal(const Meal& meal, int user_id) override;

 private:
  std::set<Meal> meals_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_MEAL_REPOSITORY_H
