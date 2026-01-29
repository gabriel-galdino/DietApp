#include "database/sqlite_meal_repository.h"

#include <memory>
#include <string>
#include <vector>

#include "core/domain/meal.h"
#include "database/database_adapter.h"

SQLiteMealRepository::SQLiteMealRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

const Meal& SQLiteMealRepository::GetMeal(const std::string& meal_name,
                                          int user_id) const {
  auto it = FindByName(meal_name, user_id);
  return *it;
}

std::vector<Meal> SQLiteMealRepository::GetMealsFromUser(int user_id) {
  std::vector<Meal> meals;
  for (const auto& meal : meals_) {
    if (meal.user_id() != user_id) {
      continue;
    }
    meals.push_back(meal);
  }
  return meals;
}

bool SQLiteMealRepository::AddMeal(Meal& meal, int user_id) {
  auto stmt = db_adapter_->Prepare(kInsertMealSql);
  stmt->Bind(":user_id", user_id);
  stmt->Bind(":name", meal.name());
  bool success = stmt->Execute();
  if (success == false) {
    return false;
  }
  meal.set_id(db_adapter_->GetLastInsertId());
  meal.set_user_id(user_id);
  meals_.insert(meal);
  return success;
}

bool SQLiteMealRepository::DelMeal(const Meal& meal, int user_id) {
  return true;
}

bool SQLiteMealRepository::UpdateMeal(const Meal& meal, int user_id) {
  return true;
}

bool SQLiteMealRepository::LoadAllMeals() {
  auto stmt = db_adapter_->Prepare(kLoadAllMeals);
  auto result_set = stmt->ExecuteQuery();
  while (result_set->NextRow()) {
    Meal meal(result_set->GetInt("id"), result_set->GetString("name"));
    meal.set_user_id(result_set->GetInt("user_id"));
    meals_.insert(meal);
  }
  return true;
}