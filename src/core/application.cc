#include "core/application.h"

#include <memory>

#include <wx/filename.h>
#include <wx/wxsqlite3.h>

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

bool Application::AddMealToUser(const std::string& user_name,
                                const std::string& meal_name) {
  try {
    db_adapter_->BeginTransaction();

    user_repo_->AddUser(User(user_name));
    int user_id = db_adapter_->GetLastInsertId();

    meal_repo_->AddMeal(Meal(meal_name), user_id);

    db_adapter_->CommitTransaction();
    return true;
  } catch (const wxSQLite3Exception& e) {
    db_adapter_->RollbackTransaction();
  }
  return false;
}