#ifndef DIETAPP_INCLUDE_CORE_I_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_I_APPLICATION_H

#include <string>
#include <vector>

#include "core/dto/meal_dto.h"
#include "core/dto/user_dto.h"

class IApplication {
 public:
  virtual ~IApplication() = default;

  virtual bool Initialize() = 0;

  virtual bool Shutdown() = 0;

  virtual bool IsTestsMode() const = 0;

  virtual bool CreateUserWithMeal(const CreateUserWithMealDTO& data) = 0;

  virtual bool AddMealToUser(const AddMealToUserDTO& data) = 0;

  virtual std::vector<MealDTO> LoadMealsFromUser(
      const std::string& username) = 0;

  virtual bool DoesUserExist(const std::string& username) = 0;

  virtual UserDTO GetUserData(const std::string& username) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_I_APPLICATION_H
