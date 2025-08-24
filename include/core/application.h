#ifndef DIETAPP_INCLUDE_CORE_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_APPLICATION_H

#include <memory>
#include <string>

// Forward declarations
class DatabaseManager;
class UserRepository;

class Application {
 public:
  Application();
  ~Application();
  bool InitializeDatabase(const std::string& databasePath);
  bool InitializeApplicationSettings();
  void Shutdown();

  DatabaseManager* GetDatabaseManager() const;
  UserRepository* GetUserRepository() const;

  bool ValidateUserCredentials(const std::string& username,
                               const std::string& password);

 private:
  std::unique_ptr<DatabaseManager> db_manager_;
  std::unique_ptr<UserRepository> user_repo_;
};

#endif  // DIETAPP_INCLUDE_CORE_APPLICATION_H
