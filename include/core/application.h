#ifndef DIETAPP_INCLUDE_CORE_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_APPLICATION_H

#include <memory>
#include <string>
#include <vector>

#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "core/i_application.h"

// Forward declarations
class DatabaseAdapter;
class DatabaseManager;
class UserRepository;
class MealRepository;
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

  bool IsTestsMode() const override { return false; }

  bool CreateUserWithMeal(const CreateUserWithMealDTO& data) override;

  bool AddMealToUser(const AddMealToUserDTO& data) override;

  std::vector<MealDTO> LoadMealsFromUser(const std::string& username) override;

  bool DoesUserExist(const std::string& username) override;

  UserDTO GetUserData(const std::string& username) override;

 private:
  std::unique_ptr<DatabaseManager> db_manager_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
  std::unique_ptr<Presentation> presentation_;
  std::unique_ptr<UserRepository> user_repo_;
  std::unique_ptr<MealRepository> meal_repo_;
};

#endif  // DIETAPP_INCLUDE_CORE_APPLICATION_H
