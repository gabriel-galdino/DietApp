#include "database/sqlite_meal_repository.h"

#include <memory>
#include <string>

#include "core/domain/meal.h"
#include "database/database_adapter.h"

SQLiteMealRepository::SQLiteMealRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

Meal& SQLiteMealRepository::GetMeal(const std::string& meal_name, int user_id) {
  std::string dummy_string;
  Meal dummy(dummy_string);
  return dummy;
}

bool SQLiteMealRepository::AddMeal(const Meal& meal, int user_id) {
  auto stmt = db_adapter_->Prepare(kInsertMealSql);
  stmt->Bind(":user_id", user_id);
  stmt->Bind(":name", meal.name());
  return stmt->Execute();
}

bool SQLiteMealRepository::DelMeal(const Meal& meal, int user_id) {
  return true;
}

bool SQLiteMealRepository::UpdateMeal(const Meal& meal, int user_id) {
  return true;
}