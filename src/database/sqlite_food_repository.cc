#include "database/sqlite_food_repository.h"

#include <iostream>
#include <memory>
#include <string>

#include <wx/wxsqlite3.h>

#include "core/domain/food.h"
#include "database/database_adapter.h"

SQLiteFoodRepository::SQLiteFoodRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

const Food SQLiteFoodRepository::GetFood(
    const std::string& display_name) const {
  Food food;
  try {
    auto stmt = db_adapter_->Prepare(kSelectFoodSql);
    stmt->Bind(":display_name", display_name);
    auto result_set = stmt->ExecuteQuery();
    while (result_set->NextRow()) {
      food.set_id(result_set->GetInt("id"));
      food.set_name(result_set->GetString("name"));
      food.set_display_name(display_name);
      food.set_proteins_per_100g(result_set->GetDouble("proteins_per_100g"));
      food.set_carbs_per_100g(result_set->GetDouble("carbs_per_100g"));
      food.set_fats_per_100g(result_set->GetDouble("fats_per_100g"));
    }
  } catch (const wxSQLite3Exception& e) {
    std::cerr << e.GetMessage() << std::endl;
  }
  return food;
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
  auto check_stmt = db_adapter_->Prepare(kCheckTableSql);
  auto result = check_stmt->ExecuteQuery();
  if (result->NextRow()) {
    return true;
  }

  bool global_success = false;
  for (const auto& food : foods) {
    bool local_success = false;
    auto stmt = db_adapter_->Prepare(kInsertFoodSql);
    stmt->Bind(":name", food.name());
    stmt->Bind(":display_name", food.display_name());
    stmt->Bind(":proteins_per_100g", food.proteins_per_100g());
    stmt->Bind(":carbs_per_100g", food.carbs_per_100g());
    stmt->Bind(":fats_per_100g", food.fats_per_100g());
    local_success = stmt->Execute();
    if (local_success == false) {
      global_success = false;
      continue;
    }
  }
  return global_success;
}