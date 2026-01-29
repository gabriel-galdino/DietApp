#include "fake_application.h"

#include <string>
#include <vector>

#include "core/domain/meal.h"

bool FakeApplication::AddMealToUser(const std::string&, const std::string&) {
  return success_;
}

std::vector<Meal> FakeApplication::LoadMealsFromUser(const std::string& name) {
  return std::vector<Meal>();
}
