#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_USER_H

#include <string>

class User {
 public:
  User(int id, const std::string& name);

  User(const std::string& name);

  std::string name() const { return name_; }

  void set_id(int id) { id_ = id; }

  int id() const { return id_; }

  bool operator==(const User& other) const { return id_ == other.id_; }

  bool operator<(const User& other) const { return id_ < other.id_; }

 private:
  int id_{-1};
  std::string name_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
