#include "core/application.h"

#include "database/database_manager.h"
#include "database/user_repository.h"

Application::Application() {
  db_manager_ = std::unique_ptr<DatabaseManager>(new DatabaseManager());
}

Application::~Application() {
  Shutdown();
}

bool Application::InitializeDatabase(const std::string& databasePath) {
  // try {
  //     db_manager_ = std::make_unique<DatabaseManager>();
  //     if (!db_manager_->Connect(databasePath)) {
  //         return false;
  //     }
  //
  //     user_repo_ = std::make_unique<UserRepository>(db_manager_.get());
  //
  //     return db_manager_->verifyDatabaseSchema();
  //
  // } catch (const std::exception& e) {
  //     wxLogError("Erro na inicialização do database: %s", e.what());
  //     return false;
  // }
  return false;
}

bool Application::InitializeApplicationSettings() {
  return false;
}

void Application::Shutdown() {
  // user_repo_.reset();
  // db_manager_.reset();
}

DatabaseManager* Application::GetDatabaseManager() const {
  // return db_manager_.get();
  return nullptr;
}

UserRepository* Application::GetUserRepository() const {
  // return user_repo_.get();
  return nullptr;
}

bool Application::ValidateUserCredentials(const std::string& username,
                                          const std::string& password) {
  // if (!user_repo_) return false;
  // return user_repo_->ValidateCredentials(username, password);
  return false;
}