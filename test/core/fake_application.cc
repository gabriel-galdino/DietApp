#include "fake_application.h"

#include <string>
#include <vector>

void FakeApplication::Initialize() {}

void FakeApplication::Shutdown() {}

bool FakeApplication::CreateUserWithMeal(const CreateUserWithMealDTO& data) {
  return create_user_with_meal_return_;
}

bool FakeApplication::AddMealToUser(const AddMealToUserDTO& data) {
  return add_meal_to_user_return_;
}

std::vector<MealDTO> FakeApplication::LoadMealsFromUser(
    const std::string& name) {
  return meals_;
}

bool FakeApplication::DoesUserExist(const std::string& username) {
  return does_user_exists_return_;
}

UserDTO FakeApplication::GetUserData(const std::string& username) {
  UserDTO user_data = user_data_;
  if (user_data.username.empty()) {
    user_data.username = username;
  }
  return user_data;
}

FoodDTO FakeApplication::GetFoodData(const std::string& food_name) {
  FoodDTO food_data = food_data_;
  if (food_data.id == 0) {
    food_data.id = 1;
  }
  if (food_data.name.empty()) {
    food_data.name = food_name;
  }
  if (food_data.display_name.empty()) {
    food_data.display_name = food_name;
  }
  return food_data;
}
