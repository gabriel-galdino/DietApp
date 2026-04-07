#include "fake_application.h"

#include <string>
#include <vector>

bool FakeApplication::Initialize() {
  return true;
}

bool FakeApplication::Shutdown() {
  return true;
}

bool FakeApplication::CreateUserWithMeal(const CreateUserWithMealDTO& data) {
  return add_meal_to_user_return_;
}

bool FakeApplication::AddMealToUser(const AddMealToUserDTO& data) {
  return true;
}

std::vector<MealDTO> FakeApplication::LoadMealsFromUser(
    const std::string& name) {
  return meals_;
}

bool FakeApplication::DoesUserExist(const std::string& username) {
  return does_user_exists_return_;
}

UserDTO FakeApplication::GetUserData(const std::string& username) {
  UserDTO user_data = {.username = username, .display_name = ""};
  return user_data;
}
