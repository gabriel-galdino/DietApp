#include "core/application.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <wx/filename.h>
#include <wx/wxsqlite3.h>

#include "core/core_error_messages.h"
#include "core/domain/meal.h"
#include "core/domain/user.h"
#include "core/dto/food_dto.h"
#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "database/database_manager.h"
#include "database/sqlite_food_repository.h"
#include "database/sqlite_meal_repository.h"
#include "database/sqlite_user_repository.h"
#include "presentation/presentation.h"

Application::Application()
    : db_manager_(std::make_unique<DatabaseManager>()),
      db_adapter_(db_manager_->CreateAdapter()),
      presentation_(std::make_unique<Presentation>(this)),
      user_repo_(std::make_unique<SQLiteUserRepository>(db_adapter_)),
      meal_repo_(std::make_unique<SQLiteMealRepository>(db_adapter_)),
      food_repo_(std::make_unique<SQLiteFoodRepository>(db_adapter_)),
      xlsx_service_(std::make_unique<XlsxService>()) {}

Application::~Application() {
  Shutdown();
}

void Application::Initialize() {
  wxFileName workbook;
  wxFileName database;
  wxFileName xrc_resources;
  std::vector<Food> foods;
  std::vector<FoodDTO> data;
  const std::string worksheet_name = "CMVCol taco3";

  try {
    db_adapter_->Initialize(database);
  } catch (const wxSQLite3Exception& e) {
    throw std::runtime_error(kDatabaseStartupErrMsg +
                             e.GetMessage().utf8_string());
  }

  try {
    xlsx_service_->Initialize(workbook);
    xlsx_service_->LoadFoodDataFromSheet(worksheet_name, foods, data);
  } catch (const OpenXLSX::XLException& e) {
    throw std::runtime_error(kSpreadSheetStartupErrMsg + std::string(e.what()));
  } catch (const std::exception& e) {
    throw std::runtime_error(kSpreadSheetStartupErrMsg + std::string(e.what()));
  }

  try {
    user_repo_->LoadAllUsers();
    meal_repo_->LoadAllMeals();
  } catch (const wxSQLite3Exception& e) {
    throw std::runtime_error(kDatabaseStartupErrMsg +
                             e.GetMessage().utf8_string());
  }

  bool transaction_started = false;
  try {
    db_adapter_->BeginTransaction();
    transaction_started = true;
    food_repo_->FillFoodsTable(foods);
    db_adapter_->CommitTransaction();
    transaction_started = false;
  } catch (const wxSQLite3Exception& e) {
    if (transaction_started) {
      try {
        db_adapter_->RollbackTransaction();
      } catch (const wxSQLite3Exception& rollback_error) {
        throw std::runtime_error(kFillFoodsTableErrMsg +
                                 e.GetMessage().utf8_string() +
                                 "; falha no rollback: " +
                                 rollback_error.GetMessage().utf8_string());
      }
    }
    throw std::runtime_error(kFillFoodsTableErrMsg +
                             e.GetMessage().utf8_string());
  }

  try {
    presentation_->Initialize(xrc_resources, nullptr);
    presentation_->FillFoodChoices(data);
  } catch (const std::exception& e) {
    throw std::runtime_error(kPresentationStartupErrMsg + std::string(e.what()));
  }
}

void Application::Shutdown() {
  db_adapter_->Shutdown();
}

DatabaseManager* Application::GetDatabaseManager() const {
  return nullptr;
}

UserRepository* Application::GetUserRepository() const {
  return nullptr;
}

bool Application::ValidateUserCredentials(const std::string& username,
                                          const std::string& password) {
  return false;
}

bool Application::CreateUserWithMeal(const CreateUserWithMealDTO& data) {

  bool success = true;
  if (user_repo_->Exists(data.username)) {
    success = false;
    return success;
  }

  try {
    db_adapter_->BeginTransaction();

    User user(-1, data.username, data.display_name);
    user_repo_->AddUser(user);

    Meal meal(data.meal_name);
    meal_repo_->AddMeal(meal, user.id());

    db_adapter_->CommitTransaction();
  } catch (const wxSQLite3Exception& e) {
    success = false;
    db_adapter_->RollbackTransaction();
    std::cerr << e.GetMessage() << std::endl;
  }
  return success;
}

bool Application::AddMealToUser(const AddMealToUserDTO& data) {
  bool success = true;
  try {
    db_adapter_->BeginTransaction();

    const auto user = user_repo_->GetUser(data.username);
    Meal meal(data.meal_name);
    meal_repo_->AddMeal(meal, user.id());

    db_adapter_->CommitTransaction();
  } catch (const wxSQLite3Exception& e) {
    success = false;
    db_adapter_->RollbackTransaction();
    std::cerr << e.GetMessage() << std::endl;
  }
  return success;
}

std::vector<MealDTO> Application::LoadMealsFromUser(
    const std::string& username) {
  std::vector<MealDTO> meals_data;
  try {
    auto user = user_repo_->GetUser(username);
    auto meals = meal_repo_->GetMealsFromUser(user.id());
    MealDTO meal_data;
    for (const auto& meal : meals) {
      meal_data.name = meal.name();
      meal_data.user_id = meal.user_id();
      meals_data.push_back(meal_data);
    }
  } catch (const wxSQLite3Exception& e) {
    meals_data.clear();
    std::cerr << e.GetMessage() << std::endl;
  }
  return meals_data;
}

bool Application::DoesUserExist(const std::string& username) {
  return user_repo_->Exists(username);
}

UserDTO Application::GetUserData(const std::string& username) {
  const auto user = user_repo_->GetUser(username);
  UserDTO user_data = {.username = user.username(),
                       .display_name = user.display_name()};
  return user_data;
}

FoodDTO Application::GetFoodData(const std::string& food_name) {
  const auto food = food_repo_->GetFood(food_name);
  FoodDTO food_data = {.id = food.id(),
                       .name = food.name(),
                       .display_name = food.display_name(),
                       .proteins_per_100g = food.proteins_per_100g(),
                       .carbs_per_100g = food.carbs_per_100g(),
                       .fats_per_100g = food.fats_per_100g()};
  return food_data;
}
