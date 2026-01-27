#ifndef DIETAPP_INCLUDE_CORE_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_APPLICATION_H

#include <memory>
#include <string>

#include "core/i_application.h"

// Forward declarations
class DatabaseAdapter;
class DatabaseManager;
class UserRepository;
class Presentation;

class Application : public IApplication {
 public:
  Application();

  ~Application();

  bool InitializeDatabase(const std::string& databasePath);

  bool InitializePresentation(const std::string& databasePath);

  bool InitializeApplicationSettings();

  void Shutdown();

  DatabaseManager* GetDatabaseManager() const;

  UserRepository* GetUserRepository() const;

  bool ValidateUserCredentials(const std::string& username,
                               const std::string& password);

  bool AddMealToUser(const std::string& name, const std::string& meal) override;

 private:
  std::unique_ptr<DatabaseManager> db_manager_;
  std::unique_ptr<DatabaseAdapter> db_adapter_;
  std::unique_ptr<Presentation> presentation_;
  std::unique_ptr<UserRepository> user_repo_;
};

#endif  // DIETAPP_INCLUDE_CORE_APPLICATION_H
