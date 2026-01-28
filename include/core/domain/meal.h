#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H

#include <string>

class Meal {
 public:
  Meal(const std::string& meal_name);

  virtual ~Meal() = default;

  std::string name() const { return name_; }

  bool operator==(const Meal& other) const { return name_ == other.name_; }

 private:
  std::string name_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H
