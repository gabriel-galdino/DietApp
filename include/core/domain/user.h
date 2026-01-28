#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_USER_H

#include <string>

class User {
 public:
  User(const std::string& name);

  virtual ~User() = default;

  std::string name() const { return name_; }

  bool operator==(const User& other) const { return name_ == other.name_; }

 private:
  std::string name_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
