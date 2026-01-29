#include "core/domain/meal.h"

Meal::Meal(const std::string& meal_name) : id_(-1), name_(meal_name) {}

Meal::Meal(int id, const std::string& meal_name) : id_(id), name_(meal_name) {}