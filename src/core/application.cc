#include "core/application.h"

#include <memory>
#include <vector>

#include <wx/filename.h>
#include <wx/wxsqlite3.h>

#include "core/domain/meal.h"
#include "core/domain/user.h"
#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "core/ports/repository/meal_repository.h"
#include "core/ports/repository/user_repository.h"
#include "database/database_manager.h"
#include "database/sqlite_meal_repository.h"
#include "database/sqlite_user_repository.h"
#include "presentation/presentation.h"

Application::Application()
    : db_manager_(std::make_unique<DatabaseManager>()),
      db_adapter_(db_manager_->CreateAdapter()),
      presentation_(std::make_unique<Presentation>(this)),
      user_repo_(std::make_unique<SQLiteUserRepository>(db_adapter_)),
      meal_repo_(std::make_unique<SQLiteMealRepository>(db_adapter_)) {
  wxFileName database;
  db_adapter_->Initialize(database);
  wxFileName xrc_resources;
  presentation_->Initialize(xrc_resources, nullptr);
  user_repo_->LoadAllUsers();
  meal_repo_->LoadAllMeals();
}

Application::~Application() {
  Shutdown();
  db_adapter_->Shutdown();
}

bool Application::InitializeDatabase(const std::string& databasePath) {
  return false;
}

bool Application::InitializeApplicationSettings() {
  return false;
}

void Application::Shutdown() {}

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

  if (user_repo_->Exists(data.username)) {
    return false;
  }

  try {
    db_adapter_->BeginTransaction();

    User user(-1, data.username, data.display_name);
    user_repo_->AddUser(user);

    Meal meal(data.meal_name);
    meal_repo_->AddMeal(meal, user.id());

    db_adapter_->CommitTransaction();
  } catch (const wxSQLite3Exception& e) {
    db_adapter_->RollbackTransaction();
    return false;
  }
  return true;
}

bool Application::AddMealToUser(const AddMealToUserDTO& data) {
  try {
    db_adapter_->BeginTransaction();

    const auto user = user_repo_->GetUser(data.username);
    Meal meal(data.meal_name);
    meal_repo_->AddMeal(meal, user.id());

    db_adapter_->CommitTransaction();
  } catch (const wxSQLite3Exception& e) {
    db_adapter_->RollbackTransaction();
    return false;
  }
  return true;
}

std::vector<MealDTO> Application::LoadMealsFromUser(
    const std::string& username) {
  try {
    auto user = user_repo_->GetUser(username);
    auto meals = meal_repo_->GetMealsFromUser(user.id());
    std::vector<MealDTO> meals_data;
    MealDTO meal_data;
    for (const auto& meal : meals) {
      meal_data.name = meal.name();
      meal_data.user_id = meal.user_id();
      meals_data.push_back(meal_data);
    }
    return meals_data;
  } catch (const wxSQLite3Exception& e) {
    void();
  }
  return {};
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