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

std::vector<FoodDTO> SQLiteMealRepository::GetFoodsForMeal(int meal_id) {
  std::vector<FoodDTO> foods;
  auto stmt = db_adapter_->Prepare(
      "SELECT f.id, f.name, f.display_name, f.proteins_per_100g, "
      "f.carbs_per_100g, f.fats_per_100g, mf.quantity_grams "
      "FROM foods f "
      "JOIN meal_foods mf ON f.id = mf.food_id "
      "WHERE mf.meal_id = :meal_id");
  stmt->Bind(":meal_id", meal_id);
  auto result_set = stmt->ExecuteQuery();
  while (result_set->NextRow()) {
    FoodDTO food;
    food.id = result_set->GetInt("id");
    food.name = result_set->GetString("name");
    food.display_name = result_set->GetString("display_name");
    food.proteins_per_100g = result_set->GetDouble("proteins_per_100g");
    food.carbs_per_100g = result_set->GetDouble("carbs_per_100g");
    food.fats_per_100g = result_set->GetDouble("fats_per_100g");
    food.quantity = result_set->GetDouble("quantity_grams");
    foods.push_back(food);
  }
  return foods;
}

bool SQLiteMealRepository::SaveMealFoods(const MealDTO& meal) {
  auto del_stmt = db_adapter_->Prepare(kDeleteMealFoodsEntry);
  del_stmt->Bind(":meal_id", meal.id);
  del_stmt->Execute();

  for (const auto& food : meal.foods) {
    auto ins_stmt = db_adapter_->Prepare(kInsertMealFoodsEntry);
    ins_stmt->Bind(":meal_id", meal.id);
    ins_stmt->Bind(":food_id", food.id);
    ins_stmt->Bind(":quantity_grams", food.quantity);
    ins_stmt->Bind(":calories", 0);
    if (ins_stmt->Execute() == false) {
      return false;
    }
  }
  return true;
}
