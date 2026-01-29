#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H

#include <string>

class Meal {
 public:
  Meal(int id, const std::string& meal_name);

  Meal(const std::string& meal_name);

  void set_id(int id) { id_ = id; }

  int id() const { return id_; }

  int user_id() const { return user_id_; }

  void set_user_id(int user_id) { user_id_ = user_id; }

  std::string name() const { return name_; }

  bool operator==(const Meal& other) const { return id_ == other.id_; }

  bool operator<(const Meal& other) const { return id_ < other.id_; }

 private:
  int id_{-1};
  std::string name_;
  int user_id_{-1};
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_MEAL_H
