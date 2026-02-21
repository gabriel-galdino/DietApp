#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_USER_H

#include <string>

class User {
 public:
  User(int id, const std::string& username, const std::string& display_name);

  User(const std::string& username);

  std::string username() const { return username_; }

  std::string display_name() const { return display_name_; }

  void set_id(int id) { id_ = id; }

  int id() const { return id_; }

  bool operator==(const User& other) const { return id_ == other.id_; }

  bool operator<(const User& other) const { return id_ < other.id_; }

 private:
  int id_{-1};
  std::string username_;
  std::string display_name_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
