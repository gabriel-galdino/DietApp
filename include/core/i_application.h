#ifndef DIETAPP_INCLUDE_CORE_I_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_I_APPLICATION_H

#include <string>
#include <vector>

#include "core/domain/meal.h"

class IApplication {
 public:
  virtual ~IApplication() = default;
  virtual bool IsTestsMode() const = 0;
  virtual bool AddMealToUser(const std::string& user_name,
                             const std::string& meal_name) = 0;
  virtual std::vector<Meal> LoadMealsFromUser(const std::string& user_name) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_I_APPLICATION_H
