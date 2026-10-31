#ifndef DIETAPP_INCLUDE_TEST_APPLICATION_H
#define DIETAPP_INCLUDE_TEST_APPLICATION_H

#include <string>
#include <vector>

#include "core/dto/food_dto.h"
#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"
#include "core/i_application.h"

class FakeApplication : public IApplication {
 public:
  void Initialize() override;

  void Shutdown() override;

  bool IsTestsMode() const override { return true; }

  bool CreateUserWithMeal(const CreateUserWithMealDTO& data) override;

  bool AddMealToUser(const AddMealToUserDTO& data) override;

  bool LoadMealsFromUser(const std::string& name,
                         MealsFromUserDTO& meals_from_user_data) override;

  bool DoesUserExist(const std::string& username) override;

  UserDTO GetUserData(const std::string& username) override;

  FoodDTO GetFoodData(const std::string& food_data) override;

  bool SaveMealFoods(const MealDTO& meal) override { return true; }

  void AddMealToUserWillReturn(bool success) {
    add_meal_to_user_return_ = success;
  }

  void CreateUserWithMealWillReturn(bool success) {
    create_user_with_meal_return_ = success;
  }

  void LoadMealsFromUserWillReturn(bool success) {
    load_meals_from_user_return_ = success;
  }

  void LoadMealsFromUserWillBeSetWith(MealsFromUserDTO &meals_from_user_data) {
    fake_meals_from_user_data_ = meals_from_user_data;
  }

  void DoesUserExistsWillReturn(bool success) {
    does_user_exists_return_ = success;
  }

  void GetUserDataWillReturn(const UserDTO& user_data) {
    user_data_ = user_data;
  }

  void GetFoodDataWillReturn(const FoodDTO& food_data) {
    food_data_ = food_data;
  }

 private:
  bool create_user_with_meal_return_ = true;
  bool add_meal_to_user_return_ = true;
  bool does_user_exists_return_ = false;
  bool load_meals_from_user_return_ = true;
  std::vector<MealDTO> meals_;
  UserDTO user_data_;
  FoodDTO food_data_{};
  MealsFromUserDTO fake_meals_from_user_data_;
};

#endif  // DIETAPP_INCLUDE_TEST_APPLICATION_H
