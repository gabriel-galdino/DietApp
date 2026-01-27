#include "core/application.h"

#include <memory>

#include <wx/filename.h>

#include "core/ports/repository/user_repository.h"
#include "database/database_adapter.h"
#include "database/database_manager.h"
#include "presentation/presentation.h"

Application::Application()
    : db_manager_(std::make_unique<DatabaseManager>()),
      db_adapter_(db_manager_->CreateAdapter()),
      presentation_(std::make_unique<Presentation>(this)) {
  /*TODO: Fix DB initialization */
  db_adapter_->Initialize("../schema-model/schema.sql");
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

bool Application::AddMealToUser(const std::string& name,
                                const std::string& meal) {
  printf("Add Meal To User\n");
  return true;
}