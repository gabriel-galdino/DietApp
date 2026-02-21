#ifndef DIETAPP_INCLUDE_TEST_APPLICATION_H
#define DIETAPP_INCLUDE_TEST_APPLICATION_H

#include <string>
#include <vector>

#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "core/i_application.h"

class FakeApplication : public IApplication {
 public:
  FakeApplication() = default;

  ~FakeApplication() = default;

  bool IsTestsMode() const override { return true; }

  bool CreateUserWithMeal(const CreateUserWithMealDTO& data) override;

  bool AddMealToUser(const AddMealToUserDTO& data) override;

  std::vector<MealDTO> LoadMealsFromUser(const std::string& name) override;

  bool DoesUserExist(const std::string& username) override;

  UserDTO GetUserData(const std::string& username) override;

  void AddMealToUserWillReturn(bool success) {
    add_meal_to_user_return_ = success;
  }

  void LoadMealsFromUserWillReturn(const MealDTO& meal_data) {
    meals_.push_back(meal_data);
  }

  void DoesUserExistsWillReturn(bool success) {
    does_user_exists_return_ = success;
  }

 private:
  bool add_meal_to_user_return_ = true;
  bool does_user_exists_return_ = false;
  std::vector<MealDTO> meals_;
};

#endif  // DIETAPP_INCLUDE_TEST_APPLICATION_H
