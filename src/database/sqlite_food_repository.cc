#include "database/sqlite_food_repository.h"

#include <memory>
#include <string>

#include "core/domain/food.h"
#include "database/database_adapter.h"

SQLiteFoodRepository::SQLiteFoodRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

const Food& SQLiteFoodRepository::GetFood(const std::string& food_name) const {
  auto it = FindByName(food_name);
  return *it;
}

bool SQLiteFoodRepository::AddFood(Food& food) {
  return false;
}

bool SQLiteFoodRepository::DelFood(const Food& food) {
  return true;
}

bool SQLiteFoodRepository::UpdateFood(const Food& food) {
  return true;
}

bool SQLiteFoodRepository::LoadAllFoods() {
  return false;
}

bool SQLiteFoodRepository::FillFoodsTable(const std::vector<Food>& foods) {
  return {};
}