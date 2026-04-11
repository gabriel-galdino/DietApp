#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_FOOD_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_FOOD_H

#include <string>

class Food {
 public:
  Food(int id, const std::string& food_name);

  Food(const std::string& food_name);

  Food() = default;

  void set_id(int id) { id_ = id; }

  int id() const { return id_; }

  std::string name() const { return name_; }

  void set_name(const std::string& name) { name_ = name; }

  std::string display_name() const { return display_name_; }

  void set_display_name(const std::string& display_name) {
    display_name_ = display_name;
  }

  double proteins_per_100g() const { return proteins_per_100g_; }

  void set_proteins_per_100g(double val) { proteins_per_100g_ = val; }

  double carbs_per_100g() const { return carbs_per_100g_; }

  void set_carbs_per_100g(double val) { carbs_per_100g_ = val; }

  double fats_per_100g() const { return fats_per_100g_; }

  void set_fats_per_100g(double val) { fats_per_100g_ = val; }

  bool operator==(const Food& other) const { return id_ == other.id_; }

  bool operator<(const Food& other) const { return id_ < other.id_; }

 private:
  int id_{-1};
  std::string name_;
  std::string display_name_;
  double proteins_per_100g_;
  double carbs_per_100g_;
  double fats_per_100g_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_FOOD_H
